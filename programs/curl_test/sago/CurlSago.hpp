/*
  CurlSago. A Small header only C++ wrapper to force error checking and support C++ types. 
 
  Copyright (c) 2015 Poul Sander

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation files
  (the "Software"), to deal in the Software without restriction,
  including without limitation the rights to use, copy, modify, merge,
  publish, distribute, sublicense, and/or sell copies of the Software,
  and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
 */

#ifndef CURLSAGO_HPP
#define CURLSAGO_HPP

/*
 * This is some macros to ensure that error checking is done.
 * They are macros because curl_easy_setopt cannot be called otherwise
 */
#define CURLSAGO_SETOPT_MACRO_MSG(METHOD, PARAM, ERRORMSG) throw_on_curl_error(curl_easy_setopt(curl, METHOD, PARAM), ERRORMSG)
#define CURLSAGO_SETOPT_MACRO(METHOD, PARAM) throw_on_curl_error(curl_easy_setopt(curl, METHOD, PARAM))

#include <curl/curl.h>
#include <string>

namespace sago {

/**
 * This is the exception thrown by CurlSago. All exceptions thrown by this library
 * are derived from this.
 * All members are public and can be freely read
 */
class CurlSagoException : public std::exception {
public:
	std::string errorMsg;  //< The error message. This is also what what() retruns.
	CURLcode res = CURLE_OK;  //< The error code from curl if the error came from curl. If CURLE_OK (0) then the error was not libcurl related
	std::string url;  //< The url that was used. Can be used to identify the failing connection in complex systems
	
	/**
	 * Constructs a CurlSagoException with only an error message. 
	 * Can be used if the error is not libcurl or related
	 * @param errMsg The error message describing the error.
	 */
	CurlSagoException(const std::string& errMsg) : errorMsg(errMsg) {}
	
	/**
	 * Constructs a CurlSagoException with an error message and an URL
	 */
	CurlSagoException(const std::string& errMsg, const std::string& url) : errorMsg(errMsg), url(url) {}
	
	const char* what() const noexcept override  {
		return errorMsg.c_str();
	}
};


/**
 * This is an exception used for HTTP specific errors.
 * In addition to common CurlSagoException data this exception also contains
 * an http return code.
 */
class CurlSagoHttpException : public CurlSagoException {
	CurlSagoHttpException(const std::string& errMsg, const std::string& url, long http_code) : 
	CurlSagoException(errMsg, url), http_code(http_code) { }
	long http_code = 0;
};

class CurlSago {

	struct CurlSListHolder {
		curl_slist *slist = nullptr;

		void append(const char* headerStr) {
			curl_slist *slist_tmp = curl_slist_append(slist, headerStr);
			if (!slist_tmp) {
				throw CurlSagoException(std::string("Failed to append string: ") + headerStr);
			}
			slist = slist_tmp;
		}

		void clear() {
			if (slist) {
				curl_slist_free_all(slist);
				slist = nullptr;
			}
		}
		CurlSListHolder() = default;

		~CurlSListHolder() {
			clear();
		}
	private:
		CurlSListHolder(const CurlSListHolder&) = delete;
		CurlSListHolder& operator=(const CurlSListHolder&) = delete;
	};
public:
	CurlSListHolder headersIn;
public:

	CurlSago() {
		curl = curl_easy_init();
		if (!curl) {
			throw CurlSagoException("Failed to initialize libcurl");
		}
	}

	~CurlSago() {
		curl_easy_cleanup(curl);
	}

	/**
	 * Sets the url. 
	 * Always use this method for setting URL. The URL is included in exceptions
	 * and will give bad error messages if set in another way.
	 * Do not include the port! Use SetPort instead.
	 * @param url The URL to use.
	 */
	void SetUrl(const std::string& url) {
		CURLSAGO_SETOPT_MACRO_MSG(CURLOPT_URL, url.c_str(), std::string("Failed to set url: ") + url);
		this->url = url;
	}
	
	void SetUsername(const std::string& username) {
		CURLSAGO_SETOPT_MACRO(CURLOPT_USERNAME, username.c_str());
	}
	
	void SetPassword(const std::string& password) {
		CURLSAGO_SETOPT_MACRO(CURLOPT_USERNAME, password.c_str());
	}

