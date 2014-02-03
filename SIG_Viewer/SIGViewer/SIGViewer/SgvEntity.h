#ifndef __SGVENTITY__
#define __SGVENTITY__

#include <Ogre.h>
#include "CX3DParser.h"

namespace Sgv
{

	class SgvEntity
	{
	public:

		//SgvEntity(Ogre::SceneNode *headnode, Ogre::String name, Ogre::String shapefile,
		//Ogre::Vector3 pos, Ogre::Quaternion rot, Ogre::Vector3 scale,unsigned int id = 0);

		SgvEntity(Ogre::SceneNode *headnode, Ogre::String name, Ogre::Vector3 pos, 
			Ogre::Quaternion rot, Ogre::Vector3 scale,unsigned int id = 0);

		~SgvEntity();

		//bool createObj(CX3DParser *parser, Ogre::SceneNode *headnode);
		//bool createShape(Ogre::ManualObject *obj, CX3DShapeNode *shape); 

		bool createObj(CX3DParser *parser, Ogre::SceneManager *mgr, bool currshape, int alg);

		bool createTransform(Ogre::SceneNode *parent, CX3DTransformNode *trans, Ogre::SceneManager *mgr, int count); 

		bool createShape(Ogre::SceneNode *parent, CX3DShapeNode *shape, Ogre::SceneManager *mgr); 

		Ogre::Entity *createSimpleShape(int type, Ogre::SceneNode *parent, Ogre::SceneManager *mgr, CX3DNode *geo);

		bool createRobotObj(CX3DParser *parser, Ogre::SceneManager *mgr);

		bool createRobotJoint(Ogre::SceneNode *parent, CX3DOpenHRPJointNode *jNode, Ogre::SceneManager *mgr);

		bool createRobotSegment(Ogre::SceneNode *parent, CX3DOpenHRPSegmentNode *sNode, Ogre::SceneManager *mgr);

		bool computeCenterOfMass(CX3DParser *parser, int alg);

		void addCameraName(int id ,std::string cam){ 
			m_cameraNames.insert(std::map<int, std::string>::value_type(id, cam));
		}

		void addCameraPosEntity(Ogre::Entity* ent){m_allCameraPosEntities.push_back(ent);}

		void addCameraArrowEntity(Ogre::ManualObject* ent){m_allCameraArrowEntities.push_back(ent);}

		void setCameraPositionVisible(bool vis){
			std::vector<Ogre::Entity*>::iterator it = m_allCameraPosEntities.begin();
			while(it != m_allCameraPosEntities.end()){
				(*it)->setVisible(vis);
				it++;
			}
		}

		void setCameraArrowVisible(bool vis){
			std::vector<Ogre::ManualObject*>::iterator it = m_allCameraArrowEntities.begin();
			while(it != m_allCameraArrowEntities.end()){
				(*it)->setVisible(vis);
				it++;
			}
		}

		std::map<int, std::string> getCameraName(){ return m_cameraNames; }

		std::string getCameraName(int id){ 
			std::map<int, std::string>::iterator it = m_cameraNames.find(id);
			if(it != m_cameraNames.end()){
				return (*it).second;
			}
			else{
				return "";
			}
			return "";
		}

		void setTransparency(float a);

		void resetTransparency();

		int getCameraNum(){ 
			//if(m_cameraNames.empty()) return 0;
			//else
			return m_cameraNames.size();
		}

		int getJointNum(){ return m_allJoints.size();}

		void changeShape(Ogre::String shape);

		void setisAttach(bool isAttach){m_isAttach = isAttach;}

		bool isAttach(){return m_isAttach;}

		Ogre::String getName() { return m_name;}

		std::vector<Ogre::Entity*> getAllEntities() { return m_allEntities;}

		Ogre::SceneNode* getHeadNode() { return m_entHeadNode;}

		Ogre::SceneNode* getBoundingNode() { return m_NodeForBounding;}

		void setBoundingBoxVisible(bool vis) { m_NodeForBounding->showBoundingBox(vis);}

		void createPositionMark(Ogre::SceneManager *mgr);

		void setPositionMarkVisible(bool vis){mPosition->setVisible(vis);}

		void setJointPositionVisible(bool vis){
			std::vector<Ogre::Entity*>::iterator it = m_allJPosEntities.begin();
			while(it != m_allJPosEntities.end()){
				(*it)->setVisible(vis);
				it++;
			}
		}
		void setSegmentPositionVisible(bool vis){
			std::vector<Ogre::Entity*>::iterator it = m_allSegPosEntities.begin();
			while(it != m_allSegPosEntities.end()){
				(*it)->setVisible(vis);
				it++;
			}
		}


		Ogre::Vector3 getBBoxSize(){return mBBoxSize;}

		bool isRobot(){return m_isRobot;}

		bool MinMaxFromMFNode(MFNode *node,
			double *min_x, 
			double *min_y,
			double *min_z,
			double *max_x,
			double *max_y,
			double *max_z);

		bool SgvEntity::MinMaxFromIndexedNode(CX3DCoordinateNode* Coord,
			double *min_x, 
			double *min_y,
			double *min_z,
			double *max_x,
			double *max_y,
			double *max_z);

		bool COPFromMFNode(MFNode *node);

		void setVisible(bool vis);

		void createBoundingBox(Ogre::SceneManager *mgr);

		Ogre::Vector3 getScale(){return mScale;}

	private:

		Ogre::String m_name;

		Ogre::String m_sfile;

		Ogre::SceneNode *m_entHeadNode;

		Ogre::String m_currShape;

		unsigned int m_id;

		bool m_isRobot;

		bool m_isAttach;

		float m_gx;
		float m_gy;
		float m_gz;

		std::map<int, std::string> m_cameraNames;

		std::vector<Ogre::Entity*> m_allEntities;

		std::map<Ogre::MaterialPtr, float> m_allMaterials;

		Ogre::Entity *mPosition;

		Ogre::Vector3 mBBoxSize;

		std::vector<Ogre::String> m_allJoints;

		std::vector<Ogre::Entity*> m_allJPosEntities;

		std::vector<Ogre::Entity*> m_allSegPosEntities;

		std::vector<Ogre::Entity*> m_allCameraPosEntities;

		std::vector<Ogre::ManualObject*> m_allCameraArrowEntities;

		Ogre::SceneNode *m_NodeForBounding;

		Ogre::Vector3 mScale;


	};
}

#endif // __SGVENTITY__
