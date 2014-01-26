#include "SIGService.h"
#include "binary.h"

#include <time.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
//#include <boost/thread.hpp>
//#include <boost/bind.hpp>

namespace sigverse
{
	std::string IntToString(int x)
	{
		char tmp[32];
		sprintf_s(tmp, 32,"%d",x);
		std::string str = std::string(tmp);
		str += ",";
		return str;
	}

	bool RecvMsgEvent::setData(std::string data, int size)
	{
		int strPos1 = 0;
		int strPos2;
		std::string tmpss;

		strPos2 = data.find(",", strPos1);
		m_from.assign(data, strPos1, strPos2-strPos1);

		strPos1 = strPos2 + 1;

		strPos2 = data.find(",", strPos1);
		tmpss.assign(data, strPos1, strPos2-strPos1);
		int msgSize = atoi(tmpss.c_str());

		strPos1 = strPos2 + 1;
		m_msg.assign(data, strPos1, msgSize);

		return true;
	}

	SIGService::SIGService(std::string name)
	{
		m_sgvsock = new SgvSocket();
		m_sgvsock->initWinsock();

		m_name = name;
		m_connectedView = false;
		m_viewsock = NULL;

		m_autoExitLoop = false;
		m_autoExitProc = false;
		m_onLoop = false;
	}

	SIGService::SIGService()
	{
		m_sgvsock = new SgvSocket();
		m_sgvsock->initWinsock();

		m_connectedView = false;
		m_viewsock = NULL;

		m_autoExitLoop = false;
		m_autoExitProc = false;
		m_onLoop = false;
	}

	SIGService::~SIGService()
	{
		delete m_sgvsock;

		if(m_viewsock != NULL)
			delete m_viewsock;
	}

	bool SIGService::connect(std::string host, int port)
	{
		if(m_sgvsock->connectTo(host.c_str(), port)) {

			std::string tmp = "SIGMESSAGE," + m_name + ",";
			m_sgvsock->sendData(tmp.c_str(), tmp.size());
		}
		else{
			MessageBox( NULL, "Could not connect to simserver", "Error", MB_OK);
			return false;
		}
		char tmp[8];
		memset(tmp, 0, sizeof(tmp));

		if(recv(m_sgvsock->getSocket(), tmp, sizeof(tmp), 0) < 0) {
			MessageBox( NULL, "Service: Failed to connect server", "Error", MB_OK);
		}

		if(strcmp(tmp,"SUCC") == 0) {
			m_host = host;
			return true;
		}
		else if(strcmp(tmp,"FAIL") == 0){
			char tmp[128];
			sprintf_s(tmp, 128, "Service name \"%s\" is already exist.", m_name.c_str());
			MessageBox( NULL, tmp, "Error", MB_OK);
			return false;
		}

		return true;
	}

	bool SIGService::disconnect()
	{
		if(!m_sgvsock->sendData("00004", 5)) return false;
		return true;
	}

	bool SIGService::sendMsgToCtr(std::string to, std::string msg)
	{
		std::map<std::string, SgvSocket*>::iterator it;
		it = m_consocks.find(to);

		if(it != m_consocks.end()) {
			int msgSize = msg.size();
			std::string ssize = IntToString(msgSize);
			//msg[msgSize] = '\0';
			SgvSocket *sock = (*it).second;
			std::string sendMsg = m_name + "," + ssize + msg;
			int tmpsize = sizeof(unsigned short) * 2;
			int sendSize = sendMsg.size() + tmpsize;

			char *sendBuff = new char[sendSize];
			char *p = sendBuff;

			BINARY_SET_DATA_S_INCR(p, unsigned short, 0x0002);
			BINARY_SET_DATA_S_INCR(p, unsigned short, sendSize);
			memcpy(p, sendMsg.c_str(), sendMsg.size());
			if(!sock->sendData(sendBuff, sendSize)) {
				delete [] sendBuff;			
				return false; 
			}

			delete [] sendBuff;	 
			return true;
		}
		else {
			return false;
		}
	}

