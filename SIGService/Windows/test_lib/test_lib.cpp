#include "SIGService.h"
//#include "ViewImage.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <tchar.h>
#include <string>
#include <fstream>
#include <time.h>



class MyService : public sigverse::SIGService
{

public:
	MyService(std::string name) : SIGService(name){};
	~MyService();
	void onRecvMsg(sigverse::RecvMsgEvent &evt);
	void onInit();
	double onAction();
	int getCount(){return count;}

private:
	int count;
};


MyService::~MyService()  
{  
	//MessageBox( NULL, "destractor", "Error", MB_OK);
    this->disconnect();  
}

void MyService::onInit()
{
	count = 0;
	//MessageBox( NULL, "oninit", "Error", MB_OK);
}


double MyService::onAction()
{
	return 5.0;
}

void MyService::onRecvMsg(sigverse::RecvMsgEvent &evt)
{
	std::string msg = evt.getMsg();
	MessageBox( NULL, msg.c_str(), _T("Error"), MB_OK);
}


void main(void){
	
}


