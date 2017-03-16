#include <iostream>
#include <boost/program_options.hpp>
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif


#ifndef VERSIONNUMBER
#define VERSIONNUMBER "0.1.0"
#endif

class HelloWorldApp : public wxApp
{
public:
	virtual bool OnInit();
};

DECLARE_APP(HelloWorldApp)

bool HelloWorldApp::OnInit()
{
	wxFrame *frame = new wxFrame((wxFrame*) NULL, -1, _T("Hello wxWidgets World"));
	frame->CreateStatusBar();
	frame->SetStatusText(_T("Hello World"));
	frame->Show(true);
	SetTopWindow(frame);
	return true;
}

IMPLEMENT_APP_NO_MAIN(HelloWorldApp);
IMPLEMENT_WX_THEME_SUPPORT;

int main(int argc, char* argv[]) {
	boost::program_options::options_description desc("Options");
	desc.add_options()
	("version", "Print version information and quit")
	("help,h", "Print basic usage information to stdout and quit")
	;
	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);
	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 0;
	}
	if (vm.count("version")) {
		std::cout << "hello_world " << VERSIONNUMBER << "\n";
		return 0;
	}
	return wxEntry(argc, argv);
}