	bool SIGService::sendMsg(std::string to, std::string msg)
	{
		char msize[6];
		int ms = (int)msg.size();
		sprintf_s(msize, 6,"%.5d", ms);
		std::string msg_size;
		msg_size = std::string(msize);

		std::string tmp_data = msg_size + "," + msg + "," + "-1.0," + "1," + to + ",";
		char bsize[6];
		int nbyte = (int)tmp_data.size() + 5;
		sprintf_s(bsize, 6,"%.5d", nbyte);
		std::string tmp_size = std::string(bsize);
		tmp_data = tmp_size + tmp_data;
		if(!m_sgvsock->sendData(tmp_data.c_str(), tmp_data.size())) return false;

		return true;
	}

	bool SIGService::sendMsg(std::vector<std::string> to, std::string msg)
	{
		char msize[6];
		int ms = (int)msg.size();
		sprintf_s(msize, 6, "%.5d", ms);
		std::string msg_size;
		msg_size = std::string(msize);

		std::string tmp_data = msg_size + "," + msg + "," + "-1.0,";

		int size = to.size();

		char tmp[6];
		sprintf_s(tmp, 6, "%d", size);
		std::string num = std::string(tmp);

		tmp_data += num + ","; // + to + "," + msg;
		for(int i = 0; i < size; i++) {
			tmp_data += to[i] + ",";
		}

		char bsize[6];
		int nbyte = (int)tmp_data.size() + 5;
		sprintf_s(bsize, 6, "%.5d", nbyte);
		std::string tmp_size = std::string(bsize);
		tmp_data = tmp_size + tmp_data;

		if(!m_sgvsock->sendData(tmp_data.c_str(), tmp_data.size())) return false;
		return true;
	}

	void SIGService::startLoop(double ltime)
	{
		m_start = true;
		static clock_t start;
		clock_t now;
		start = clock();

		m_onLoop = true;
		while(1){

			if(m_onLoop){
				if(ltime > 0){
					now = clock();
					double tmp =(double)(now - start)/CLOCKS_PER_SEC;
					if(tmp > ltime) {
						m_onLoop = false;
						break;
					}
				}
				if(!checkRecvData(100)) {
					m_onLoop = false;
					break;
				}
			}
			else break;
		}
		m_start = false;
	}

