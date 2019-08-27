/*
  Its is under the MIT license, to encourage reuse by cut-and-paste.

  Copyright (c) 2019 Poul Sander

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


#include <iostream>
#include <boost/program_options.hpp>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include "hello.hpp"


#ifndef VERSIONNUMBER
#define VERSIONNUMBER "0.1.0"
#endif

int main(int argc, char* argv[]) {
	 try {
			cppcms::service srv(argc, argv);
			srv.applications_pool().mount( cppcms::applications_factory<HelloWorld>() );
			srv.run();
	} catch(std::exception const &e) {
			std::cerr << e.what() << std::endl;
	}
	return 0;
}
