
#ifndef __SgvMain_h_
#define __SgvMain_h_
 
#include "BaseApplication.h"
#include "SgvSocket.h"
#include "SgvEntity.h"
#include "SgvX3D.h"
#include "ViewerService.h"

#include <stdio.h> 
#include <map>
#include <CEGUI.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <libssh2.h>

class SgvMain : public BaseApplication
{
public:
    SgvMain(void);
    virtual ~SgvMain(void);

	enum RequestType{
		GET_ALL_ENTITIES  = 1, // get all entity data.
		START             = 2, // simulation start
		STOP              = 3, // simulation stop
		GET_MOVE_ENTITIES = 4, // update entities
		DOWNLOAD          = 5, // download shape file.
		DISCONNECT        = 6, // disconnect to server
		QUIT              = 7, // quit simulation
	};

	enum ServiceRequestType{
		SV_NODATA            = 0, 
		SV_GET_IMAGE         = 1,  // captureView
		SV_GET_DISTANCE      = 2,  // distanceSensor
		SV_DISCONNECT        = 3,  // disconnect
		SV_GET_DISTANCEIMAGE = 4,  // distanceImage
		SV_GET_DEPTHIMAGE    = 5,  // depthImage
		SV_REQUEST_SIZE   ,
	};

	enum ImageType{
		IM_320X240 = 0,
	};

	enum ColorBitType{
		COLORBIT_24 = 0,
		DEPTHBIT_8  = 1,
	};

protected:
    CEGUI::OgreRenderer* mRenderer;
 
    virtual void createScene(void);
    virtual void destroyScene(void);
 
	virtual void chooseSceneManager(void);
    virtual void createFrameListener(void);
 
    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
 
    // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    virtual void windowResized(Ogre::RenderWindow* rw);

    bool quit(const CEGUI::EventArgs &e);

    bool connect(const CEGUI::EventArgs &e);

    bool disconnect(const CEGUI::EventArgs &e);

    bool closeRecvMessageTray(const CEGUI::EventArgs &e);

    bool closeSendMessageTray(const CEGUI::EventArgs &e);

    bool addService(const CEGUI::EventArgs &e);

    bool editService(const CEGUI::EventArgs &e);

    bool removeService(const CEGUI::EventArgs &e);

    bool okService(const CEGUI::EventArgs &e);

    bool createAllEntities();   

	void closeChAndSock(LIBSSH2_CHANNEL *channel, SOCKET sock);

	Sgv::SgvEntity *getSgvEntity(std::string name){
		std::map<std::string, Sgv::SgvEntity*>::iterator it = mAllEntities.find(name);
		if(it == mAllEntities.end()) return NULL;
		else return (*it).second;
	}

    bool startRequest(const CEGUI::EventArgs &e);

    bool agentView1(const CEGUI::EventArgs &e);
    bool agentView2(const CEGUI::EventArgs &e);
    bool agentView3(const CEGUI::EventArgs &e);
    bool agentView4(const CEGUI::EventArgs &e);

	bool cameraView_Down  (const CEGUI::EventArgs &e);
	bool cameraView_Move  (const CEGUI::EventArgs &e);
	bool cameraView_Up    (const CEGUI::EventArgs &e);
    bool cameraView_Sized (const CEGUI::EventArgs &e);
    bool cameraView_Sizing(const CEGUI::EventArgs &e);

    int cameraView_Select(const CEGUI::EventArgs &e, int *result);

	bool startService1(const CEGUI::EventArgs &e);
	bool startService2(const CEGUI::EventArgs &e);
	bool startService3(const CEGUI::EventArgs &e);
	bool startService4(const CEGUI::EventArgs &e);
	bool startService5(const CEGUI::EventArgs &e);
	bool startService6(const CEGUI::EventArgs &e);

	bool startService(std::string fullpath);

    bool sshCheckONOFF(const CEGUI::EventArgs &e);

    bool subView(const CEGUI::EventArgs &e);

    bool overwriteShape(const CEGUI::EventArgs &e);

    bool dynamicsView(const CEGUI::EventArgs &e);

    bool changeTimeUnit(const CEGUI::EventArgs &e);

	bool messageTray(const CEGUI::EventArgs &e);

	bool sendMessage(const CEGUI::EventArgs &e);

	bool displayEntityData(const CEGUI::EventArgs &e);

	bool setMOV(const CEGUI::EventArgs &e);

	bool setCOV(const CEGUI::EventArgs &e);

	bool sendRequest(RequestType type);

	bool recvMoveEntities();