	bool SIGService::checkRecvData(int usec)
	{

		static bool init;
		struct timeval tv;

		tv.tv_sec = 0;
		tv.tv_usec = usec;

		SOCKET mainSock = m_sgvsock->getSocket();
		if(mainSock == NULL) return false;

		static double timewidth;
		static clock_t start;
		clock_t now;

		if(m_start && !init) {
			onInit();
			timewidth = onAction();
			start = clock();
			init = true;
		}
		else{
			now = clock();
			double tmp =(double)(now - start)/CLOCKS_PER_SEC;

			if(tmp > timewidth) {
				timewidth = onAction();
				start = clock();
			}
		}
		fd_set fds, readfds;

		int n;

		std::vector<sigverse::SgvSocket*> tmp_socks;
		tmp_socks.push_back(m_sgvsock);

		FD_ZERO(&readfds);

		FD_SET(mainSock,&readfds);

		if(m_connectedView){
			SOCKET viewSock = m_viewsock->getSocket();
			tmp_socks.push_back(m_viewsock);
			FD_SET(viewSock,&readfds);
		}

		std::map<std::string, SgvSocket*>::iterator it;
		it = m_consocks.begin();

		while(it != m_consocks.end()) {
			SOCKET sock = (*it).second->getSocket();
			FD_SET(sock,&readfds);
			tmp_socks.push_back((*it).second);
			it++;
		}

		memcpy(&fds, &readfds, sizeof(fd_set));

		n = select(0, &fds, NULL, NULL, &tv);

		if (n == 0) {
			return true;
		}
		else if(n < 0) {
			return false;
		}

		int connectSize = tmp_socks.size();
		for(int i = 0; i < connectSize; i++) {
			SOCKET nowsock = tmp_socks[i]->getSocket();

			if (FD_ISSET(nowsock, &fds)) {

				char bsize[4];
				if(!tmp_socks[i]->recvData(bsize,4)) return true;

				char *p = bsize;
				unsigned short header = BINARY_GET_DATA_S_INCR(p, unsigned short);
				unsigned short size = BINARY_GET_DATA_S_INCR(p, unsigned short);

				size -= 4;

				char *recvBuff = new char[size];
				if(size > 0)
					if(!tmp_socks[i]->recvData(recvBuff, size)) return true;

				char *nexttoken;

				switch(header) {
					case 0x0001:
						{
							RecvMsgEvent msg;
							msg.setData(recvBuff, size);
 
							onRecvMsg(msg);
							break;
						}
					case 0x0002:
						{
							std::string name = recvBuff;
							name[size] = '\0';
							m_elseServices.push_back(name);
							break;
						}
					case 0x0003:
						{
							char *pp = recvBuff;
							unsigned short port = BINARY_GET_DATA_S_INCR(pp, unsigned short);


							std::string name = strtok_s(pp, ",", &nexttoken);
							SgvSocket *sock = new SgvSocket();
							sock->initWinsock();

							if(sock->connectTo(m_host.c_str(), port)) {
								m_consocks.insert(std::map<std::string, SgvSocket*>::value_type(name, sock));
								m_entitiesName.push_back(name);
							}

							char tmp[sizeof(unsigned short)*2];
							char *p = tmp;

							BINARY_SET_DATA_S_INCR(p, unsigned short, 0x0001);
							BINARY_SET_DATA_S_INCR(p, unsigned short, sizeof(unsigned short));

							sock->sendData(tmp, sizeof(unsigned short)*2);

							break;
						}
					case 0x0004:
						{
							char *pp = recvBuff;
							std::string ename = strtok_s(pp, ",", &nexttoken);

							disconnectFromController(ename);

							std::map<std::string, SgvSocket*>::iterator sockit;
							sockit = m_consocks.find(ename);
							if(sockit != m_consocks.end()) {
								m_consocks.erase(sockit);
							}
							return false;
						}
					case 0x0005:
						{
							delete m_viewsock;
							m_viewsock = NULL;
							m_connectedView = false;

							if(m_autoExitProc){
								exit(0);
							}
							else if(m_autoExitLoop){
								m_onLoop = false;
							}

							break;
						}
				}
				delete [] recvBuff;
			} //if (FD_ISSET(mainSock, &fds)) {
		} // for(int i = 0; i < connectSize; i++){
		return true;
	}

	bool SIGService::connectToViewer()
	{
		m_viewsock = new SgvSocket();
		//m_viewsock->initWinsock();

		if(!m_viewsock->connectTo("localhost", 11000)) {
			MessageBox( NULL, "Could not connect to viewer", "Error", MB_OK);
			return false;
		}
		else {
			int nsize = m_name.size();
			int sendSize = nsize + sizeof(unsigned short)*2;
			char *sendBuff = new char[sendSize];
			char *p = sendBuff;

			BINARY_SET_DATA_S_INCR(p, unsigned short, 0x0000);
			BINARY_SET_DATA_S_INCR(p, unsigned short, sendSize);

			memcpy(p, m_name.c_str(), m_name.size());
			if(!m_viewsock->sendData(sendBuff, sendSize)) {
				delete [] sendBuff;
			}
			delete [] sendBuff;

			m_connectedView = true;
			return true;
		}
	}
	bool SIGService::disconnectFromController(std::string entityName)
	{
		std::map<std::string, SgvSocket*>::iterator it;
		it = m_consocks.find(entityName);

		if(it != m_consocks.end()) {
			SgvSocket *sock = (*it).second;

			char tmp[sizeof(unsigned short)*2];
			char *p = tmp;

			BINARY_SET_DATA_S_INCR(p, unsigned short, 0x0004);
			BINARY_SET_DATA_S_INCR(p, unsigned short, sizeof(unsigned short));

			sock->sendData(tmp, sizeof(unsigned short)*2);

			m_consocks.erase(it);
			delete sock;

			return true;
		}
		else
			return false;
	}

	void SIGService::disconnectFromAllController()
	{
		std::map<std::string, SgvSocket*>::iterator it;
		it = m_consocks.begin();

		while(it != m_consocks.end()) {
			SgvSocket *sock = (*it).second;

			char tmp[sizeof(unsigned short)*2];
			char *p = tmp;

			BINARY_SET_DATA_S_INCR(p, unsigned short, 0x0004);
			BINARY_SET_DATA_S_INCR(p, unsigned short, sizeof(unsigned short));

			sock->sendData(tmp, sizeof(unsigned short)*2);

			delete sock;
			it++;
		}
		m_consocks.clear();
	}

