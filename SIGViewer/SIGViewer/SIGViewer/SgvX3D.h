#ifndef _SGV_X3D_H_
#define _SGV_X3D_H_

#include "Ogre.h"
#include "CX3DParser.h"
#include "CX3DShapeNode.h"
#include "CX3DTransformNode.h"
#include "CX3DIndexedFaceSetNode.h"
#include "CSimplifiedShape.h"

#define MAX_POINTS_PER_ONE_POLYGON		1000
#define EXT_ENTITY_INFO_MAX_NAME_LEN	64

using namespace Ogre;

namespace Sgv
{
	//class ExtEntityInfo;

	////////////////////////////////////////////////////////////
	///
	///
	///
	///
	///
	///
	class X3D
	{
	public:
		// -------------------------------------
		// -------------------------------------
		X3D();

		// -------------------------------------
		// -------------------------------------
		virtual ~X3D();

		// -------------------------------------
		///
		// -------------------------------------
		bool init();

		// --------------------------------------------------------
		///
		///
		///
		// --------------------------------------------------------
		CX3DParser* loadX3DFromFile(const char *x3dFileName);

		// --------------------------------------------------------
		///
		// --------------------------------------------------------
		int countLoadedX3DFiles();

		// -----------------------------------------------------
		///
		///
		// -----------------------------------------------------
		const char *getLoadedX3DFileName(int i);

		// -----------------------------------------------------
		///
		///
		// -----------------------------------------------------
		bool isLoadedX3DFile(const char *x3dFileName);

		// -----------------------------------------------------
		///
		///
		///
		// -----------------------------------------------------
		bool setCurrentX3DFile(const char *x3dFileName);

		// -----------------------------------------------------
		///
		// -----------------------------------------------------
		const char *getCurrentX3DFile();

		// ------------------------------------------------------------
		///
		///
		// ------------------------------------------------------------
		bool isOpenHRPHumanoidShape();


		// *************************************************
		// *************************************************

		// -------------------------------------
		///
		///
		///
		// -------------------------------------
		SceneNode *createStaticNode(
			SceneManager *mgr,
			SceneNode *parentNode,
			const std::string &visObjElemName);

		// --------------------------------------
		///
		///
		// --------------------------------------
		bool generateSimplifiedShapeFromX3DParseResult(CSimplifiedShape::SHAPE_TYPE stype = CSimplifiedShape::NONE);

		// --------------------------------------
		///
		///
		// --------------------------------------
		void setSimplifiedShape(CSimplifiedShape *ss);

		// ----------------------------------------------
		///
		///
		///
		// ----------------------------------------------
		CSimplifiedShape *getSimplifiedShape();

		// --------------------------------------
		///
		///
		///
		///
		// --------------------------------------
		SceneNode *createSimplifiedShapeNode(
			SceneManager *mgr,
			SceneNode *parentNode);

		// ------------------------------------------------------------
		///
		///
		///
		// ------------------------------------------------------------
		bool calcShapeHizumis(float& hizumiSphere, float& hizumiCylinder, float& hizumiBox);


		// *************************************************
		// *************************************************

		// ===============================================
		// ===============================================
		// -------------------------------------------------------------
		///
		///
		///
		// -------------------------------------------------------------
		SceneNode *createOpenHRPNodes(
			SceneManager *mgr,
			SceneNode *parentNode,
			const std::string &visObjElemName,
			std::map<std::string, SceneNode *> &mapJointNameToSceneNode,
			std::map<std::string, SceneNode *> &mapPartsNameToSceneNode,
			bool bTreeMode,
			bool bUseX3DCenterOfMass);

		// -------------------------------------------------
		///
		///
		///
		// -------------------------------------------------
		bool createOpenHRPNodes_Tree(
			SceneManager *mgr,
			SceneNode *parentNode,
			const std::string &visObjElemName,
			std::map<std::string, SceneNode *> &mapJointNameToSceneNode,
			std::map<std::string, SceneNode *> &mapPartsNameToSceneNode,
			bool bUseX3DCenterOfMass);

		// -------------------------------------------------
		///
		///
		// -------------------------------------------------
		SceneNode * createOpenHRP_Joint_Tree(
			SceneManager *mgr,
			SceneNode *parentNode,
			const std::string &visObjElemName,
			CX3DOpenHRPJointNode *pJointNode,
			std::map<std::string, SceneNode *> &mapJointNameToSceneNode,
			std::map<std::string, SceneNode *> &mapPartsNameToSceneNode,
			bool bUseX3DCenterOfMass,
			int indent);