	bool downloadFileRequest(std::string name);

private:
	void setDepthView(bool depth);

	void createInitWindow();

	void acceptFromService();

	bool captureView();

	bool distanceSensor();

	bool detectEntities();

	bool getImage(Ogre::Camera *cam, unsigned char *image, int headSize = 4, ColorBitType ctype = COLORBIT_24, ImageType viewType = IM_320X240);

	bool getDistance(Ogre::Camera *cam, unsigned char *distance, double offset, double range, ColorBitType ctype = DEPTHBIT_8);

	bool getDistanceImage(Ogre::Camera *cam, unsigned char *distance, double offset, double range, int dimension, ColorBitType ctype = DEPTHBIT_8);

	bool getDepthImage(Ogre::Camera *cam, unsigned char *distance, double offset, double range, ColorBitType ctype = DEPTHBIT_8, ImageType viewType = IM_320X240);

	bool setCameraForDistanceSensor(Ogre::Camera *cam);

	//------------------------------------------------------
	//------------------------------------------------------
	LIBSSH2_SESSION *sshLogin(const char *uname, const char *key1, const char *key2, const char *p\
ass, const char *host);

	//------------------------------------------------------
	//------------------------------------------------------
	void sshPortForwarding(unsigned int localport, unsigned int remoteport, const char *listenhost);


	//------------------------------------------------------
	//------------------------------------------------------
	void transportData(int id);

	bool checkRequestFromService();

protected:
	Ogre::RaySceneQuery *mRaySceneQuery;// The ray scene query pointer
	bool mLMouseDown, mRMouseDown;		
	bool mShift;                		
	bool mCtrl;                 		
	int mCount;							// The number of robots on the screen
	Ogre::SceneNode *mCurrentObject;	// pointer to our currently selected object
	Ogre::SceneNode *mHeadNode;	
	float mRotateSpeed;		
	float mMoveXYSpeed;		
	float mMoveZSpeed;		
	bool  mConnectServer;               
	bool  mSimRun;                      
	bool  mSubView;                     
	std::string mHost;                  
	std::string mPort;                  
	CEGUI::Renderer *mGUIRenderer;      // our CEGUI renderer

    bool mMove;                         
    int  mTidx;                         
    int  mBm[2];                        
                          
	sigverse::SgvSocket *mSock;

	Sgv::ViewerService *mService;

	std::map<std::string ,Sgv::SgvEntity*> mAllEntities;

	// 
	Sgv::X3D *m_pX3D;

	std::vector<Ogre::Viewport*> mViews;

	std::vector<CEGUI::Window*> mSubWindows;

	std::vector<CEGUI::Window*> mSubViews;

	CEGUI::Window *mTelop;
	//CEGUI::Listbox *mTelop;

	std::map<CEGUI::String, CEGUI::ListboxTextItem*> mMsgList; 

	std::vector<CEGUI::ListboxTextItem*> mEntityDataList; 

	Sgv::LogPtr mLog;

	//std::vector<CEGUI::String> mElseUser;
	std::list<CEGUI::String> mElseUsers;

	std::map<std::string, sigverse::SgvSocket*> mServices;

	sigverse::SgvSocket *mRecvService; 

	Ogre::TexturePtr mTex;

	Ogre::TexturePtr mDstTex;

	Ogre::Camera  *mCaptureCamera;

	Ogre::Camera  *mDistanceCamera;

	Ogre::Camera  *OculusCamera;

	//Ogre::Camera  *mDetectCamera;

	Ogre::ManualObject* mPlane;

	Ogre::ManualObject* mAxis;

	Ogre::Entity *mGround;

	bool mSended;

	Ogre::PlaneBoundedVolumeListSceneQuery *m_VolQuery;

	SOCKET m_sshSock;

	LIBSSH2_SESSION *m_session;

	bool mSSHConnect;

	std::vector<std::string> mPSrvs;

	TCHAR mSettingPath[128];

	std::vector<SOCKET> m_transSocks;

	std::vector<LIBSSH2_CHANNEL*> m_transChannels;

	int m_dynamicsView;

	std::vector<Ogre::SceneNode*> m_ODENodes;

	bool mOverWrite;

	Sgv::SgvEntity* mEntityData;

	// 1 Average vertices (old)  
	// 2 Center of Vertices (default)
	int mAlgEntityPos;

	Ogre::String mCurrentEntityName;

	std::vector<Ogre::ManualObject*>  mCameraArrows;

};
 
#endif // #ifndef __SgvMain_h_