	void SIGService::disconnectFromViewer()
	{
		if(!m_connectedView || m_viewsock == NULL) return;

		char buf[4];
		char *p = buf;
		BINARY_SET_DATA_S_INCR(p, unsigned short, 0x0003);
		BINARY_SET_DATA_S_INCR(p, unsigned short, 0x0004);

		m_viewsock->sendData(buf, 4);

		delete m_viewsock;
		m_viewsock = NULL;
	}

	ViewImage* SIGService::captureView(std::string entityName, int camID, ColorBitType ctype,ImageDataSize size)
	{
		if(!m_connectedView){
			MessageBox( NULL, "captureView: Service is not connected to viewer", "Error", MB_OK);
			return NULL;
		}
		std::string cameraID = IntToString(camID);
		std::string camSize = IntToString(size);
		std::string sendMsg = entityName + "," + cameraID + camSize;
		int tmpsize = sizeof(unsigned short) * 2;
		int sendSize = sendMsg.size() + tmpsize;

		char *sendBuff = new char[sendSize];
		char *p = sendBuff;

		BINARY_SET_DATA_S_INCR(p, unsigned short, 0x0001);
		BINARY_SET_DATA_S_INCR(p, unsigned short, sendSize);
		memcpy(p, sendMsg.c_str(), sendMsg.size());

		if(!m_viewsock->sendData(sendBuff, sendSize)) {
			delete [] sendBuff;			
			return NULL; 
		}

		ViewImageInfo info(IMAGE_DATA_WINDOWS_BMP, COLORBIT_24, IMAGE_320X240);
		ViewImage *view = new ViewImage(info);

		int headerSize = sizeof(unsigned short) + sizeof(double)*2;
		int imageSize = 320*240*3;

		char *headerBuff = new char[headerSize];

		if(!m_viewsock->recvData(headerBuff, headerSize)) {
			delete [] headerBuff;
			return NULL; 
		}

		p = headerBuff;
		double fov;
		double ar;
		unsigned short result = BINARY_GET_DATA_S_INCR(p, unsigned short);
		if(result == 2){
			char tmp[64];
			sprintf_s(tmp, 64,"captureView: cannot find entity [%s]", entityName.c_str());
			MessageBox( NULL, tmp, "Error", MB_OK);
			delete [] headerBuff;
			return NULL;
		}
		else if (result == 3){
			char tmp[64];
			sprintf_s(tmp, 64,"captureView: %s doesn't have camera id %d", entityName.c_str(), camID);
			MessageBox( NULL, tmp, "Error", MB_OK);
			delete [] headerBuff;
			return NULL;
		}
		else{
			fov = BINARY_GET_DOUBLE_INCR(p);
			ar = BINARY_GET_DOUBLE_INCR(p);
			view->setFOVy(fov);
			view->setAspectRatio(ar);
		}

		delete [] headerBuff;

		char *imageBuff = new char[imageSize];
		if(!m_viewsock->recvData(imageBuff, imageSize)) {
			delete [] imageBuff;
			return NULL; 
		}

		view->setBuffer(imageBuff);
		return view;
	}