	/**
	 * Set the port to use. Setting it to 0 will use the default port for the protocol.
	 * 
	 * @throws if port number is out of range and not 0
	 * 
	 * @param port Port to use if 1-65535 or 0 for the default port
	 */
	void SetPort(long port) {
		if (port < 0 || port > 65535) {
			throw CurlSagoException(std::string("Port number must be in range: 0-65535. Value given: ") + std::to_string(port));
		}
		CURLcode res = curl_easy_setopt(curl, CURLOPT_PORT, port);
		throw_on_curl_error(res, std::string("Failed to set port to: ") + std::to_string(port));
	}
	
	/**
	 * Performs an HTTP GET request. 
	 * @throws CurlSagoException if the retrun code is not 2XX
	 */
	void PerformHttpGet() {
		CURLSAGO_SETOPT_MACRO(CURLOPT_HTTPGET, 1L);
		internal_setup_output();
		CURLSAGO_SETOPT_MACRO(CURLOPT_HTTPHEADER, headersIn.slist);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			throw_on_curl_error(res, "Failed to perform GET operation");
		}
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
		throw_on_http_error_code(http_code, outputString);
	}
	
	/**
	 * Performs an HTTP POST request.
	 * @throws CurlSagoException if the return code is not 2XX
	 */
	void PerformHttpPost() {
		CURLSAGO_SETOPT_MACRO(CURLOPT_HTTPPOST, 1L);
		internal_setup_output();
		internal_setup_input();
		CURLSAGO_SETOPT_MACRO(CURLOPT_HTTPHEADER, headersIn.slist);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			throw_on_curl_error(res, "Failed to perform POST operation");
		}
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
		throw_on_http_error_code(http_code, outputString);
	}
	
	/**
	 * Performs an HTTP PUT request.
	 * This does the same as PerformUpload but verifies the http return code
	 * @throws CurlSagoException if the return code is not 2XX
	 */
	void PerformHttpPut() {
		PerformUpload();
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
		throw_on_http_error_code(http_code, outputString);
	}
	
	/**
	 * Performs the upload process for the given protocol. 
	 * @note Does not check the http code for HTTP request. Error may go unnoticed.
	 * @throws on error
	 */
	void PerformUpload() {
		CURLSAGO_SETOPT_MACRO(CURLOPT_UPLOAD, 1L);
		internal_setup_input();
		CURLSAGO_SETOPT_MACRO(CURLOPT_HTTPHEADER, headersIn.slist);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			throw_on_curl_error(res, "Failed to perform Upload operation");
		}
	}
	
	/**
	 *
	 */
	void Perform() {
		internal_setup_output();
		CURLSAGO_SETOPT_MACRO(CURLOPT_HTTPHEADER, headersIn.slist);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			throw_on_curl_error(res, "Failed to perform operation");
		}
	}

	/**
	 * Retrives the most recent http return code. 
	 * @return The most recent http return code
	 */
	long GetHttpReturnCode() const {
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
		return http_code;
	}
	
	std::string Escape(const std::string& input) const {
		std::string ret;
		char* ownedMemory = curl_easy_escape(curl, input.c_str(), input.length());
		if (!ownedMemory) { 
			char buffer[1024];
			snprintf(buffer, sizeof (buffer), "CurlSago Escape error escaping: \"%s\"", input.c_str());
			throw CurlSagoException(buffer);
		}
		ret = ownedMemory;
		curl_free(ownedMemory);
		return ret;
	}
	
	std::string UnEscape(const std::string& input) const {
		std::string ret;
		int outputLength = 0;
		char* ownedMemory = curl_easy_unescape(curl, input.c_str(), input.length(), &outputLength);
		ret.reserve(outputLength);
		if (!ownedMemory) { 
			char buffer[1024];
			snprintf(buffer, sizeof (buffer), "CurlSago UnEscape error unescaping: \"%s\"", input.c_str());
			throw CurlSagoException(buffer);
		}
		ret = ownedMemory;
		curl_free(ownedMemory);
		return ret;
	}
	
	/**
	 * Enabled output to a C++ output stream.
	 * Disables the output for other SetOutput* methods
	 * 
	 * Calling it with NULL will disable output.
	 * 
	 * The pointer must be valid for the lifetime of the CurlSago object or until this another SetOutput* method is called.
	 * 
	 * @param output Pointer to an output stream that the content will be written to.
	 */
	void SetOutputStream(std::ostream* output) {
		outputStream = output;
		outputString = nullptr;
	}
	
	/**
	 * Enables output to a C++ string
	 * Disables the output for other SetOutput* methods
	 * 
	 * Calling it with NULL will disable output.
	 * 
	 * The pointer must be valid for the lifetime of the CurlSago object or until this another SetOutput* method is called.
	 * 
	 * @param output
	 */
	void SetOutputString(std::string* output) {
		outputStream = nullptr;
		outputString = output;
	}

