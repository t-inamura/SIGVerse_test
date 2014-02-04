#include<stdio.h>
#include<stdlib.h>
#include<iostream>

#include"SIGService.h"

#include "OVR.h"
#include "Kernel/OVR_KeyCodes.h"

using namespace OVR;

#undef new  // reset the redefinition in OVR*.h headers to normal one

class OculusRiftService : public sigverse::SIGService{
	Ptr<DeviceManager> pManager;
	Ptr<HMDDevice>     pHMD;
	Ptr<SensorDevice>  pSensor;
	SensorFusion       FusionResult;

public:
    OculusRiftService(std::string name) : SIGService(name){};
    ~OculusRiftService();

	// initialization for whole procedure on this service
	void onInit();

	// function to be called if this service receives messages
	//void onRecvMsg(sigverse::RecvMsgEvent &evt);
	
	// function to be called periodically
	double onAction();
	
	// an utility function for conversion of data type from float to string.
	std::string DoubleToString(float x);	
};

void OculusRiftService::onInit(){
	pManager=0;
	pHMD=0;
	pSensor=0;
	
	pManager = *DeviceManager::Create();
	pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();

	if (!pHMD){
//		printf_s("[ERR]  Could not find Oculus Rift.\n");
		exit(EXIT_SUCCESS);
	}
	else{
//		printf_s("[SYS]  An Oculus Rift is detected.\n");
	}
	pSensor = *pHMD->GetSensor();

	// Get DisplayDeviceName, ScreenWidth/Height, etc..
	HMDInfo hmdInfo;
	pHMD->GetDeviceInfo(&hmdInfo);

	if (pSensor){
		FusionResult.AttachToSensor(pSensor);
//		printf_s("[SYS]  Sensors implemented in Oculus Rift are attached.\n");
	}
	else{
//		printf_s("[ERR]  Could not find the sensors.\n");
	}
}

OculusRiftService::~OculusRiftService(){
    this->disconnect();
	
	pSensor.Clear();
	pHMD.Clear();
	pManager.Clear();
}

double OculusRiftService::onAction(){
	float r_yaw, r_pitch, r_roll;

	Quatf q = FusionResult.GetOrientation();
	Matrix4f bodyFrameMatrix(q);
	q.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&r_yaw, &r_pitch, &r_roll);
	
	//printf_s("Euler angles[deg.]: Yaw, Ptch, Roll> %f, %f, %f \r", d_yaw, d_pitch, d_roll);

	//this->checkRecvData(1);

	std::vector<std::string> names;
	names = this->getAllConnectedEntitiesName();
	int entSize = names.size();
	for(int i = 0; i < entSize; i++) {
		std::string msg = "ORS_DATA ";
		msg += DoubleToString(r_yaw);
		msg += DoubleToString(r_pitch);
		msg += DoubleToString(r_roll);
		this->sendMsg(names[i], msg);
	}

	//this->checkRecvData(1);
	return 0.05;
}

std::string OculusRiftService::DoubleToString(float x){
	char tmp[32];
	sprintf_s(tmp, 32, "%.3f", x);
	std::string str = std::string(tmp);
	str += ",";
	return str;
}

void main(int argc, char* argv[]){
	// start OVR system
	OVR::System::Init();

	// create instance
	OculusRiftService srv("SIGORS");

	char saddr[128];
	unsigned int portnumber;
	if (argc > 1) {
		sprintf_s(saddr, 128, "%s", argv[1]);
		portnumber = (unsigned int)atoi(argv[2]);
	}
	else{
		exit(0);
	}

	// connect to server
	if(srv.connect(saddr, portnumber)){
	// connect to SIGViewer
//    if(srv.connectToViewer()){
		//srv.checkRecvData(50);
		// set exit condition for main-loop automatic
		// if SIGViewer is disconnected with server, main loop will be broken up
		//srv.setAutoExitLoop(true);
		srv.setAutoExitProc(true);
//	}
	}

	// start main loop up
	srv.startLoop();  

	// finalize this process
	OVR::System::Destroy();
	exit(0);
}