	unsigned char SIGService::distanceSensor(std::string entityName, double offset, double range, int camID)
	{
		if(!m_connectedView){
			MessageBox( NULL, "distanceSensor: Service is not connected to viewer", "Error", MB_OK);
			return 255;
		}

		std::string cameraID = IntToString(camID);
		std::string sendMsg = entityName + "," + cameraID;

		int tmpsize = sizeof(double) * 2 + sizeof(unsigned short)*2;
		int sendSize = sendMsg.size() + tmpsize;

		char *sendBuff = new char[sendSize];
		char *p = sendBuff;

		BINARY_SET_DATA_S_INCR(p, unsigned short, 0x0002);
		BINARY_SET_DATA_S_INCR(p, unsigned short, sendSize);
		BINARY_SET_DOUBLE_INCR(p, offset);
		BINARY_SET_DOUBLE_INCR(p, range);
		memcpy(p, sendMsg.c_str(), sendMsg.size());

		if(!m_viewsock->sendData(sendBuff, sendSize)) {
			delete [] sendBuff;			
			return 255; 
		}

		delete [] sendBuff;
		int recvSize = sizeof(unsigned short) + 1;

		char *recvBuff = new char[recvSize];
		if(!m_viewsock->recvData(recvBuff, recvSize)) {
			delete [] recvBuff;
			return 255; 
		}
		p = recvBuff;

		unsigned short result = BINARY_GET_DATA_S_INCR(p, unsigned short);
		if(result == 2){
			char tmp[128];
			sprintf_s(tmp, 128,"distanceSensor: cannot find entity [%s]", entityName.c_str());
			MessageBox( NULL, tmp, "Error", MB_OK);
			delete [] recvBuff;
			return 255;
		}
		else if (result == 3){
			char tmp[128];
			sprintf_s(tmp, 128,"distanceSensor: %s doesn't have camera [id: %d]", entityName.c_str(), camID);
			MessageBox( NULL, tmp, "Error", MB_OK);
			delete [] recvBuff;
			return 255;
		}
		unsigned char distance = p[0];
		delete [] recvBuff;
		return distance;
	}

	ViewImage* SIGService::distanceSensor1D(std::string entityName, double offset, double range, int camID, ColorBitType ctype, ImageDataSize size)
	{
		if(!m_connectedView){
			MessageBox( NULL, "distanceSensor1D: Service is not connected to viewer", "Error", MB_OK);
			return NULL;
		}
		return getDistanceImage(entityName, offset, range, camID, 1, ctype, size);
	}

	ViewImage* SIGService::distanceSensor2D(std::string entityName, double offset, double range, int camID, ColorBitType ctype, ImageDataSize size)
	{
		if(!m_connectedView){
			MessageBox( NULL, "distanceSensor2D: Service is not connected to viewer", "Error", MB_OK);
			return NULL;
		}
		return getDistanceImage(entityName, offset, range, camID, 2, ctype, size);
	}

	ViewImage* SIGService::getDistanceImage(std::string entityName, double offset, double range, int camID, int dimension, ColorBitType ctype, ImageDataSize size)
	{
		std::string dim = IntToString(dimension);
		std::string cameraID = IntToString(camID);
		std::string camSize = IntToString(size);
		std::string sendMsg = entityName + "," + dim + cameraID + camSize;

		int tmpsize = sizeof(double) * 2 + sizeof(unsigned short)*2;

		int sendSize = sendMsg.size() + tmpsize;
		char *sendBuff = new char[sendSize];
		char *p = sendBuff;

		BINARY_SET_DATA_S_INCR(p, unsigned short, 0x0004);
		BINARY_SET_DATA_S_INCR(p, unsigned short, sendSize);
		BINARY_SET_DOUBLE_INCR(p, offset);
		BINARY_SET_DOUBLE_INCR(p, range);
		memcpy(p, sendMsg.c_str(), sendMsg.size());

		if(!m_viewsock->sendData(sendBuff, sendSize)){
			delete [] sendBuff;
			return NULL;
		}

		delete [] sendBuff;

		int headerSize = sizeof(unsigned short) + sizeof(double)*2;
		char *header = new char[headerSize];
		if(!m_viewsock->recvData(header, headerSize)){
			return NULL;
		}
		p = header;
		unsigned short result = BINARY_GET_DATA_S_INCR(p, unsigned short);

		if(result == 2){
			char tmp[128];
			if(dimension == 1)
				sprintf_s(tmp, 128,"distanceSensor1D: cannot find entity [%s]", entityName.c_str());
			else
				sprintf_s(tmp, 128,"distanceSensor2D: cannot find entity [%s]", entityName.c_str());
			MessageBox( NULL, tmp, "Error", MB_OK);
			delete [] header;
			return NULL;
		}
		else if (result == 3){
			char tmp[128];
			if(dimension == 1)
				sprintf_s(tmp, 128,"distanceSensor1D: %s doesn't have camera [id: %d]", entityName.c_str(), camID);
			else
				sprintf_s(tmp, 128,"distanceSensor2D: %s doesn't have camera [id: %d]", entityName.c_str(), camID);
			MessageBox( NULL, tmp, "Error", MB_OK);
			delete [] header;
			return NULL;
		}

		double fov = BINARY_GET_DOUBLE_INCR(p);
		double ar = BINARY_GET_DOUBLE_INCR(p);

		delete [] header;

		int recvSize;
		if(dimension == 1){
			recvSize = 320;
		}
		else if(dimension == 2){
			recvSize = 320*240;
		}
		char *recvBuff = new char[recvSize];
		if(!m_viewsock->recvData(recvBuff, recvSize)) {
			delete [] recvBuff;
			return NULL; 
		}

		ViewImage *img;
		if(dimension == 1){
			ViewImageInfo info(IMAGE_DATA_WINDOWS_BMP, DEPTHBIT_8, IMAGE_320X1);
			img = new ViewImage(info);
		}

		else if(dimension == 2){
			ViewImageInfo info(IMAGE_DATA_WINDOWS_BMP, DEPTHBIT_8, IMAGE_320X240);
			img = new ViewImage(info);
		}
		img->setFOVy(fov);
		img->setAspectRatio(ar);
		img->setBuffer(recvBuff);
		//unsigned char *recvData = new unsigne
		//ViewImage *img;
		return img;
	}

