#ifndef __SGV_MESSAGE__
#define __SGV_MESSAGE__

#include "SgvSocket.h"
#include "ViewImage.h"
#include <vector>
#include <map>
#include <string>


namespace sigverse
{
	class RecvMsgEvent
	{
	private:
		std::string   m_from;
		std::string   m_msg;
	public:
		bool  setData(std::string data, int size);

		const char *getSender() { return m_from.c_str(); }

		const char *getMsg() { return m_msg.c_str(); }
	};

	class SIGService
	{
	public:
		SIGService(std::string name);
		SIGService();
		virtual ~SIGService();

		bool sendMsg(std::string to, std::string msg);

		bool sendMsgToCtr(std::string to, std::string msg);

		bool sendMsg(std::vector<std::string> to, std::string msg);

		bool connect(std::string host, int port);

		bool disconnect();

		bool disconnectFromController(std::string entityName);

		void disconnectFromAllController();

		void disconnectFromViewer();

		void startLoop(double time = -1.0);

		bool checkRecvData(int usec);

		bool connectToViewer();

		bool getIsConnectedView(){return m_connectedView;}

		ViewImage* captureView(std::string entityName, 
			int camID = 1,
			ColorBitType cbit = COLORBIT_24, 
			ImageDataSize size = IMAGE_320X240);

		unsigned char distanceSensor(std::string entityName, 
			double offset = 0.0, 
			double range = 255.0, 
			int camID = 1
			//ColorBitType cbit = DEPTHBIT_8
			);

		ViewImage* distanceSensor1D(std::string entityName, 
			double offset = 0.0, 
			double range = 255.0, 
			int camID = 1,
			ColorBitType cbit = DEPTHBIT_8,
			sigverse::ImageDataSize size = IMAGE_320X1);

		ViewImage* distanceSensor2D(std::string entityName, 
			double offset = 0.0, 
			double range = 255.0, 
			int camID = 1,
			ColorBitType cbit = DEPTHBIT_8,
			sigverse::ImageDataSize size = IMAGE_320X240);


		ViewImage* getDepthImage(std::string entityName, 
			double offset = 0.0, 
			double range = 255.0, 
			int camID = 1,
			ColorBitType cbit = DEPTHBIT_8,
			sigverse::ImageDataSize size = IMAGE_320X240);


		std::string getName() { return m_name; }

		void setName(std::string name) { m_name = name; }

		int getNewServiceNum() { return (int)m_elseServices.size() ; }

		std::vector<std::string> getAllOtherServices() { return m_elseServices; }

		std::vector<std::string> getAllConnectedEntitiesName() {
			return m_entitiesName;
		}

		/*
		std::string getElseServiceName() {
			int size = (int)m_elseServices.size(); 
			std::string name = m_elseServices[size - 1];
			m_elseServices.pop_back();
			return name;
		}
		*/

		//void setElseServiceName(std::string name) {
		//m_elseServices.push_back(name);
		//}

		SgvSocket* getSgvSocket(){
			return m_sgvsock;
		}

		SgvSocket* getControllerSocket(std::string name){
			std::map<std::string, sigverse::SgvSocket*>::iterator it;
			it = m_consocks.find(name);
			if(it != m_consocks.end())
				return (*it).second;
			else
				return NULL;
		}

		int getConnectedControllerNum() {
			return (int)m_consocks.size();
		}

		void setAutoExitLoop(bool flag){m_autoExitLoop = flag;}

		void setAutoExitProc(bool flag){m_autoExitProc = flag;}

		virtual void onInit(){}

		virtual void onRecvMsg(RecvMsgEvent &evt) {}

		virtual double onAction() { return 0.0;}

	private:
		ViewImage* getDistanceImage(std::string entityName, 
			double offset, 
			double range, 
			int camID,
			int dimension,
			ColorBitType cbit,
			sigverse::ImageDataSize size);


	protected:
		//! socket(for server)
		sigverse::SgvSocket *m_sgvsock;

		//! socket(for viewer)
		sigverse::SgvSocket *m_viewsock;

		std::map<std::string, SgvSocket*> m_consocks;

		std::vector<std::string> m_entitiesName;

		//! name
		std::string m_name;

		std::vector<std::string> m_elseServices;

		std::string m_host;

		bool m_start;

		bool m_connected;

		bool m_connectedView;

		bool m_autoExitLoop;

		bool m_autoExitProc;

		bool m_onLoop;
	};
}

#endif

