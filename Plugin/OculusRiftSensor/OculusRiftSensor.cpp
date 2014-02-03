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
	
	// an utility function for conversion of angle unit from radian to degree.
};

void OculusRiftService::onInit(){
	pManager=0;
	pHMD=0;
	pSensor=0;
	
	pManager = *DeviceManager::Create();
	pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();

	if (!pHMD){
		printf_s("[ERR]  Could not find Oculus Rift.\n");
		exit(EXIT_SUCCESS);
	}
	else{
		printf_s("[SYS]  An Oculus Rift is detected.\n");
	}
	pSensor = *pHMD->GetSensor();

	// Get DisplayDeviceName, ScreenWidth/Height, etc..
	HMDInfo hmdInfo;
	pHMD->GetDeviceInfo(&hmdInfo);

	if (pSensor){
		FusionResult.AttachToSensor(pSensor);
		printf_s("[SYS]  Sensors implemented in Oculus Rift are attached.\n");
	}
	else{
		printf_s("[ERR]  Could not find the sensors.\n");
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
	float d_yaw, d_pitch, d_roll;

	Quatf q = FusionResult.GetOrientation();
	Matrix4f bodyFrameMatrix(q);
	q.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&r_yaw, &r_pitch, &r_roll);

	d_yaw=RadToDegree(r_yaw);
	d_pitch=RadToDegree(r_pitch);
	d_roll=RadToDegree(r_roll);
	
	//printf("Euler angles[deg.]: Yaw, Ptch, Roll> %f, %f, %f \r", d_yaw, d_pitch, d_roll);

	std::vector<std::string> names;
	names = this->getAllConnectedEntitiesName();
	int entSize = names.size();
	for(int i = 0; i < entSize; i++) {
		std::cout<<names[i];
		std::string msg = "ORS_DATA ";
		msg += DoubleToString(d_yaw);
		msg += DoubleToString(d_pitch);
		msg += DoubleToString(d_roll);
		this->sendMsg(names[i], msg);
	}

	return 0.01;
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
		portnumber = atoi(argv[2]);
	}
	else{
		exit(0);
	}

	// connect to server
	srv.connect(saddr, portnumber);
	// connect to SIGViewer
    if(srv.connectToViewer()){
		srv.checkRecvData(50);
		// set exit condition for main-loop automatic
		// if SIGViewer is disconnected with server, main loop will be broken up
		srv.setAutoExitLoop(true);
	}

	// start main loop up
	srv.startLoop();  

	// finalize this process
	OVR::System::Destroy();
	exit(0);
}
