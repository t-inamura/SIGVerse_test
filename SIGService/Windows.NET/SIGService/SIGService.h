// SIGService.h

#pragma once

#include "stdafx.h"

#include "SgvSocket.h"
#include "ViewImage.h"
#include <vector>
#include <map>
#include <string>
#include <cliext/map>
#include <cliext/vector>

using namespace System::Collections::Generic;

using namespace System;

namespace sigverse {

	public ref class RecvMsgEvent
	{
	public:
		RecvMsgEvent(): m_from(nullptr), m_msg(nullptr){};
		bool  setData(String^ data, int size);

		String^ getSender() { return m_from; }

		String^ getMsg() { return m_msg; }

	private:
		System::String^   m_from;
		System::String^   m_msg;

	};


	public ref class SIGService
	{

	public:
		SIGService(String^ name);
		SIGService();
		virtual ~SIGService();

		bool sendMsg(String^ to, String^ msg);

		bool sendMsgToCtr(String^ to, String^ msg);

		bool sendMsg(List<String^>^ to, String^ msg);

		bool connect(String^ host, int port);

		bool disconnect();

		bool disconnectFromController(String^ entityName);

		void disconnectFromAllController();

		void disconnectFromViewer();

		void startLoop(double time);

		void startLoop(){startLoop(-1.0);}

		bool checkRecvData(int usec);

		bool connectToViewer();

		bool getIsConnectedView(){return m_connectedView;}

		ViewImage^ captureView(String^ entityName, 
			int camID,
			ColorBitType cbit, 
			ImageDataSize size);

		ViewImage^ captureView(String^ entityName){ return captureView(entityName,1,COLORBIT_24,IMAGE_320X240);} 
		ViewImage^ captureView(String^ entityName,int camID){ return captureView(entityName,camID,COLORBIT_24,IMAGE_320X240);} 
		ViewImage^ captureView(String^ entityName,int camID, ColorBitType cbit){ return captureView(entityName,camID,cbit,IMAGE_320X240);} 

		unsigned char distanceSensor(String^ entityName, 
			double offset, 
			double range, 
			int camID);
		unsigned char distanceSensor(String^ entityName){return distanceSensor(entityName, 0.0, 255.0, 1);} 
		unsigned char distanceSensor(String^ entityName, double offset){return distanceSensor(entityName, offset, 255.0, 1);} 
		unsigned char distanceSensor(String^ entityName, double offset, double range){return distanceSensor(entityName, offset, range, 1);} 

		ViewImage^ distanceSensor1D(String^ entityName, 
			double offset, 
			double range, 
			int camID,
			ColorBitType cbit,
			sigverse::ImageDataSize size);
		ViewImage^ distanceSensor1D(String^ entityName){return distanceSensor1D(entityName, 0.0, 255.0, 1, DEPTHBIT_8, IMAGE_320X1);} 
		ViewImage^ distanceSensor1D(String^ entityName, double offset){return distanceSensor1D(entityName, offset, 255.0, 1, DEPTHBIT_8, IMAGE_320X1);} 
		ViewImage^ distanceSensor1D(String^ entityName, double offset, double range){return distanceSensor1D(entityName, offset, range, 1, DEPTHBIT_8, IMAGE_320X1);} 
		ViewImage^ distanceSensor1D(String^ entityName, double offset, double range, int camID){return distanceSensor1D(entityName, offset, range, camID, DEPTHBIT_8, IMAGE_320X1);} 
		ViewImage^ distanceSensor1D(String^ entityName, double offset, double range, int camID, ColorBitType cbit){return distanceSensor1D(entityName, offset, range, camID, cbit, IMAGE_320X1);} 

		ViewImage^ distanceSensor2D(String^ entityName, 
			double offset, 
			double range, 
			int camID,
			ColorBitType cbit,
			sigverse::ImageDataSize size);
		ViewImage^ distanceSensor2D(String^ entityName){return distanceSensor2D(entityName, 0.0, 255.0, 1, DEPTHBIT_8, IMAGE_320X240);} 
		ViewImage^ distanceSensor2D(String^ entityName, double offset){return distanceSensor2D(entityName, offset, 255.0, 1, DEPTHBIT_8, IMAGE_320X240);} 
		ViewImage^ distanceSensor2D(String^ entityName, double offset, double range){return distanceSensor2D(entityName, offset, range, 1, DEPTHBIT_8, IMAGE_320X240);} 
		ViewImage^ distanceSensor2D(String^ entityName, double offset, double range, int camID){return distanceSensor2D(entityName, offset, range, camID, DEPTHBIT_8, IMAGE_320X240);} 
		ViewImage^ distanceSensor2D(String^ entityName, double offset, double range, int camID, ColorBitType cbit){return distanceSensor2D(entityName, offset, range, camID, cbit, IMAGE_320X240);} 

		ViewImage^ getDepthImage(String^ entityName, 
			double offset, 
			double range, 
			int camID,
			ColorBitType cbit,
			sigverse::ImageDataSize size );
		ViewImage^ getDepthImage(String^ entityName){return getDepthImage(entityName, 0.0, 255.0, 1, DEPTHBIT_8, IMAGE_320X240);} 
		ViewImage^ getDepthImage(String^ entityName, double offset){return getDepthImage(entityName, offset, 255.0, 1, DEPTHBIT_8, IMAGE_320X240);} 
		ViewImage^ getDepthImage(String^ entityName, double offset, double range){return getDepthImage(entityName, offset, range, 1, DEPTHBIT_8, IMAGE_320X240);} 
		ViewImage^ getDepthImage(String^ entityName, double offset, double range, int camID){return getDepthImage(entityName, offset, range, camID, DEPTHBIT_8, IMAGE_320X240);} 
		ViewImage^ getDepthImage(String^ entityName, double offset, double range, int camID, ColorBitType cbit){return getDepthImage(entityName, offset, range, camID, cbit, IMAGE_320X240);} 

		String^ getName() { return m_name; }

		void setName(String^ name) { m_name = name; }

		int getNewServiceNum() { return (int)m_elseServices.Count; }

		List<System::String^>^ getAllOtherServices() { 
			return %m_elseServices; 
		}

		List<System::String^>^ getAllConnectedEntitiesName() {
			return %m_entitiesName;
		}

		SgvSocket^ getSgvSocket(){
			return m_sgvsock;
		}

		SgvSocket^ getControllerSocket(String^ name){
			cliext::map<String^, sigverse::SgvSocket^>::iterator it;
			it = m_consocks.find(name);
			if(it != m_consocks.end())
				return (*it)->second;
			else
				return nullptr;
		}

		int getConnectedControllerNum() {
			return (int)m_consocks.size();
		}

		void setAutoExitLoop(bool flag){m_autoExitLoop = flag;}

		void setAutoExitProc(bool flag){m_autoExitProc = flag;}

		virtual void onInit() {}

		virtual void onRecvMsg(RecvMsgEvent ^evt){}

		virtual double onAction() { return 0.0;}

	private:
		ViewImage^ getDistanceImage(String^ entityName, 
			double offset, 
			double range, 
			int camID,
			int dimension,
			ColorBitType cbit,
			sigverse::ImageDataSize size);


		//protected:
	private:
		//! socket(for server)
		sigverse::SgvSocket^ m_sgvsock;

		//! socket(for viewer)
		sigverse::SgvSocket^ m_viewsock;

		cliext::map<System::String^, SgvSocket^> m_consocks;

    public:
		List<String^> m_entitiesName;

		List<String^> m_elseServices;

	protected:

		//! name
		String^ m_name;

		String^ m_host;

		bool m_start;

		bool m_connected;

		bool m_connectedView;

		bool m_autoExitLoop;

		bool m_autoExitProc;

		bool m_onLoop;
	};
}

