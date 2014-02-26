#include<stdio.h>
#include<stdlib.h>

#include"SIGService.h"

#include "OVR.h"
#include <iostream>
#include <conio.h>

//using namespace OVR;

#undef new  // reset the redefinition in OVR*.h headers to normal one

class OculusRiftService : public sigverse::SIGService{
	OVR::Ptr<OVR::DeviceManager>  pManager;
	OVR::Ptr<OVR::HMDDevice>      pHMD;
	OVR::Ptr<OVR::SensorDevice>   pSensor;
	OVR::SensorFusion*       pFusionResult;
	OVR::HMDInfo             Info;
	bool                InfoLoaded;

public:
    OculusRiftService(std::string name) : SIGService(name){};
    ~OculusRiftService();

	// initialization for whole procedure on this service
	void onInit();

	// get information of Oculus Rift
	void getInfoOculusRift();

	// function to be called if this service receives messages
	//void onRecvMsg(sigverse::RecvMsgEvent &evt);
	
	// function to be called periodically
	double onAction();
	
	// an utility function for conversion of data type from float to string.
	std::string DoubleToString(float x);	
};

void OculusRiftService::onInit(){
	// initialization(prepare varibles for data acquisition of OculusRift's sensors)
	OVR::System::Init();

	pFusionResult = new OVR::SensorFusion();
	pManager = *OVR::DeviceManager::Create();

	pHMD = *pManager->EnumerateDevices<OVR::HMDDevice>().CreateDevice();

	if (pHMD){
		InfoLoaded = pHMD->GetDeviceInfo(&Info);
		pSensor = *pHMD->GetSensor();
	}
	else{
	   pSensor = *pManager->EnumerateDevices<OVR::SensorDevice>().CreateDevice();
	}

	if (pSensor){
	   pFusionResult->AttachToSensor(pSensor);
	}
}

void OculusRiftService::getInfoOculusRift(){
	//display info. of attached Oculus Rift
	//---------------------------------------------------------------
	std::cout << "----- Oculus Console -----" << std::endl;

	if (pHMD){
		std::cout << " [x] HMD Found" << std::endl;
	}
	else{
		std::cout << " [ ] HMD Not Found" << std::endl;
	}

	if (pSensor){
		std::cout << " [x] Sensor Found" << std::endl;
	}
	else{
		std::cout << " [ ] Sensor Not Found" << std::endl;
	}

	std::cout << "--------------------------" << std::endl;

	if (InfoLoaded){
		std::cout << " DisplayDeviceName: " << Info.DisplayDeviceName << std::endl;
		std::cout << " ProductName: " << Info.ProductName << std::endl;
		std::cout << " Manufacturer: " << Info.Manufacturer << std::endl;
		std::cout << " Version: " << Info.Version << std::endl;
		std::cout << " HResolution: " << Info.HResolution<< std::endl;
		std::cout << " VResolution: " << Info.VResolution<< std::endl;
		std::cout << " HScreenSize: " << Info.HScreenSize<< std::endl;
		std::cout << " VScreenSize: " << Info.VScreenSize<< std::endl;
		std::cout << " VScreenCenter: " << Info.VScreenCenter<< std::endl;
		std::cout << " EyeToScreenDistance: " << Info.EyeToScreenDistance << std::endl;
		std::cout << " LensSeparationDistance: " << Info.LensSeparationDistance << std::endl;
		std::cout << " InterpupillaryDistance: " << Info.InterpupillaryDistance << std::endl;
		std::cout << " DistortionK[0]: " << Info.DistortionK[0] << std::endl;
		std::cout << " DistortionK[1]: " << Info.DistortionK[1] << std::endl;
		std::cout << " DistortionK[2]: " << Info.DistortionK[2] << std::endl;
		std::cout << "--------------------------" << std::endl;
	}
}

OculusRiftService::~OculusRiftService(){
	// shutdown SIGService
    this->disconnect();
	
	// finalize Oculus Rift connection
	pSensor.Clear();
	pHMD.Clear();
	pManager.Clear();
	delete pFusionResult;
	OVR::System::Destroy();
}

//periodic procedure for sending messages to the controller
double OculusRiftService::onAction(){
	float r_yaw, r_pitch, r_roll;

	OVR::Quatf q = pFusionResult->GetOrientation();
	OVR::Matrix4f bodyFrameMatrix(q);
	q.GetEulerAngles<OVR::Axis_Y, OVR::Axis_X, OVR::Axis_Z>(&r_yaw, &r_pitch, &r_roll);

	std::vector<std::string> names;
	names = this->getAllConnectedEntitiesName();
	int entSize = names.size();
	for(int i = 0; i < entSize; i++) {
		std::string msg = "ORS_DATA ";
		msg += DoubleToString(r_yaw);
		msg += DoubleToString(r_pitch);
		msg += DoubleToString(r_roll);
		this->sendMsgToCtr(names[i], msg);
		std::cout<<msg<<std::endl;
	}

	return 0.1;  //time period
}

// utility function to convert data type from 'double' to 'string'
std::string OculusRiftService::DoubleToString(float x){
	char tmp[32];
	sprintf_s(tmp, 32, "%.3f", x);
	std::string str = std::string(tmp);
	str += ",";
	return str;
}

//entry point
void main(int argc, char* argv[]){
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

	// connect to SIGServer
	if(srv.connect(saddr, portnumber)){
	// connect to SIGViewer
		if(srv.connectToViewer()){
		// set exit condition of main-loop automatic
		// if SIGViewer is disconnected from server, main loop will be broken up
			srv.setAutoExitLoop(true);
		}
	}

	// start main loop
	srv.startLoop();  

	// finalize this process
	exit(0);
}
