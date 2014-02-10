#include <winsock2.h>
#include <errno.h>
#include "binary.h"
#include "SgvSocket.h"

namespace sigverse
{
	SgvSocket::SgvSocket()
	{
		m_sock = NULL;

		Sgv::LogFactory::setLog(1, Sgv::LogPtr(new Sgv::Log(Sgv::Log::DEBUG, "main", 
			Sgv::LogFileWriterAutoPtr(new Sgv::DelayFileWriter<Sgv::FileLock>("SgvSocket.log"))))
			);
		mLog = Sgv::LogFactory::getLog(1);
	}

	SgvSocket::SgvSocket(SOCKET sock)
	{
		m_sock = sock;
	}


	SgvSocket::~SgvSocket()
	{
		closesocket(m_sock);
		WSACleanup();
	}

	bool SgvSocket::initWinsock()
	{

		WSADATA wsaData;

		int err = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (err != 0)
		{
			switch (err) {
				case WSASYSNOTREADY:
					{
						mLog->err("WSASYSNOTREADY\n");
						break;
					}
				case WSAVERNOTSUPPORTED:
					{
						mLog->err("WSAVERNOTSUPPORTED\n");
						break;
					}
				case WSAEINPROGRESS:
					{
						mLog->err("WSAEINPROGRESS\n");
						break;
					}
				case WSAEPROCLIM:
					{
						mLog->err("WSAEPROCLIM\n");
						break;
					}
				case WSAEFAULT:
					{
						mLog->err("WSAEFAULT\n");
						break;
					}
			}
			WSACleanup();
			return false;
		}

		//log->flush();			 
		return true;
	}

	bool SgvSocket::connectTo(const char *hostname, int port)
	{
		//Sgv::LogPtr log = Sgv::LogFactory::getLog(0);

		struct sockaddr_in server;
		//char buf[32];
		unsigned int **addrptr;

		m_sock = socket(AF_INET, SOCK_STREAM, 0);

		server.sin_family = AF_INET;
		server.sin_port = htons(port);

		server.sin_addr.S_un.S_addr = inet_addr(hostname);
		if (server.sin_addr.S_un.S_addr == 0xffffffff) {
			struct hostent *host;

			host = gethostbyname(hostname);
			if (host == NULL) {
				if(WSAGetLastError() == WSAHOST_NOT_FOUND) {
					MessageBox(NULL, "Could not find host.", "error", MB_OK);
					mLog->printf(Sgv::LogBase::ERR,"host not found : %s\n", hostname);
				}
				return false;
			}

			addrptr = (unsigned int **)host->h_addr_list;

			while (*addrptr != NULL) {
				server.sin_addr.S_un.S_addr = *(*addrptr);

				if (connect(m_sock,
					(struct sockaddr *)&server,
					sizeof(server)) == 0) {
						break;
				}

				addrptr++;
			}

			if (*addrptr == NULL) {
				MessageBox(NULL, "Could not connect", "error", MB_OK);
				mLog->printf(Sgv::LogBase::ERR, "connect failed: %d\n", WSAGetLastError());
				return false;
			}
		}
		else
		{

			if (connect(m_sock,
				(struct sockaddr *)&server,
				sizeof(server)) != 0) {
					MessageBox(NULL, "Could not connect", "error", MB_OK);
					mLog->printf(Sgv::LogBase::ERR, "connect : %d\n", WSAGetLastError());
					mLog->flush();
					return false;
			}
		}

		return true;
	}

	bool SgvSocket::sendData( const char *msg, int size)
	{
		int sended = 0;

		while(1){

			int r = send(m_sock, msg + sended, size - sended, 0);

			if(r < 0)
			{
				if(errno == EAGAIN || errno == WSAEWOULDBLOCK){
					Sleep(1);
					continue;
				}
				else{
					return false;
				}
			}
			sended += r;

			if(size == sended) break;

		}
		return true;
	}

	bool SgvSocket::recvData( char *msg, int size)
	{
		int recieved = 0;

		while(1){

			int r = recv(m_sock, msg + recieved, size - recieved, 0);

			if(r < 0)
			{
				if(errno == EAGAIN || errno == WSAEWOULDBLOCK){
					Sleep(1);
					continue;
				}
				else{
					return false;
				}
			}
			recieved += r;

			if(size == recieved) break;

		}
		return true;
	}

}

