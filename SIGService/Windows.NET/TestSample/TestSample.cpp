
#include "stdafx.h"
#include "Form1.h"

using namespace TestSample;

#include <tchar.h>  
#include <string>  

  
ref class MyService : public sigverse::SIGService  
{  
  
public:  
    MyService(String^ name) : SIGService(name){};  
    ~MyService();  
	virtual void onRecvMsg(sigverse::RecvMsgEvent ^evt) override;  
	virtual double onAction() override;  
};  
  
MyService::~MyService()  
{  
    this->disconnect();  
}  
  
double MyService::onAction()  
{  
    return 1.0;  
}  
  
void MyService::onRecvMsg(sigverse::RecvMsgEvent^ evt)  
{  
    String^ sender = evt->getSender();  
    String^ msg = evt->getMsg();  
  
    if(msg == "Hello") {  

		//this->sendMsgToCtr(sender, "Hello! this is MyService");  
		//sigverse::ViewImage^ img = this->captureView(sender);
		//sigverse::ViewImage^ img = this->getDepthImage(sender,0, 700);
		sigverse::ViewImage^ img = this->distanceSensor1D(sender);
		unsigned char *data = (unsigned char*)img->getBuffer();
		//img->saveAsWindowsBMP("test.bmp");
    }  
}  

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

    MyService srv("MyService");  
//  unsigned short port = (unsigned short)(atoi(argv[2]));  
//  srv.connect(argv[1], port);  
  
    srv.connect("socio4.iir.nii.ac.jp", 9001);  
	srv.connectToViewer();  

    srv.startLoop();  

	Application::Run(gcnew Form1());
	return 0;
}