		// -------------------------------------------------
		///
		///
		// -------------------------------------------------
		SceneNode * createOpenHRP_Segment_Tree(
			SceneManager *mgr,
			SceneNode *parentNode,
			const std::string &visObjElemName,
			CX3DOpenHRPSegmentNode *pSegmentNode,
			std::map<std::string, SceneNode *> &mapPartsNameToSceneNode,
			bool bUseX3DCenterOfMass,
			int indent);

		// -------------------------------------------------
		///
		///
		// -------------------------------------------------
		SceneNode * createOpenHRP_Transform_Tree(
			SceneManager *mgr,
			SceneNode *parentNode,
			const std::string &visObjElemName,
			CX3DTransformNode *pTransNode,
			const std::string &parentName,
			std::map<std::string, SceneNode *> &mapPartsNameToSceneNode,
			Ogre::Vector3 *pCenterOfMass,
			int indent);

		// -------------------------------------------------
		///
		///
		// -------------------------------------------------
		SceneNode * createOpenHRP_Shape_Tree(
			SceneManager *mgr,
			SceneNode *parentNode,
			const std::string &visObjElemName,
			CX3DShapeNode *pShapeNode,
			const std::string &parentName,
			std::map<std::string, SceneNode *> &mapPartsNameToSceneNode,
			Ogre::Vector3 *pCenterOfMass,
			int indent);


		// ===============================================
		// ===============================================

		// -------------------------------------------------
		///
		///
		///
		// -------------------------------------------------
		bool createOpenHRPNodes_Flat(
			SceneManager *mgr,
			SceneNode *parentNode,
			const std::string &visObjElemName,
			std::map<std::string, SceneNode *> &mapPartsNameToSceneNode,
			bool bUseX3DCenterOfMass);

		// -------------------------------------------------
		///
		///
		// -------------------------------------------------
		SceneNode * createOpenHRP_Joint_Flat(
			SceneManager *mgr,
			SceneNode *rootParentNode,
			SceneNode *parentNode,
			const std::string &visObjElemName,
			CX3DOpenHRPJointNode *pJointNode,
			std::map<std::string, SceneNode *> &mapPartsNameToSceneNode,
			bool bUseX3DCenterOfMass,
			int indent);

		// -------------------------------------------------
		///
		///
		// -------------------------------------------------
		SceneNode * createOpenHRP_Segment_Flat(
			SceneManager *mgr,
			SceneNode *rootParentNode,
			SceneNode *parentNode,
			const std::string &visObjElemName,
			CX3DOpenHRPSegmentNode *pSegmentNode,
			std::map<std::string, SceneNode *> &mapPartsNameToSceneNode,
			bool bUseX3DCenterOfMass,
			int indent);

		// -------------------------------------------------
		///
		///
		// -------------------------------------------------
		SceneNode * createOpenHRP_Transform_Flat(
			SceneManager *mgr,
			SceneNode *rootParentNode,
			SceneNode *parentNode,
			const std::string &visObjElemName,
			CX3DTransformNode *pTransNode,
			const std::string &parentName,
			std::map<std::string, SceneNode *> &mapPartsNameToSceneNode,
			Ogre::Vector3 *pCenterOfMass,
			int indent);

		// -------------------------------------------------
		///
		///
		// -------------------------------------------------
		SceneNode * createOpenHRP_Shape_Flat(
			SceneManager *mgr,
			SceneNode *rootParentNode,
			SceneNode *parentNode,
			const std::string &visObjElemName,
			CX3DShapeNode *pShapeNode,
			const std::string &parentName,
			std::map<std::string, SceneNode *> &mapPartsNameToSceneNode,
			Ogre::Vector3 *pCenterOfMass,
			int indent);

		// *************************************************
		// *************************************************

		// --------------------------------------------------------------
		///
		// --------------------------------------------------------------
		void setTransAndRot(SceneNode *pSceneNode, SFVec3f *trans, SFRotation *rot);

		// --------------------------------------------------------------
		///
		///
		// --------------------------------------------------------------
		static void printAllChildrenOfNode(SceneNode *node, int level=0);

		// --------------------------------------------------------------
		///
		// --------------------------------------------------------------
		static void printExtEntityInfo(SceneNode *node, int level=0);

		// --------------------------------------------------------------
		///
		///
		// --------------------------------------------------------------
		static void deleteAllExtEntityInfo(SceneNode *node);