	ViewImage* SIGService::getDepthImage(std::string entityName, double offset, double range, int camID, ColorBitType ctype, ImageDataSize size)
	{
		if(!m_connectedView){
			MessageBox( NULL, "getDepthImage: Service is not connected to viewer", "Error", MB_OK);
			return NULL;
		}

		std::string cameraID = IntToString(camID);
		std::string camSize = IntToString(size);
		std::string sendMsg = entityName + "," + cameraID + camSize;

		int tmpsize = sizeof(double) * 2 + sizeof(unsigned short)*2;

		int sendSize = sendMsg.size() + tmpsize;
		char *sendBuff = new char[sendSize];
		char *p = sendBuff;

		BINARY_SET_DATA_S_INCR(p, unsigned short, 0x0005);
		BINARY_SET_DATA_S_INCR(p, unsigned short, sendSize);
		BINARY_SET_DOUBLE_INCR(p, offset);
		BINARY_SET_DOUBLE_INCR(p, range);
		memcpy(p, sendMsg.c_str(), sendMsg.size());

		if(!m_viewsock->sendData(sendBuff, sendSize)){
			delete [] sendBuff;
			return NULL;
		}

		delete [] sendBuff;

		int headerSize = sizeof(unsigned short) + sizeof(double)*2;
		char *header = new char[headerSize];
		if(!m_viewsock->recvData(header, headerSize)){
			return NULL;
		}
		p = header;
		unsigned short result = BINARY_GET_DATA_S(p, unsigned short);

		if(result == 2){
			char tmp[128];
			sprintf_s(tmp, 128,"getDepthImage: cannot find entity [%s]", entityName.c_str());
			MessageBox( NULL, tmp, "Error", MB_OK);
			delete [] header;
			return NULL;
		}
		else if (result == 3){
			char tmp[128];
			sprintf_s(tmp, 128,"getDepthImage: %s doesn't have camera [id: %d]", entityName.c_str(), camID);
			MessageBox( NULL, tmp, "Error", MB_OK);
			delete [] header;
			return NULL;
		}

		double fov = BINARY_GET_DOUBLE_INCR(p);
		double ar = BINARY_GET_DOUBLE_INCR(p);

		delete [] header;

		int recvSize = 320*240;

		char *recvBuff = new char[recvSize];
		if(!m_viewsock->recvData(recvBuff, recvSize)) {
			delete [] recvBuff;
			return NULL; 
		}

		ViewImage *img;
		ViewImageInfo info(IMAGE_DATA_WINDOWS_BMP, DEPTHBIT_8, IMAGE_320X240);
		img = new ViewImage(info);

		img->setFOVy(fov);
		img->setAspectRatio(ar);
		img->setBuffer(recvBuff);
		//unsigned char *recvData = new unsigne
		//ViewImage *img;
		return img;
		return NULL;
	}

};