private:
	CurlSago(const CurlSago&) = delete;
	CurlSago& operator=(const CurlSago&) = delete;

	static size_t write_string_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
		std::string* s = static_cast<std::string*> (userdata);
		s->append(ptr, size * nmemb);
		return size * nmemb;
	}
	
	static size_t write_stream_callback (char *ptr, size_t size, size_t nmemb, void *userdata) {
		std::ostream* s = static_cast<std::ostream*> (userdata);
		s->write(ptr, size * nmemb);
		return size * nmemb;
	}
	
	static size_t read_stream_callback(char *buffer, size_t size, size_t nitems, void *instream) {
		std::istream* s = static_cast<std::istream*> (instream);
		s->read(buffer, size*nitems);
		return s->gcount();
	}
	
	struct read_string_callback_data {
		const std::string* s;
		size_t pos;
	};
	
	read_string_callback_data read_string_data;
	
	static size_t read_string_callback(char *buffer, size_t size, size_t nitems, void *instream) {
		read_string_callback_data* data = static_cast<read_string_callback_data*> (instream);
		size_t startPos = data->pos;
		size_t endPos = startPos + size*nitems;
		if (endPos > data->s->length()) {
			endPos = data->s->length();
		}
		size_t readSize = endPos-startPos;
		memcpy(buffer, data->s->data(), readSize);
		startPos = endPos;
		return readSize;
	}
	
	void internal_setup_output() {
		if (outputStream) {
			CURLSAGO_SETOPT_MACRO(CURLOPT_WRITEFUNCTION, sago::CurlSago::write_stream_callback);
			CURLSAGO_SETOPT_MACRO(CURLOPT_WRITEDATA, outputStream);
		} 
		if (outputString) {
			CURLSAGO_SETOPT_MACRO(CURLOPT_WRITEFUNCTION, sago::CurlSago::write_string_callback);
			CURLSAGO_SETOPT_MACRO(CURLOPT_WRITEDATA, outputString);
		}
	}
	
	void internal_setup_input() {
		if (inputStream) {
			CURLSAGO_SETOPT_MACRO(CURLOPT_READFUNCTION, sago::CurlSago::read_stream_callback);
			CURLSAGO_SETOPT_MACRO(CURLOPT_READDATA, &inputStream);
		}
		if (inputString) {
			read_string_data.pos = 0;
			read_string_data.s = inputString;
			CURLSAGO_SETOPT_MACRO(CURLOPT_READFUNCTION, sago::CurlSago::read_string_callback);
			CURLSAGO_SETOPT_MACRO(CURLOPT_READDATA, &read_string_data);
		}
	}

	void throw_on_curl_error(CURLcode code, const std::string& errorMsg = "") {
		if (code == CURLE_OK) {
			return;
		}
		char buffer[1024];
		snprintf(buffer, sizeof (buffer), "CurlSago error: \"%s\" (%i). %s", curl_easy_strerror(code), static_cast<int> (code), errorMsg.c_str());
		CurlSagoException e(buffer, url);
		e.res = code;
		throw e;
	}

	void throw_on_http_error_code(long http_code) {
		if (http_code >= 200 && http_code < 300) {
			return;
		}
		char buffer[1024];
		snprintf(buffer, sizeof (buffer), "CurlSago error. Failed to get \"%s\". Error code %li ", url.c_str(), http_code);
		throw CurlSagoException(buffer);
	}

	void throw_on_http_error_code(long http_code, std::string* response) {
		if (!response) {
			throw_on_http_error_code(http_code);
			return;
		}
		if (http_code >= 200 && http_code < 300) {
			return;
		}
		char buffer[1024];
		snprintf(buffer, sizeof (buffer), "CurlSago error. Failed to get \"%s\". Error code %li. \nThe response was:\n%s", url.c_str(), http_code, response->c_str());
		throw std::runtime_error(buffer);
	}
private:
	CURL *curl = nullptr;
	CURLcode res = CURLE_OK;
	long http_code = 0;
	std::string url; //Used for error reporting
	std::ostream* outputStream = nullptr;
	std::string* outputString = nullptr;
	std::istream* inputStream = nullptr;
	const std::string* inputString = nullptr;
};
}

#endif /* CURLSAGO_HPP */