		// --------------------------------------------------------------
		///
		// --------------------------------------------------------------
		static void destroyAllManualObjects(SceneManager *mgr, SceneNode *node);

		// --------------------------------------------------------------
		// --------------------------------------------------------------
		//std::map<ManualObject*, Ogre::String> getAllManualObject(){ return m_allManualObject;}
		std::map<Entity*, Ogre::String> getAllManualObject(){ return m_allManualObject;}


	private:
		// -----------------------------------------------
		// -----------------------------------------------
		bool m_bInitOk;

		// -----------------------------------------------
		// -----------------------------------------------
		CX3DParser *m_pParser;

		// -----------------------------------------------
		// -----------------------------------------------
		std::string m_currentX3DFileName;

		// -----------------------------------------------
		// -----------------------------------------------
		std::vector<std::string> m_loadedX3DFiles;

		//added by okamoto@tome(2011/9/7)
		// -----------------------------------------------
		// -----------------------------------------------
		std::map<Entity*, Ogre::String> m_allManualObject;

		// -----------------------------------------------
		///
		///
		// -----------------------------------------------
		void addNewX3DFileNameToCache(const char *x3dFileName);

		// ----------------------------------------------------------
		// ----------------------------------------------------------
		std::map<std::string, bool> m_mapLoadedFileIsOpenHRPHumanoid;
		std::map<std::string, MFNode *> m_mapTransNodes;
		std::map<std::string, CSimplifiedShape *> m_mapSimplifiedShape;
		std::map<std::string, MFNode *> m_mapHumanoidNodes;

		// -----------------------------------------------
		// -----------------------------------------------
		void initCache(const char *x3dFileName);

		// -----------------------------------------------
		// -----------------------------------------------
		void freeAllCache();

		// ----------------------------------------------------------
		// ----------------------------------------------------------
		bool getValueFromCache_LoadedFileIsOpenHRPHumanoid(const char *x3dFileName);
		MFNode *getValueFromCache_TransNodes(const char *x3dFileName);
		CSimplifiedShape *getValueFromCache_SimplifiedShape(const char *x3dFileName);
		MFNode *getValueFromCache_HumanoidNodes(const char *x3dFileName);

		// ----------------------------------------------------------
		// ----------------------------------------------------------
		void setValueToCache_LoadedFileIsOpenHRPHumanoid(const char *x3dFileName, bool b);
		void setValueToCache_TransNodes(const char *x3dFileName, MFNode *p);
		void setValueToCache_SimplifiedShape(const char *x3dFileName, CSimplifiedShape *p);
		void setValueToCache_HumanoidNodes(const char *x3dFileName, MFNode *p);

		// -----------------------------------------------
		// -----------------------------------------------
		bool m_bLoadedFileIsOpenHRPHumanoid;

		// -----------------------------------------------
		// -----------------------------------------------
		MFNode *m_pTransNodes;

		// -----------------------------------------------
		// -----------------------------------------------
		CSimplifiedShape *m_pSimplifiedShape;

		// -----------------------------------------------
		// -----------------------------------------------
		MFNode *m_pHumanoidNodes;

		// ========================================
		// ========================================

		// ----------------------------------------------------
		///
		///
		// ----------------------------------------------------
		char *genEntityName();

		// --------------------------------------------------
		///
		///
		// --------------------------------------------------
		bool createNodeFromTrans(
			SceneManager *mgr,
			SceneNode *parentNode,
			const std::string &visObjElemName,
			MFNode *pTransNodes);

		// --------------------------------------------------
		///
		///
		// --------------------------------------------------
		bool createNodeFromShapes(
			SceneManager *mgr,
			SceneNode *parentNode,
			const std::string &visObjElemName,
			MFNode *pShapeNodes,
			Vector3& gShapes);

		// ------------------------------------------------------------------------
		///
		///
		///
		///
		///
		///
		///
		/// (case3) bSetPosRelToG=false ---------------> gout=(0, 0, 0)
		///
		// ------------------------------------------------------------------------
		ManualObject *createOgreManualObjectFromIndexedFaceSet(
			SceneManager *mgr,
			CX3DIndexedFaceSetNode *faceSet,
			//ExtEntityInfo *ei,
			char *materialName,
			bool bSetPosRelToG,
			Vector3 *gin,
			Vector3& gout,
			ColourValue *pColor = NULL);

	};


}

#endif

