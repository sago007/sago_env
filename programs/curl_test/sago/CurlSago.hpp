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

#include <curl/curl.h>
#include <string>

namespace sago {

class CurlSago {
    struct CurlSListHolder {
        curl_slist *slist = nullptr;
        void append(const char* headerStr) {
            curl_slist *slist_tmp = curl_slist_append(slist, headerStr);
            if (!slist_tmp) {
                throw std::runtime_error(std::string("Failed to append string: ")+headerStr);
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
    long http_code = 0;
    CurlSListHolder headersIn;
public:
    CurlSago() {
        curl = curl_easy_init();
        if (!curl) {
            throw std::runtime_error("Failed to initialize libcurl");
        }
    }
    
    ~CurlSago() {
        curl_easy_cleanup(curl);
    }
    
    void SetUrl(const std::string& url) {
        CURLcode res = curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        throw_on_curl_error(res, std::string("Failed to set url: ")+url);
        this->url = url;
    }
    
    void SetPort(long port) {
        if (port < 0 || port > 65535) {
            throw std::runtime_error(std::string("Port number must be in range: 0-65535. Value given: ")+std::to_string(port));
        }
        CURLcode res = curl_easy_setopt(curl, CURLOPT_PORT, port);
        throw_on_curl_error(res, std::string("Failed to set port to: ")+std::to_string(port));
    }
    
    std::string PerformGet() {
        
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        std::string result;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, sago::CurlSago::write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result); 
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headersIn.slist);
        curl_easy_perform(curl);
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
        throw_on_http_error_code(http_code, result);
        return result;
    }
    
private:
    CurlSago(const CurlSago&) = delete;
    CurlSago& operator=(const CurlSago&) = delete;
    
    static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
        std::string* s = static_cast<std::string*>(userdata);
        s->append(ptr, size*nmemb);
        return size;
    }
    void throw_on_curl_error (CURLcode code, const std::string& errorMsg = "") {
        if (code == CURLE_OK) {
            return;
        }
        char buffer[1024];
        snprintf(buffer, sizeof(buffer), "CurlSago error: \"%s\" (%i). %s", curl_easy_strerror(code), static_cast<int>(code), errorMsg.c_str() );
        throw std::runtime_error(buffer);
    }
    void throw_on_http_error_code(long http_code) {
        if (http_code >= 200 && http_code < 300) {
            return;
        }
        char buffer[1024];
        snprintf(buffer, sizeof(buffer), "CurlSago error. Failed to get \"%s\". Error code %li ", url.c_str(), http_code );
        throw std::runtime_error(buffer);
    }
    void throw_on_http_error_code(long http_code, std::string& response) {
        if (http_code >= 200 && http_code < 300) {
            return;
        }
        char buffer[1024];
        snprintf(buffer, sizeof(buffer), "CurlSago error. Failed to get \"%s\". Error code %li. \nThe response was:\n%s", url.c_str(), http_code, response.c_str() );
        throw std::runtime_error(buffer);
    }
private:
    CURL *curl = nullptr;
    CURLcode res = CURLE_OK;
    std::string url;  //Used for error reporting
    
};
}

#endif /* CURLSAGO_HPP */

