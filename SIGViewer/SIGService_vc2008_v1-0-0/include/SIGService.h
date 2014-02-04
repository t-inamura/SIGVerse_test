#ifndef __SGV_MESSAGE__
#define __SGV_MESSAGE__

#include "SgvSocket.h"
#include <vector>
#include <map>

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

	class RecvCptEvent
	{
	public:
		void setSender(std::string name){ m_from = name; }

		std::string getSender(){return m_from;}

		void setCameraID(int id ){m_id = id;}

		int getCameraID(){return m_id;}

	private:
		std::string   m_from;
		int        m_id;    
	};

	class RecvDtcEvent
	{
	public:
		void setSender(std::string name){ m_from = name; }

		std::string getSender(){return m_from;}

		void setCameraID(int id ){m_id = id;}

		int getCameraID(){return m_id;}

	private:
		std::string   m_from;
		int        m_id;    
	};

	class RecvDstEvent
	{
	public:
		void setSender(std::string name){ m_from = name; }

		void setCameraID(int id ){m_id = id;}

		std::string getSender(){return m_from;}

		int getCameraID(){return m_id;}

		void setType(int type ){m_type = type;}

		int getType(){return m_type;}

		void setMin(float min){m_min = min;}

		float getMin(){return m_min;}

		void setMax(float max){m_max = max;}

		float getMax(){return m_max;}

	private:
		std::string   m_from;
		int   m_id;  
		int      m_type;    
		float   m_min;     
		float   m_max;     
	};


	class SIGService
	{
	public:
		SIGService(std::string name);
		SIGService();
		~SIGService();

		bool sendMsg(std::string to, std::string msg);

		bool sendMsgToCtr(std::string to, std::string msg);

		bool sendMsg(std::vector<std::string> to, std::string msg);

		bool connect(std::string host, int port);

		bool disconnect();

		bool disconnectFromController(std::string entityName);

		void disconnectFromAllController();

		bool startService();

		bool connectToView();

		std::string getName() { return m_name; }

		void setName(std::string name) { m_name = name; }

		int getNewServiceNum() { return m_elseServices.size() ; }

		std::vector<std::string> getConnectedAllEntitiyName() {
			m_entitiesName;
		}

		std::string getElseServiceName() {
			int size = m_elseServices.size(); 
			std::string name = m_elseServices[size - 1];
			m_elseServices.pop_back();
			return name;
		}

		void setElseServiceName(std::string name) {
			m_elseServices.push_back(name);
		}

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
			return m_consocks.size();
		}


		virtual void recvMsg(RecvMsgEvent &evt) {}

		virtual void recvCaptureView(RecvCptEvent &evt) {}

		virtual void recvDistanceSensor(RecvDstEvent &evt) {}

		virtual void recvDetectEntities(RecvDtcEvent &evt) {}


	private:
		void recvDataLoop(SgvSocket *sock);


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
	};
}

#endif

