// WiiRemote.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <string>
#include "SIGService.h"
#include "include\wiimote.h"



class WiiRemote_Service : public sigverse::SIGService
{

public:
	WiiRemote_Service(std::string name);
	~WiiRemote_Service();
    double onAction();
	void onRecvMsg(sigverse::RecvMsgEvent &evt);
	wiimote cWiiRemote;
private: 
	HWND hWnd;	
};

WiiRemote_Service::WiiRemote_Service(std::string name) : SIGService(name){

	_tprintf(_T("contains WiiYourself! wiimote code by gl.tter\nhttp://gl.tter.org\n"));

	static const TCHAR* wait_str[] = { _T(".  "), _T(".. "), _T("...") };
	unsigned count = 0;
	while(!cWiiRemote.Connect(wiimote::FIRST_AVAILABLE)) {
		_tprintf(_T("\b\b\b\b%s "), wait_str[count%3]);
		count++;
		Beep(500, 30); Sleep(1000);
	}

	_tprintf(_T("Connected.\n"));
	hWnd = NULL;
};

WiiRemote_Service::~WiiRemote_Service(){
	cWiiRemote.Disconnect();
	_tprintf(_T("Disconnected.\n"));
	this->disconnect();
}

double WiiRemote_Service::onAction(){
	if(cWiiRemote.bExtension) cWiiRemote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR_EXT); // no IR dots
	else cWiiRemote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR);      //    IR dots
	while(cWiiRemote.RefreshState() == NO_CHANGE){
	}
	if(cWiiRemote.Button.A()){			
		std::string msg = "go";
		sendMsgToCtr("man_000", msg);
		std::cout<<msg<<std::endl;
		//Sleep(1000);
	}
	else if(cWiiRemote.Button.B()){			
		std::string msg = "take";
		sendMsgToCtr("man_000", msg);
		std::cout<<msg<<std::endl;
		//Sleep(1000);
	}
	else if(cWiiRemote.Button.Plus()){			
		std::string msg = "put";
		sendMsgToCtr("man_000", msg);
		std::cout<<msg<<std::endl;
		//Sleep(1000);
	}
	return 1.0;
}

void WiiRemote_Service::onRecvMsg(sigverse::RecvMsgEvent &evt){
   
}




int main(int argc, char* argv[])
{
	WiiRemote_Service srv("Wii_Service");
	unsigned short port = (unsigned short)(atoi(argv[2]));
	srv.connect(argv[1], port);
	srv.startLoop();

	return 0;
}

