
#include "SgvX3D.h"
#include "Ogre.h"
#include "CX3DParser.h"
#include "CX3DParserUtil.h"
#include "CX3DField.h"
#include "CX3DGroupNode.h"
#include "CX3DTransformNode.h"
#include "CX3DAppearanceNode.h"
#include "CX3DMaterialNode.h"
#include "CX3DIndexedFaceSetNode.h"
#include "CX3DImageTextureNode.h"
#include "CX3DCoordinateNode.h"
#include "CX3DNormalNode.h"
#include "CX3DTextureCoordinateNode.h"
#include "CSimplifiedShape.h"
#include "WinMain.h"
//#include "SgvMainWindow.h"
//#include "SgvLog.h"

namespace Sgv
{
	// ============================================================
	// ============================================================

	class TempVec2f {
	public:
		float x, y;
	};

	class TempVec3f {
	public:
		float x, y, z;
	};

	static int *allocTempIntArray(int num);
	static TempVec2f **allocTempVec2fArray(int num);
	static TempVec3f **allocTempVec3fArray(int num);

	static void freeTempIntArray(int *p);
	static void freeTempVec2fArray(TempVec2f **pp, int num);
	static void freeTempVec3fArray(TempVec3f **pp, int num);


	//////////////////////////////////////////////////////////////
	//
	//	X3D

	X3D::X3D()
	{
		CX3DParser::openDebugLog("SIGViewerParser.log");
		CX3DParser::setMaxPrintElemsForMFField(3);

		m_bInitOk = false;

		m_pParser = NULL;

		m_mapLoadedFileIsOpenHRPHumanoid.clear();
		m_mapTransNodes.clear();
		m_mapSimplifiedShape.clear();
		m_mapHumanoidNodes.clear();

		m_bLoadedFileIsOpenHRPHumanoid = false;
		m_pTransNodes = NULL;
		m_pSimplifiedShape = NULL;
		m_pHumanoidNodes = NULL;
	}

	X3D::~X3D()
	{
		if (m_pParser)
		{
			delete m_pParser;
			m_pParser = NULL;
		}

		freeAllCache();
	}

	bool X3D::init()
	{
		if (m_bInitOk) return true;		

		// begin(add)(2009/3/12)
		if (!CJNIUtil::init("X3DParser.cfg"))
		{
			CX3DParser::printLog("*** Failed to initialize Java VM ***");
			exit(1);
			//return false;
		}
		// end(add)

		m_pParser = new CX3DParser();

		if (m_pParser)
		{
			m_bInitOk = true;
			return true;
		}
		else
		{
			m_bInitOk = false;
			return false;
		}
	}

	CX3DParser* X3D::loadX3DFromFile(const char *x3dFileName)
	{
		if (!m_pParser->parse((char *)x3dFileName))
		{
			CX3DParser::printLog("failed to parse (%s)\n", x3dFileName);
			return false;
		}

		return m_pParser;
		//return true;
	}

	void X3D::addNewX3DFileNameToCache(const char *x3dFileName)
	{
		if (!isLoadedX3DFile(x3dFileName))
		{
			m_loadedX3DFiles.push_back(x3dFileName);
		}
	}

	int X3D::countLoadedX3DFiles()
	{
		return (int)(m_loadedX3DFiles.size());
	}

	const char *X3D::getLoadedX3DFileName(int i)
	{
		int n = m_loadedX3DFiles.size();

		if ((i<0) || (i>=n)) return NULL;

		return m_loadedX3DFiles[i].c_str();
	}

	bool X3D::isLoadedX3DFile(const char *x3dFileName)
	{
		if (!x3dFileName || !*x3dFileName) return false;

		std::vector<std::string>::iterator i;

		for (i=m_loadedX3DFiles.begin(); i!=m_loadedX3DFiles.end(); i++)
		{
			if (strcmp(x3dFileName, i->c_str())==0)
			{
				return true;
			}
		}

		return false;
	}

	bool X3D::setCurrentX3DFile(const char *x3dFileName)
	{
		if (!isLoadedX3DFile(x3dFileName)) return false;

		// ---------------------------------
		// ---------------------------------

		m_pTransNodes = getValueFromCache_TransNodes(x3dFileName);

		m_pHumanoidNodes = getValueFromCache_HumanoidNodes(x3dFileName);

		m_pSimplifiedShape = getValueFromCache_SimplifiedShape(x3dFileName);

		m_bLoadedFileIsOpenHRPHumanoid = getValueFromCache_LoadedFileIsOpenHRPHumanoid(x3dFileName);

		m_currentX3DFileName = x3dFileName;

		return true;
	}

	void X3D::initCache(const char *x3dFileName)
	{
		if (!x3dFileName || !*x3dFileName) return;

		setValueToCache_LoadedFileIsOpenHRPHumanoid(x3dFileName, false);

		setValueToCache_TransNodes(x3dFileName, NULL);

		setValueToCache_SimplifiedShape(x3dFileName, NULL);

		setValueToCache_HumanoidNodes(x3dFileName, NULL);
	}

	void X3D::freeAllCache()
	{
		m_mapLoadedFileIsOpenHRPHumanoid.clear();

		std::map<std::string, MFNode *>::iterator i1;
		for (i1=m_mapTransNodes.begin(); i1!=m_mapTransNodes.end(); i1++)
		{
			MFNode *p = i1->second;
			if (p) delete p;
		}
		m_mapTransNodes.clear();

		std::map<std::string, CSimplifiedShape *>::iterator i2;
		for (i2=m_mapSimplifiedShape.begin(); i2!=m_mapSimplifiedShape.end(); i2++)
		{
			CSimplifiedShape *p = i2->second;
			if (p) delete p;
		}
		m_mapSimplifiedShape.clear();

		std::map<std::string, MFNode *>::iterator i3;
		for (i3=m_mapHumanoidNodes.begin(); i3!=m_mapHumanoidNodes.end(); i3++)
		{
			MFNode *p = i3->second;
			if (p) delete p;
		}
		m_mapHumanoidNodes.clear();

		m_allManualObject.clear();

		//CJNIUtil::destroy();

	}

	bool X3D::getValueFromCache_LoadedFileIsOpenHRPHumanoid(const char *x3dFileName)
	{
		if (!x3dFileName || !*x3dFileName) return false;

		if (m_mapLoadedFileIsOpenHRPHumanoid.find(x3dFileName) != m_mapLoadedFileIsOpenHRPHumanoid.end())
		{
			return m_mapLoadedFileIsOpenHRPHumanoid[x3dFileName];
		}
		else
		{
			return false;
		}
	}

	MFNode *X3D::getValueFromCache_TransNodes(const char *x3dFileName)
	{
		if (!x3dFileName || !*x3dFileName) return false;

		if (m_mapTransNodes.find(x3dFileName) != m_mapTransNodes.end())
		{
			return m_mapTransNodes[x3dFileName];
		}
		else
		{
			return NULL;
		}
	}

	CSimplifiedShape *X3D::getValueFromCache_SimplifiedShape(const char *x3dFileName)
	{
		if (!x3dFileName || !*x3dFileName) return NULL;

		if (m_mapSimplifiedShape.find(x3dFileName) != m_mapSimplifiedShape.end())
		{
			return m_mapSimplifiedShape[x3dFileName];
		}
		else
		{
			return NULL;
		}
	}

	MFNode *X3D::getValueFromCache_HumanoidNodes(const char *x3dFileName)
	{
		if (!x3dFileName || !*x3dFileName) return NULL;

		if (m_mapHumanoidNodes.find(x3dFileName) != m_mapHumanoidNodes.end())
		{
			return m_mapHumanoidNodes[x3dFileName];
		}
		else
		{
			return NULL;
		}
	}

	void X3D::setValueToCache_LoadedFileIsOpenHRPHumanoid(const char *x3dFileName, bool b)
	{
		if (!x3dFileName || !*x3dFileName) return;

		m_mapLoadedFileIsOpenHRPHumanoid[x3dFileName] = b;
	}

	void X3D::setValueToCache_TransNodes(const char *x3dFileName, MFNode *p)
	{
		if (!x3dFileName || !*x3dFileName) return;

		if (m_mapTransNodes.find(x3dFileName) != m_mapTransNodes.end())
		{
			MFNode *p = m_mapTransNodes[x3dFileName];
			if (p) delete p;
		}

		m_mapTransNodes[x3dFileName] = p;
	}

	void X3D::setValueToCache_SimplifiedShape(const char *x3dFileName, CSimplifiedShape *p)
	{
		if (!x3dFileName || !*x3dFileName) return;

		if (m_mapSimplifiedShape.find(x3dFileName) != m_mapSimplifiedShape.end())
		{
			CSimplifiedShape *p = m_mapSimplifiedShape[x3dFileName];
			if (p) delete p;
		}

		m_mapSimplifiedShape[x3dFileName] = p;
	}

	void X3D::setValueToCache_HumanoidNodes(const char *x3dFileName, MFNode *p)
	{
		if (!x3dFileName || !*x3dFileName) return;

		if (m_mapHumanoidNodes.find(x3dFileName) != m_mapHumanoidNodes.end())
		{
			MFNode *p = m_mapHumanoidNodes[x3dFileName];
			if (p) delete p;
		}

		m_mapHumanoidNodes[x3dFileName] = p;
	}

	const char *X3D::getCurrentX3DFile()
	{
		return m_currentX3DFileName.c_str();
	}

	bool X3D::isOpenHRPHumanoidShape()
	{
		return m_bLoadedFileIsOpenHRPHumanoid;
	}

	char *X3D::genEntityName()
	{
		static char entName[256];
		static int nEntitys = 0;

		sprintf(entName, "entity%d", nEntitys++);
		return entName;
	}

	SceneNode *X3D::createStaticNode(SceneManager *mgr, SceneNode *parentNode, const std::string &visObjElemName)
	{
		SceneNode *node = parentNode->createChildSceneNode();

		node->setPosition(Vector3(0.0, 0.0, 0.0));
		node->setOrientation(Ogre::Quaternion(Ogre::Radian(0), Ogre::Vector3(1, 0, 0)));

		if (createNodeFromTrans(mgr, node, visObjElemName, m_pTransNodes))
		{
			return node;
		}
		else
		{
			return NULL;
		}
	}

	bool X3D::createNodeFromTrans(
		SceneManager *mgr,
		SceneNode *parentNode,
		const std::string &visObjElemName,
		MFNode *pTransNodes)
	{
		if (!pTransNodes) return false;

		int nTrans = pTransNodes->count();

		std::vector<SceneNode *> ogreTransNodes;
		std::vector<Vector3> translations;
		std::vector<Vector3> gShapesVec;

		for (int iTrans=0; iTrans<nTrans; iTrans++)
		{
			CX3DTransformNode *pTrans = (CX3DTransformNode *)(pTransNodes->getNode(iTrans));
			if (!pTrans) continue;

			MFNode *pShapeNodes = pTrans->searchNodesFromDirectChildren("Shape");
			if (pShapeNodes)
			{
				SceneNode *ogreTransNode = parentNode->createChildSceneNode();

				Vector3 gShapes;
				if (!createNodeFromShapes(mgr, ogreTransNode, visObjElemName, pShapeNodes, gShapes))
				{
					CX3DParser::printLog("failed to create shape node\n");
					delete pShapeNodes;	
					return false;
				}

				gShapesVec.push_back(gShapes);		
				ogreTransNodes.push_back(ogreTransNode);	

				SFVec3f *t = pTrans->getTranslation();
				Vector3 tt(t->x(), t->y(), t->z());
				translations.push_back(tt);		

				delete pShapeNodes;	
			}
		}

		Vector3 GG(0, 0, 0);
		for (int iTrans=0; iTrans<nTrans; iTrans++)
		{
			GG += gShapesVec[iTrans];
		}
		GG /= nTrans;

		for (int iTrans=0; iTrans<nTrans; iTrans++)
		{
			SceneNode *node = ogreTransNodes[iTrans];
			if (node)
			{
				Vector3 pos = translations[iTrans] + (gShapesVec[iTrans] - GG);
				node->setPosition(pos);
				node->setOrientation(Ogre::Quaternion(Ogre::Radian(0), Ogre::Vector3(1, 0, 0)));
			}
		}

		return true;
	}

	bool X3D::createNodeFromShapes(
		SceneManager *mgr,
		SceneNode *parentNode,
		const std::string &visObjElemName,
		MFNode *pShapeNodes,
		Vector3& gShapes)
	{
		std::string matName = "";
		bool bHasTex = false;

		//okamoto
		ColourValue polyColor(1.0, 1.0, 1.0, 1.0);
		ColourValue sColor(0.0, 0.0, 0.0);
		ColourValue emis(0.0, 0.0, 0.0);
		float shinn = 0.0;
		bool bPolyColorSpecified = false;
		//

		if (!pShapeNodes) return false;

		std::vector<SceneNode *> children;
		std::vector<Vector3> gs;

		int nShapes = pShapeNodes->count();
		for (int iShape=0; iShape<nShapes; iShape++)
		{
			CX3DShapeNode *pShape = (CX3DShapeNode *)(pShapeNodes->getNode(iShape));
			if (!pShape) continue;



			
			// ------------------------------------------
			// ------------------------------------------
			CX3DAppearanceNode *pAppearance = (CX3DAppearanceNode *)(pShape->getAppearance()->getNode());

			if (pAppearance)
			{
				CX3DImageTextureNode *pImageTex = (CX3DImageTextureNode *)(pAppearance->getTexture()->getNode());

					if (pImageTex)
					{
						MFString *texFiles = pImageTex->getUrl();

						if (texFiles->count() > 0)
						{
							const char *texFile = texFiles->getValue(0);

							// (ex)
							// "C:/PROJECTS/NII/TOY_YELLOW.JPG"
							// ---> "TOY_YELLOW"
							if (texFile)
							{
								matName = CX3DParserUtil::extractFileBaseName(texFile);
								bHasTex = true;

								CX3DParser::printLog("[matName] (%s)-->(%s)\n", texFile, matName.c_str());
								if(bHasTex) CX3DParser::printLog("bHasTex = true\n");
							}
						}
					}
					CX3DMaterialNode *pMatNode = (CX3DMaterialNode *)(pAppearance->getMaterial()->getNode());
					//okamoto 2011/1/4
					if (pMatNode)
					{
						SFColor *color = pMatNode->getDiffuseColor();
						polyColor.r = color->r();
						polyColor.g = color->g();
						polyColor.b = color->b();

						SFFloat *trans = pMatNode->getTransparency();
						if(trans->getValue() != 0.0)
						{
							polyColor.a = trans->getValue();
						}
						
						SFColor *scolor = pMatNode->getSpecularColor();
						sColor.r = scolor->r();
						sColor.g = scolor->g();
						sColor.b = scolor->b();

						SFFloat *shin = pMatNode->getShininess();
						shinn = shin->getValue();

						SFColor *ems = pMatNode->getEmissiveColor();
						emis.r = ems->r();
						emis.g = ems->g();
						emis.b = ems->b();

						bPolyColorSpecified = true;
					}
			   }

			CX3DNode *pG = pShape->getGeometry()->getNode();
			int type = pG->getNodeType();

			ManualObject *o; 
			Vector3 g;	     
			SceneNode *node = parentNode->createChildSceneNode(); 
			//ExtEntityInfo *ei = new ExtEntityInfo(visObjElemName.c_str(), "");
			//Sphere
			//type = INDEXED_FACE_SET_NODE;
			if(type == SPHERE_NODE || type == BOX_NODE || type == CYLINDER_NODE)
			{
				Entity *ent;
				char *objName = genEntityName();
				if(type == SPHERE_NODE)
				{

					ent = mgr->createEntity(objName,"konao/MySphere.mesh");
					CX3DSphereNode *sphere = (CX3DSphereNode*)pG;
					float rad = sphere->getRadius()->getValue();
					node->setScale(rad,rad,rad);
				}

				if(type == BOX_NODE)
				{
					ent = mgr->createEntity(objName,"konao/MyBox.mesh");
					CX3DBoxNode *sphere = (CX3DBoxNode*)pG;
					SFVec3f *size = sphere->getSize();
					node->setScale(size->x(),size->y(),size->z());
				}

				if(type == CYLINDER_NODE)
				{
					ent = mgr->createEntity(objName,"konao/MyCylinder.mesh");
					CX3DCylinderNode *sphere = (CX3DCylinderNode*)pG;
					float radius = sphere->getRadius()->getValue();
					float height = sphere->getHeight()->getValue();
					node->setScale(radius,radius,height);
				}
				Ogre::MaterialPtr tmp = Ogre::MaterialManager::getSingleton().getByName("MA_Magenta");
				Ogre::MaterialPtr mptr = tmp->clone(objName);
				mptr->setDiffuse(polyColor);
				mptr->setSpecular(sColor);
				mptr->setShininess(shinn);
				mptr->setSelfIllumination(emis);
				mptr->setLightingEnabled(true);
				if(polyColor.a != 1.0)
				{
					mptr->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
					mptr->setDepthCheckEnabled(false);
				}
				ent->setMaterialName(objName);
				//ent->setNormaliseNormals(true);
				m_allManualObject.insert(std::map<Ogre::Entity*, Ogre::String>::value_type(ent, objName));

				o = (ManualObject*)ent;
				//o->setUserObject(ei);


			}

			else if(type == INDEXED_FACE_SET_NODE)
			{
				// -------------------------------------------
				// -------------------------------------------
				CX3DIndexedFaceSetNode *pFaceSet = (CX3DIndexedFaceSetNode *)(pShape->getGeometry()->getNode());

				if (!pFaceSet)
				{
					CX3DParser::printLog("geometry node is NULL (IndexedFaceSet should be set)\n");
					return false;
				}

				//ExtEntityInfo *ei = new ExtEntityInfo(visObjElemName.c_str(), "");


				o = createOgreManualObjectFromIndexedFaceSet(
					mgr,
					pFaceSet,
					//ei,
					(char *)matName.c_str(),
					true,
					NULL,
					g,
					bPolyColorSpecified ? &polyColor : NULL);
				m_allManualObject.insert(std::map<Ogre::Entity*, Ogre::String>::value_type((Entity*)o,matName));
			}

			else
			{
				return false;
			}
			node->setPosition(Ogre::Vector3(0, 0, 0));
			node->setOrientation(Ogre::Quaternion(Ogre::Radian(0), Ogre::Vector3(1, 0, 0)));

			node->attachObject(o);

			children.push_back(node);
			gs.push_back(g);
		}

		Vector3 G(0, 0, 0);

		int i;
		int n = gs.size();
		for (i=0; i<n; i++)
		{
			CX3DParser::printLog("gs[%d]=(%f, %f, %f)\n", i, gs[i].x, gs[i].y, gs[i].z);
			G += gs[i];
		}
		G /= n;

		CX3DParser::printLog("G=(%f, %f, %f)\n", G.x, G.y, G.z);

		for (i=0; i<n; i++)
		{
			Vector3 gRel = gs[i] - G;
			SceneNode *child = children[i];
			child->setPosition(gRel);
			CX3DParser::printLog("%d shape node pos=(%f, %f, %f)\n", i, gRel.x, gRel.y, gRel.z);
		}

		gShapes = G;	

		return true;
	}

	ManualObject *X3D::createOgreManualObjectFromIndexedFaceSet(
		SceneManager *mgr,
		CX3DIndexedFaceSetNode *faceSet,
		//ExtEntityInfo *ei,
		char *materialName,
		bool bSetPosRelToG,
		Vector3 *gin,
		Vector3& gout,
		ColourValue *pColor)
	{
		int i;

		  if (!faceSet) return NULL;

		// ============================================
		// ============================================
		CX3DCoordinateNode *pCoord = (CX3DCoordinateNode *)(faceSet->getCoord()->getNode());
		if (!pCoord)
		{
			CX3DParser::printLog("** ERROR ** IndexedFaceSet has no coord.\n");
			return NULL;
		}
		MFVec3f *coord = pCoord->getPoint();

		CX3DNormalNode *pNormal = (CX3DNormalNode *)(faceSet->getNormal()->getNode());
		bool bHasNormal = (pNormal != NULL) ? true : false;
		MFVec3f *normal;
		if (bHasNormal) normal = pNormal->getVector();

		CX3DTextureCoordinateNode *pTexCoord = (CX3DTextureCoordinateNode *)(faceSet->getTexCoord()->getNode());
		bool bHasTex = (pTexCoord != NULL) ? true : false;
		MFVec2f *texCoord;
		if (bHasTex) texCoord = pTexCoord->getPoint();

		MFInt32 *coordIndex;
		MFInt32 *normalIndex;
		MFInt32 *texCoordIndex;

		coordIndex = faceSet->getCoordIndex();
		if (bHasNormal) normalIndex = faceSet->getNormalIndex();
		if (bHasTex) texCoordIndex = faceSet->getTexCoordIndex();

		// ============================================
		// ============================================

		// --------------------------------------------
		//
		// --------------------------------------------
		int nc = coordIndex->count();
		int nn = (bHasNormal) ? normalIndex->count() : 0;
		int nt = (bHasTex) ? texCoordIndex->count() : 0;

		if ((bHasNormal) && (nc != nn))
		{
			CX3DParser::printLog("** ERROR ** two index values must be same!! (nc=%d, nn=%d)\n", nc, nn);
			exit(1);
		}

		if ((bHasTex) && (nc != nt))
		{
			CX3DParser::printLog("** ERROR ** two index values must be same!! (nc=%d, nt=%d)\n", nc, nt);
			exit(1);
		}

		int num = nc;

#ifdef ENABLE_MANUAL_OBJECT_MAKING_LOG
		CX3DParser::printLog("num = %d\n", num);
#endif

		// ------------------------------------------------------
		//
		// ------------------------------------------------------

		int ncDelim = 0;
		int nnDelim = 0;
		int ntDelim = 0;
		for (i=0; i<num; i++)
		{
			int cind = coordIndex->getValue(i);
			int nind = (bHasNormal) ? normalIndex->getValue(i) : 0;
			int tind = (bHasTex) ? texCoordIndex->getValue(i) : 0;

			if (cind == -1) ncDelim++;
			if (bHasNormal && (nind == -1)) nnDelim++;
			if (bHasTex && (tind == -1)) ntDelim++;

			// check
			if (cind == -1)
			{
				if (bHasNormal && (nind != -1))
				{
					CX3DParser::printLog("** ERROR ** two values must be same!! [i=%d] (cind=%d, nind=%d)\n",
						i, cind, nind);
					exit(1);
				}

				if (bHasTex && (tind != -1))
				{
					CX3DParser::printLog("** ERROR ** two values must be same!! (i=%d) (cind=%d, tind=%d)\n",
						i, cind, tind);
					exit(1);
				}
			}
			else
			{
				if (bHasNormal && (nind == -1))
				{
					CX3DParser::printLog("** ERROR ** either of two values is -1!! [i=%d] (cind=%d, nind=%d)\n",
						i, cind, nind);
					exit(1);
				}

				if (bHasTex && (tind == -1))
				{
					CX3DParser::printLog("** ERROR ** either of two values is -1!! (i=%d) (cind=%d, tind=%d)\n",
						i, cind, tind);
					exit(1);
				}
			}
		}

		if (bHasNormal && (ncDelim != nnDelim))
		{
			CX3DParser::printLog("** ERROR ** two values must be same!! (ncDelim=%d, nnDelim=%d\n", ncDelim, nnDelim);
			exit(1);
		}

		if (bHasTex && (ncDelim != ntDelim))
		{
			CX3DParser::printLog("** ERROR ** two values must be same!! (ncDelim=%d, ntDelim=%d\n", ncDelim, ntDelim);
			exit(1);
		}

		int numPt = nc - ncDelim;

#ifdef ENABLE_MANUAL_OBJECT_MAKING_LOG
		CX3DParser::printLog("numPt = %d\n", numPt);
#endif
		// -----------------------------------------
		// -----------------------------------------
		TempVec3f **newCoord = allocTempVec3fArray(numPt);
		TempVec3f **newNormal = bHasNormal ? allocTempVec3fArray(numPt) : NULL;
		TempVec2f **newTexCoord = bHasTex ? allocTempVec2fArray(numPt) : NULL;

		int *newIndex = allocTempIntArray(num);

		// ---------------------------------------------------
		// ---------------------------------------------------
		int j=0;
		for (i=0; i<num; i++)
		{
			int cind = coordIndex->getValue(i);
			int nind = (bHasNormal) ? normalIndex->getValue(i) : 0;
			int tind = (bHasTex) ? texCoordIndex->getValue(i) : 0;

			if (cind != -1)
			{
				SFVec3f pos = coord->getValue(cind);

				newCoord[j]->x = pos.x();
				newCoord[j]->y = pos.y();
				newCoord[j]->z = pos.z();

				if (bHasNormal)
				{
					SFVec3f norm = normal->getValue(nind);

					newNormal[j]->x = norm.x();
					newNormal[j]->y = norm.y();
					newNormal[j]->z = norm.z();
				}

				if (bHasTex)
				{
					SFVec2f tc = texCoord->getValue(tind);

					newTexCoord[j]->x = tc.x();
					newTexCoord[j]->y = 1.0f - tc.y();
				}

				newIndex[i] = j;

				j++;
			}
			else
			{
				newIndex[i] = -1;
			}
		}

		if (j != numPt)
		{
			CX3DParser::printLog("something wrong!! j(%d) must be same as numPt(%d)\n", j, numPt);
			exit(1);
		}

		// ------------------------------------
		// ------------------------------------
		float gx, gy, gz;
		gx = gy = gz = 0;

		if (bSetPosRelToG)
		{
			if (!gin)
			{
				for (i=0; i<numPt; i++)
				{
					gx += newCoord[i]->x;
					gy += newCoord[i]->y;
					gz += newCoord[i]->z;
				}

				gx /= numPt;
				gy /= numPt;
				gz /= numPt;
			}
			else
			{
				gx = gin->x;
				gy = gin->y;
				gz = gin->z;
			}
		}

		gout = Vector3(gx, gy, gz);

		// =========================================
		// =========================================
		char *objName = genEntityName();


		ManualObject* o = mgr->createManualObject(objName);
		o->begin(materialName, RenderOperation::OT_TRIANGLE_LIST);

		// ------------------------------------
		// ------------------------------------
		for (i=0; i<numPt; i++)
		{
			float x, y, z;
			float nx, ny, nz;
			float u, v;

			if (bSetPosRelToG)
			{
				//
				//
				x = (newCoord[i]->x) - gx;
				y = (newCoord[i]->y) - gy;
				z = (newCoord[i]->z) - gz;
			}
			else
			{
				x = (newCoord[i]->x);
				y = (newCoord[i]->y);
				z = (newCoord[i]->z);
			}

			o->position(x, y, z);

			//okamoto 2011/1/5
			if (pColor)
			{
				if(!bHasTex)
				{
					o->colour(*pColor);
				}
			}

#ifdef ENABLE_MANUAL_OBJECT_MAKING_LOG
			CX3DParser::printLog("[%4d] pos(%f %f %f) ", i, x, y, z);
#endif
			if (bHasNormal)
			{
				nx = newNormal[i]->x;
				ny = newNormal[i]->y;
				nz = newNormal[i]->z;

				o->normal(nx, ny, nz);

#ifdef ENABLE_MANUAL_OBJECT_MAKING_LOG
				CX3DParser::printLog("normal(%f %f %f) ", nx, ny, nz);
#endif
			}

			if (bHasTex)
			{
				u = newTexCoord[i]->x;
				v = newTexCoord[i]->y;

				o->textureCoord(u, v);

#ifdef ENABLE_MANUAL_OBJECT_MAKING_LOG
				CX3DParser::printLog("texCoord(%f %f)", u, v);
#endif
			}

#ifdef ENABLE_MANUAL_OBJECT_MAKING_LOG
			CX3DParser::printLog("\n");
#endif
		}

		// ------------------------------------
		// ------------------------------------
		int nPoly = 0;
		int nKakukei = 0;
		int inds[MAX_POINTS_PER_ONE_POLYGON];
		for (i=0; i<num; i++)
		{
			int ind = newIndex[i];

			if (ind != -1)
			{
				if (nKakukei >= MAX_POINTS_PER_ONE_POLYGON)
				{
					CX3DParser::printLog("***  ERROR  ********************************\n");
					CX3DParser::printLog("Too big polygon. (nKakukei=%d)\n", nKakukei);
					CX3DParser::printLog("change value of MAX_POINTS_PER_ONE_POLYGON constant to avoid error.\n");
					CX3DParser::printLog("********************************************\n");

					exit(1);
				}
				inds[nKakukei++] = ind;
			}
			else
			{
				for (int j=1; j<=nKakukei-2; j++)
				{
					o->index(inds[0]);
					o->index(inds[j]);
					o->index(inds[j+1]);
				}

#ifdef ENABLE_MANUAL_OBJECT_MAKING_LOG
				CX3DParser::printLog("Polygon[%4d] : %d points\n", nPoly, nKakukei);
#endif
				nPoly++;

				nKakukei = 0;
			}
		}

		ManualObject::ManualObjectSection *sec = o->end();
		if (sec)
		{
			MaterialPtr mat = sec->getMaterial();

			//okamoto
			if(!bHasTex)
				mat->setLightingEnabled(false);	
			mat->setCullingMode(CULL_NONE);
		}

		// ------------------------------------
		// ------------------------------------
		//o->setUserObject(ei);

		// ------------------------------------
		// ------------------------------------
		//freeTempVec3fArray(newCoord, numPt);
		//if (bHasNormal) freeTempVec3fArray(newNormal, numPt);
		//if (bHasTex) freeTempVec2fArray(newTexCoord, numPt);
		//freeTempIntArray(newIndex);

#ifdef ENABLE_MANUAL_OBJECT_MAKING_LOG
		CX3DParser::printLog("*** make ManualObject succeeded ***\n");
#endif
		return o;
	}

	//////////////////////////////////////////////////
	//

	// =============================================
	// =============================================
	SceneNode *X3D::createOpenHRPNodes(
		SceneManager *mgr,
		SceneNode *parentNode,
		const std::string &visObjElemName,
		std::map<std::string, SceneNode *> &mapJointNameToSceneNode,
		std::map<std::string, SceneNode *> &mapPartsNameToSceneNode,
		bool bTreeMode,
		bool bUseX3DCenterOfMass)
	{
		//Sgv::Log::println("*** createOpenHRPNodes (%s) ***", visObjElemName.c_str());
		// begin(fix)(FIX20101027)
#if 1
		// new
		SceneNode *node = NULL;
		if (visObjElemName.length() != 0)
		{
			node = parentNode->createChildSceneNode(visObjElemName.c_str());
		}
		else
		{
			node = parentNode->createChildSceneNode();
		}
#else
		// old
		SceneNode *node = parentNode->createChildSceneNode(visObjElemName.c_str());
#endif
		// end(fix)

		if (bTreeMode)
		{
			if (!createOpenHRPNodes_Tree(mgr, node, visObjElemName, mapJointNameToSceneNode, mapPartsNameToSceneNode, bUseX3DCenterOfMass))
				goto error_return;
		}
		else
		{
			if (!createOpenHRPNodes_Flat(mgr, node, visObjElemName, mapPartsNameToSceneNode, bUseX3DCenterOfMass))
				goto error_return;
		}

		return node;

error_return:
		parentNode->removeChild(node);
		return NULL;
	}

	// =============================================
	// =============================================
	bool X3D::createOpenHRPNodes_Tree(
		SceneManager *mgr,
		SceneNode *parentNode,
		const std::string &visObjElemName,
		std::map<std::string, SceneNode *> &mapJointNameToSceneNode,
		std::map<std::string, SceneNode *> &mapPartsNameToSceneNode,
		bool bUseX3DCenterOfMass)
	{
		if (!m_bLoadedFileIsOpenHRPHumanoid)
		{
			//Sgv::Log::println("loaded x3d file does not contain OpenHRP shape.");
			return false;
		}

		if (!m_pHumanoidNodes)
		{
			//Sgv::Log::println("Humanoid node was not loaded.");
			return false;
		}

		mapJointNameToSceneNode.clear();
		mapPartsNameToSceneNode.clear();

		CX3DOpenHRPHumanoidNode *pHumanoid = (CX3DOpenHRPHumanoidNode *)(m_pHumanoidNodes->getNode(0));

		if (pHumanoid)
		{
			// -----------------------------------------
			// -----------------------------------------
			MFNode *humanoidBody = pHumanoid->getHumanoidBody();

			// -----------------------------------------
			// -----------------------------------------
			int nBody = humanoidBody->count();

			for (int i=0; i<nBody; i++)
			{
				// -----------------------------------------
				// -----------------------------------------
				CX3DNode *pNode = humanoidBody->getNode(i);
				if (pNode)
				{
					// -----------------------------------------
					// -----------------------------------------
					switch (pNode->getNodeType())
					{
					case OPENHRP_JOINT_NODE:
						{
							// -----------------------------------------
							// -----------------------------------------
							CX3DOpenHRPJointNode *pJointNode = (CX3DOpenHRPJointNode *)pNode;
							SceneNode *pSceneNode = createOpenHRP_Joint_Tree(
								mgr,
								parentNode,
								visObjElemName,
								pJointNode,
								mapJointNameToSceneNode,
								mapPartsNameToSceneNode,
								bUseX3DCenterOfMass,
								0);
						}
						break;
					}
				}
			}
		}

		return true;
	}

	SceneNode *X3D::createOpenHRP_Joint_Tree(
		SceneManager *mgr,
		SceneNode *parentNode,
		const std::string &visObjElemName,
		CX3DOpenHRPJointNode *pJointNode,
		std::map<std::string, SceneNode *> &mapJointNameToSceneNode,
		std::map<std::string, SceneNode *> &mapPartsNameToSceneNode,
		bool bUseX3DCenterOfMass,
		int indent)
	{
		// -----------------------------------------
		// -----------------------------------------
		SFString *name = pJointNode->getName();
		std::string jointName = name->getValue();

		// -----------------------------------------
		// -----------------------------------------
		SFVec3f *trans = pJointNode->getTranslation();

		// -----------------------------------------
		// -----------------------------------------
		SFRotation *rot = pJointNode->getRotation();

		// -----------------------------------------
		// -----------------------------------------
		SceneNode *pSceneNode = parentNode->createChildSceneNode();
		setTransAndRot(pSceneNode, trans, rot);

		// -------------------------------------------
		// -------------------------------------------
		mapJointNameToSceneNode[jointName] = pSceneNode;

		//Sgv::Log::printIndent(indent);
		//		Sgv::Log::println("JOINT (%s)", jointName.c_str());

		//Sgv::Log::printIndent(indent);
		//Sgv::Log::println("translation (%f %f %f)", trans->x(), trans->y(), trans->z());

		//Sgv::Log::printIndent(indent);
		//Sgv::Log::println("rotation    vec(%f %f %f) angle(%f)[rad]", rot->x(), rot->y(), rot->z(), rot->rot());

		// -----------------------------------------
		//
		// -----------------------------------------
		MFNode *children = pJointNode->getChildren();

		// -------------------------------------------
		// -------------------------------------------
		int n = children->count();

		for (int i=0; i<n; i++)
		{
			// -------------------------------------------
			// -------------------------------------------
			CX3DNode *pNode = children->getNode(i);

			// -------------------------------------------
			// -------------------------------------------
			switch (pNode->getNodeType())
			{
			case OPENHRP_JOINT_NODE:
				{
					// -------------------------------------------
					// -------------------------------------------
					CX3DOpenHRPJointNode *pJointNode = (CX3DOpenHRPJointNode *)pNode;
					createOpenHRP_Joint_Tree(
						mgr,
						pSceneNode,
						visObjElemName,
						pJointNode,
						mapJointNameToSceneNode,
						mapPartsNameToSceneNode,
						bUseX3DCenterOfMass,
						indent+1);
				}
				break;

			case OPENHRP_SEGMENT_NODE:
				{
					// -------------------------------------------
					// -------------------------------------------
					CX3DOpenHRPSegmentNode *pSegNode = (CX3DOpenHRPSegmentNode *)pNode;
					createOpenHRP_Segment_Tree(
						mgr,
						pSceneNode,
						visObjElemName,
						pSegNode,
						mapPartsNameToSceneNode,
						bUseX3DCenterOfMass,
						indent+1);
				}
				break;
			}
		}

		return pSceneNode;
	}

	SceneNode *X3D::createOpenHRP_Segment_Tree(
		SceneManager *mgr,
		SceneNode *parentNode,
		const std::string &visObjElemName,
		CX3DOpenHRPSegmentNode *pSegmentNode,
		std::map<std::string, SceneNode *> &mapPartsNameToSceneNode,
		bool bUseX3DCenterOfMass,
		int indent)
	{
		// -----------------------------------------
		// -----------------------------------------
		SFString *name = pSegmentNode->getName();
		std::string segmentName = name->getValue();

		//Sgv::Log::printIndent(indent);
		//Sgv::Log::println("SEGMENT (%s)", segmentName.c_str());

		// -----------------------------------------
		// -----------------------------------------
		Ogre::Vector3 centerOfMass(0, 0, 0);
		SFVec3f *pcm = pSegmentNode->getCenterOfMass();
		if (pcm)
		{
			centerOfMass.x = pcm->x();
			centerOfMass.y = pcm->y();
			centerOfMass.y = pcm->z();
		}

		// -----------------------------------------
		//
		// -----------------------------------------
		MFNode *children = pSegmentNode->getChildren();

		// -------------------------------------------
		// -------------------------------------------
		int n = children->count();

		for (int i=0; i<n; i++)
		{
			// -------------------------------------------
			// -------------------------------------------
			CX3DNode *pNode = children->getNode(i);

			// -------------------------------------------
			// -------------------------------------------
			switch (pNode->getNodeType())
			{
			case TRANSFORM_NODE:
				{
					// -------------------------------------------
					// -------------------------------------------
					CX3DTransformNode *pTransNode = (CX3DTransformNode *)pNode;
					createOpenHRP_Transform_Tree(
						mgr,
						parentNode,
						visObjElemName,
						pTransNode,
						segmentName,
						mapPartsNameToSceneNode,
						bUseX3DCenterOfMass ? &centerOfMass : NULL,
						indent+1);
				}
				break;
			}
		}

		return parentNode;
	}

	SceneNode *X3D::createOpenHRP_Transform_Tree(
		SceneManager *mgr,
		SceneNode *parentNode,
		const std::string &visObjElemName,
		CX3DTransformNode *pTransNode,
		const std::string &parentName,
		std::map<std::string, SceneNode *> &mapPartsNameToSceneNode,
		Ogre::Vector3 *pCenterOfMass,
		int indent)
	{
		// -----------------------------------------
		// -----------------------------------------
		SFVec3f *trans = pTransNode->getTranslation();

		// -----------------------------------------
		// -----------------------------------------
		SFRotation *rot = pTransNode->getRotation();

		// -----------------------------------------
		// -----------------------------------------
		SceneNode *pSceneNode = parentNode->createChildSceneNode();
		setTransAndRot(pSceneNode, trans, rot);

		//Sgv::Log::printIndent(indent);
		//Sgv::Log::println("TRANSFORM");

		//Sgv::Log::printIndent(indent);
		//Sgv::Log::println("translation (%f %f %f)", trans->x(), trans->y(), trans->z());

		//Sgv::Log::printIndent(indent);
		//Sgv::Log::println("rotation    vec(%f %f %f) angle(%f)[rad]", rot->x(), rot->y(), rot->z(), rot->rot());

		// -----------------------------------------
		//
		// -----------------------------------------
		MFNode *children = pTransNode->getChildren();

		// -------------------------------------------
		// -------------------------------------------
		int n = children->count();

		for (int i=0; i<n; i++)
		{
			// -------------------------------------------
			// -------------------------------------------
			CX3DNode *pNode = children->getNode(i);

			// -------------------------------------------
			// -------------------------------------------
			switch (pNode->getNodeType())
			{
			case TRANSFORM_NODE:
				{
					// -------------------------------------------
					// -------------------------------------------
					CX3DTransformNode *pTransNode = (CX3DTransformNode *)pNode;
					createOpenHRP_Transform_Tree(
						mgr,
						pSceneNode,
						visObjElemName,
						pTransNode,
						parentName,
						mapPartsNameToSceneNode,
						pCenterOfMass,
						indent+1);
				}
				break;

			case SHAPE_NODE:
				{
					// -------------------------------------------
					// -------------------------------------------
					CX3DShapeNode *pShapeNode = (CX3DShapeNode *)pNode;
					createOpenHRP_Shape_Tree(
						mgr,
						pSceneNode,
						visObjElemName,
						pShapeNode,
						parentName,
						mapPartsNameToSceneNode,
						pCenterOfMass,
						indent+1);
				}
				break;
			}
		}

		return pSceneNode;
	}

	SceneNode *X3D::createOpenHRP_Shape_Tree(
		SceneManager *mgr,
		SceneNode *parentNode,
		const std::string &visObjElemName,
		CX3DShapeNode *pShapeNode,
		const std::string &parentName,
		std::map<std::string, SceneNode *> &mapPartsNameToSceneNode,
		Ogre::Vector3 *pCenterOfMass,
		int indent)
	{
		bool bTextureSpecified = false;
		bool bPolyColorSpecified = false;
		std::string matName;

		// -----------------------------------------
		// -----------------------------------------
		SFNode *appearance = pShapeNode->getAppearance();

		// -----------------------------------------
		// -----------------------------------------
		SFNode *geometry = pShapeNode->getGeometry();

		//Sgv::Log::printIndent(indent);
		//Sgv::Log::println("SHAPE");

		// -----------------------------------------
		// -----------------------------------------
		SceneNode *node = NULL;
		ColourValue polyColor;

		if (appearance)
		{
			CX3DAppearanceNode *pAppearanceNode = (CX3DAppearanceNode *)(appearance->getNode());
			if (pAppearanceNode)
			{
				CX3DImageTextureNode *pImageTexNode = (CX3DImageTextureNode *)(pAppearanceNode->getTexture()->getNode());
				if (pImageTexNode)
				{
					MFString *texFiles = pImageTexNode->getUrl();

					if (texFiles->count() > 0)
					{
						const char *texFile = texFiles->getValue(0);

						// (ex)
						// "G:/PROJECTS/NII/DAGWORKS/CHECK/TOY_D/MAYA/TOY_YELLOW.JPG"
						// ---> "TOY_YELLOW"
						if (texFile)
						{
							matName = CX3DParserUtil::extractFileBaseName(texFile);
							bTextureSpecified = true;

//							Sgv::Log::println("[matName] (%s)-->(%s)\n", texFile, matName.c_str());
						}
					}
				}
				else
				{
					CX3DMaterialNode *pMatNode = (CX3DMaterialNode *)(pAppearanceNode->getMaterial()->getNode());
					if (pMatNode)
					{
						SFColor *color = pMatNode->getDiffuseColor();

						polyColor.r = color->r();
						polyColor.g = color->g();
						polyColor.b = color->b();

						bPolyColorSpecified = true;

						//Sgv::Log::printIndent(indent);
//						Sgv::Log::println("diffuseColor (%f %f %f)", color->r(), color->g(), color->b());
					}
				}
			}
		}

		if (geometry)
		{
			int type = geometry->getNode()->getNodeType();
			CX3DIndexedFaceSetNode *pIndexedFaceSetNode = (CX3DIndexedFaceSetNode *)(geometry->getNode());
			if (pIndexedFaceSetNode)
			{
				//Sgv::Log::printIndent(indent);
				//Sgv::Log::println("INDEXED_FACE_SET");

				std::string partsName = parentName;

				//ExtEntityInfo *ei = new ExtEntityInfo(visObjElemName.c_str(), partsName.c_str());
				ManualObject *o = NULL;
				Vector3 gout;

				if (bTextureSpecified)
				{
					o = createOgreManualObjectFromIndexedFaceSet(
						mgr,
						pIndexedFaceSetNode,
						//ei,
						(char *)matName.c_str(),
						true,
						pCenterOfMass,
						gout,
						NULL);
					m_allManualObject.insert(std::map<Ogre::Entity*, Ogre::String>::value_type((Entity*)o,matName));
				}
				else
				{
					o = createOgreManualObjectFromIndexedFaceSet(
						mgr,
						pIndexedFaceSetNode,
						//ei,
						"MA_White",
						true,
						pCenterOfMass,
						gout,
						bPolyColorSpecified ? &polyColor : NULL);
					m_allManualObject.insert(std::map<Ogre::Entity*, Ogre::String>::value_type((Entity*)o,"MA_White"));
				}
				parentNode->attachObject(o);
				//o->setMaterialName(0,"material_name");

				mapPartsNameToSceneNode[partsName] = parentNode;

#if 0
				// debug
				Vector3 pos = parentNode->getWorldPosition();
				Quaternion q = parentNode->getWorldOrientation();
				Sgv::Log::println("****** %s ******", parentName.c_str());
				Sgv::Log::println("(%f %f %f) (%f %f %f %f)",
					pos.x, pos.y, pos.z, q.w, q.x, q.y, q.z);
#endif
			}
		}

		return node;
	}

	// =============================================
	// =============================================
	bool X3D::createOpenHRPNodes_Flat(
		SceneManager *mgr,
		SceneNode *parentNode,
		const std::string &visObjElemName,
		std::map<std::string, SceneNode *> &mapPartsNameToSceneNode,
		bool bUseX3DCenterOfMass)
	{
		if (!m_bLoadedFileIsOpenHRPHumanoid)
		{
			//Sgv::Log::println("loaded x3d file does not contain OpenHRP shape.");
			return false;
		}

		if (!m_pHumanoidNodes)
		{
			//Sgv::Log::println("Humanoid node was not loaded.");
			return false;
		}

		mapPartsNameToSceneNode.clear();

		CX3DOpenHRPHumanoidNode *pHumanoid = (CX3DOpenHRPHumanoidNode *)(m_pHumanoidNodes->getNode(0));

		if (pHumanoid)
		{
			// -----------------------------------------
			// -----------------------------------------
			MFNode *humanoidBody = pHumanoid->getHumanoidBody();

			// -----------------------------------------
			// -----------------------------------------
			SceneNode *tempParentNode = parentNode->createChildSceneNode();

			// -----------------------------------------
			// -----------------------------------------
			int nBody = humanoidBody->count();

			for (int i=0; i<nBody; i++)
			{
				// -----------------------------------------
				// -----------------------------------------
				CX3DNode *pNode = humanoidBody->getNode(i);
				if (pNode)
				{
					// -----------------------------------------
					// -----------------------------------------
					switch (pNode->getNodeType())
					{
					case OPENHRP_JOINT_NODE:
						{
							// -----------------------------------------
							// -----------------------------------------
							CX3DOpenHRPJointNode *pJointNode = (CX3DOpenHRPJointNode *)pNode;
							SceneNode *pSceneNode = createOpenHRP_Joint_Flat(
								mgr,
								parentNode,
								tempParentNode,
								visObjElemName,
								pJointNode,
								mapPartsNameToSceneNode,
								bUseX3DCenterOfMass,
								0);
						}
						break;
					}
				}
			}

			if (tempParentNode)
			{
				tempParentNode->removeAndDestroyAllChildren();
				tempParentNode = NULL;
			}
		}

		return true;
	}

	SceneNode *X3D::createOpenHRP_Joint_Flat(
		SceneManager *mgr,
		SceneNode *rootParentNode,
		SceneNode *parentNode,
		const std::string &visObjElemName,
		CX3DOpenHRPJointNode *pJointNode,
		std::map<std::string, SceneNode *> &mapPartsNameToSceneNode,
		bool bUseX3DCenterOfMass,
		int indent)
	{
		// -----------------------------------------
		// -----------------------------------------
		SFString *name = pJointNode->getName();
		std::string jointName = name->getValue();

		// -----------------------------------------
		// -----------------------------------------
		SFVec3f *trans = pJointNode->getTranslation();

		// -----------------------------------------
		// -----------------------------------------
		SFRotation *rot = pJointNode->getRotation();

		// -----------------------------------------
		// -----------------------------------------
		SceneNode *pSceneNode = parentNode->createChildSceneNode();
		setTransAndRot(pSceneNode, trans, rot);

		//Sgv::Log::printIndent(indent);
		//Sgv::Log::println("JOINT (%s)", jointName.c_str());

		//Sgv::Log::printIndent(indent);
		//Sgv::Log::println("translation (%f %f %f)", trans->x(), trans->y(), trans->z());

		//Sgv::Log::printIndent(indent);
		//Sgv::Log::println("rotation    vec(%f %f %f) angle(%f)[rad]", rot->x(), rot->y(), rot->z(), rot->rot());

		// -----------------------------------------
		//
		// -----------------------------------------
		MFNode *children = pJointNode->getChildren();

		// -------------------------------------------
		// -------------------------------------------
		int n = children->count();

		for (int i=0; i<n; i++)
		{
			// -------------------------------------------
			// -------------------------------------------
			CX3DNode *pNode = children->getNode(i);

			// -------------------------------------------
			// -------------------------------------------
			switch (pNode->getNodeType())
			{
			case OPENHRP_JOINT_NODE:
				{
					// -------------------------------------------
					// -------------------------------------------
					CX3DOpenHRPJointNode *pJointNode = (CX3DOpenHRPJointNode *)pNode;
					createOpenHRP_Joint_Flat(
						mgr,
						rootParentNode,
						pSceneNode,
						visObjElemName,
						pJointNode,
						mapPartsNameToSceneNode,
						bUseX3DCenterOfMass,
						indent+1);
				}
				break;

			case OPENHRP_SEGMENT_NODE:
				{
					// -------------------------------------------
					// -------------------------------------------
					CX3DOpenHRPSegmentNode *pSegNode = (CX3DOpenHRPSegmentNode *)pNode;
					createOpenHRP_Segment_Flat(
						mgr,
						rootParentNode,
						pSceneNode,
						visObjElemName,
						pSegNode,
						mapPartsNameToSceneNode,
						bUseX3DCenterOfMass,
						indent+1);
				}
				break;
			}
		}

		return pSceneNode;
	}

	SceneNode *X3D::createOpenHRP_Segment_Flat(
		SceneManager *mgr,
		SceneNode *rootParentNode,
		SceneNode *parentNode,
		const std::string &visObjElemName,
		CX3DOpenHRPSegmentNode *pSegmentNode,
		std::map<std::string, SceneNode *> &mapPartsNameToSceneNode,
		bool bUseX3DCenterOfMass,
		int indent)
	{
		// -----------------------------------------
		// -----------------------------------------
		SFString *name = pSegmentNode->getName();
		std::string segmentName = name->getValue();

		//Sgv::Log::printIndent(indent);
		//Sgv::Log::println("SEGMENT (%s)", segmentName.c_str());

		// -----------------------------------------
		// -----------------------------------------
		Ogre::Vector3 centerOfMass(0, 0, 0);
		SFVec3f *pcm = pSegmentNode->getCenterOfMass();
		if (pcm)
		{
			centerOfMass.x = pcm->x();
			centerOfMass.y = pcm->y();
			centerOfMass.y = pcm->z();
		}

		// -----------------------------------------
		//
		// -----------------------------------------
		MFNode *children = pSegmentNode->getChildren();

		// -------------------------------------------
		// -------------------------------------------
		int n = children->count();

		for (int i=0; i<n; i++)
		{
			// -------------------------------------------
			// -------------------------------------------
			CX3DNode *pNode = children->getNode(i);

			// -------------------------------------------
			// -------------------------------------------
			switch (pNode->getNodeType())
			{
			case TRANSFORM_NODE:
				{
					// -------------------------------------------
					// -------------------------------------------
					CX3DTransformNode *pTransNode = (CX3DTransformNode *)pNode;
					createOpenHRP_Transform_Flat(
						mgr,
						rootParentNode,
						parentNode,
						visObjElemName,
						pTransNode,
						segmentName,
						mapPartsNameToSceneNode,
						bUseX3DCenterOfMass ? &centerOfMass : NULL,
						indent+1);
				}
				break;
			}
		}

		return parentNode;
	}

	SceneNode *X3D::createOpenHRP_Transform_Flat(
		SceneManager *mgr,
		SceneNode *rootParentNode,
		SceneNode *parentNode,
		const std::string &visObjElemName,
		CX3DTransformNode *pTransNode,
		const std::string &parentName,
		std::map<std::string, SceneNode *> &mapPartsNameToSceneNode,
		Ogre::Vector3 *pCenterOfMass,
		int indent)
	{
		// -----------------------------------------
		// -----------------------------------------
		SFVec3f *trans = pTransNode->getTranslation();

		// -----------------------------------------
		// -----------------------------------------
		SFRotation *rot = pTransNode->getRotation();

		// -----------------------------------------
		// -----------------------------------------
		SceneNode *pSceneNode = parentNode->createChildSceneNode();
		setTransAndRot(pSceneNode, trans, rot);

		//Sgv::Log::printIndent(indent);
		//Sgv::Log::println("TRANSFORM");

		//Sgv::Log::printIndent(indent);
		//Sgv::Log::println("translation (%f %f %f)", trans->x(), trans->y(), trans->z());

		//Sgv::Log::printIndent(indent);
		//Sgv::Log::println("rotation    vec(%f %f %f) angle(%f)[rad]", rot->x(), rot->y(), rot->z(), rot->rot());

		// -----------------------------------------
		//
		// -----------------------------------------
		MFNode *children = pTransNode->getChildren();

		// -------------------------------------------
		// -------------------------------------------
		int n = children->count();

		for (int i=0; i<n; i++)
		{
			// -------------------------------------------
			// -------------------------------------------
			CX3DNode *pNode = children->getNode(i);

			// -------------------------------------------
			// -------------------------------------------
			switch (pNode->getNodeType())
			{
			case TRANSFORM_NODE:
				{
					// -------------------------------------------
					// -------------------------------------------
					CX3DTransformNode *pTransNode = (CX3DTransformNode *)pNode;
					createOpenHRP_Transform_Flat(
						mgr,
						rootParentNode,
						pSceneNode,
						visObjElemName,
						pTransNode,
						parentName,
						mapPartsNameToSceneNode,
						pCenterOfMass,
						indent+1);
				}
				break;

			case SHAPE_NODE:
				{
					// -------------------------------------------
					// -------------------------------------------
					CX3DShapeNode *pShapeNode = (CX3DShapeNode *)pNode;
					createOpenHRP_Shape_Flat(
						mgr,
						rootParentNode,
						pSceneNode,
						visObjElemName,
						pShapeNode,
						parentName,
						mapPartsNameToSceneNode,
						pCenterOfMass,
						indent+1);
				}
				break;
			}
		}

		return pSceneNode;
	}

	SceneNode *X3D::createOpenHRP_Shape_Flat(
		SceneManager *mgr,
		SceneNode *rootParentNode,
		SceneNode *parentNode,
		const std::string &visObjElemName,
		CX3DShapeNode *pShapeNode,
		const std::string &parentName,
		std::map<std::string, SceneNode *> &mapPartsNameToSceneNode,
		Ogre::Vector3 *pCenterOfMass,
		int indent)
	{
		bool bTextureSpecified = false;
		bool bPolyColorSpecified = false;
		std::string matName;

		// -----------------------------------------
		// -----------------------------------------
		SFNode *appearance = pShapeNode->getAppearance();

		// -----------------------------------------
		// -----------------------------------------
		SFNode *geometry = pShapeNode->getGeometry();

		//Sgv::Log::printIndent(indent);
		//Sgv::Log::println("SHAPE");

		// -----------------------------------------
		// -----------------------------------------
		SceneNode *node = NULL;

		//okamoto
		ColourValue polyColor(1.0, 1.0, 1.0, 1.0);
		ColourValue sColor(0.0, 0.0, 0.0);
		ColourValue emis(0.0, 0.0, 0.0);
		float shinn = 0.0;

		if (appearance)
		{
			CX3DAppearanceNode *pAppearanceNode = (CX3DAppearanceNode *)(appearance->getNode());
			if (pAppearanceNode)
			{
				CX3DImageTextureNode *pImageTexNode = (CX3DImageTextureNode *)(pAppearanceNode->getTexture()->getNode());
				if (pImageTexNode)
				{
					MFString *texFiles = pImageTexNode->getUrl();

					if (texFiles->count() > 0)
					{
						const char *texFile = texFiles->getValue(0);

						// (ex)
						// "G:/PROJECTS/NII/DAGWORKS/CHECK/TOY_D/MAYA/TOY_YELLOW.JPG"
						// ---> "TOY_YELLOW"
						if (texFile)
						{
							matName = CX3DParserUtil::extractFileBaseName(texFile);
							bTextureSpecified = true;

//							Sgv::Log::println("[matName] (%s)-->(%s)\n", texFile, matName.c_str());
						}
					}
				}
				else
				{
					CX3DMaterialNode *pMatNode = (CX3DMaterialNode *)(pAppearanceNode->getMaterial()->getNode());
					if (pMatNode)
					{
						SFColor *color = pMatNode->getDiffuseColor();

						polyColor.r = color->r();
						polyColor.g = color->g();
						polyColor.b = color->b();

						bPolyColorSpecified = true;


						SFFloat *trans = pMatNode->getTransparency();
						if(trans->getValue() != 0.0)
						{
							polyColor.a = trans->getValue();
						}
						
						SFColor *scolor = pMatNode->getSpecularColor();
						sColor.r = scolor->r();
						sColor.g = scolor->g();
						sColor.b = scolor->b();

						SFFloat *shin = pMatNode->getShininess();
						shinn = shin->getValue();

						SFColor *ems = pMatNode->getEmissiveColor();
						emis.r = ems->r();
						emis.g = ems->g();
						emis.b = ems->b();


						//Sgv::Log::printIndent(indent);
//						Sgv::Log::println("diffuseColor (%f %f %f)", color->r(), color->g(), color->b());
					}
				}
			}
		}

		if (geometry)
		{
			int type = geometry->getNode()->getNodeType();
		    CX3DNode *pG = geometry->getNode();

			ManualObject *o = NULL; 
			Vector3 g;	     
			SceneNode *node = rootParentNode->createChildSceneNode(); 


			std::string partsName = parentName;
			//ExtEntityInfo *ei = new ExtEntityInfo(
			//visObjElemName.c_str(),
			//	partsName.c_str());

			node->resetToInitialState();

			if(type == SPHERE_NODE || type == BOX_NODE || type == CYLINDER_NODE)
			{
				Entity *ent;
				char *objName = genEntityName();
				if(type == SPHERE_NODE)
				{

					ent = mgr->createEntity(objName,"konao/MySphere.mesh");
					CX3DSphereNode *sphere = (CX3DSphereNode*)pG;
					float rad = sphere->getRadius()->getValue();
					node->setScale(rad,rad,rad);
				}

				if(type == BOX_NODE)
				{

					ent = mgr->createEntity(objName,"konao/MyBox.mesh");
					CX3DBoxNode *sphere = (CX3DBoxNode*)pG;
					SFVec3f *size = sphere->getSize();
					node->setScale(size->x(),size->y(),size->z());
				}

				if(type == CYLINDER_NODE)
				{
					ent = mgr->createEntity(objName,"konao/MyCylinder.mesh");
					CX3DCylinderNode *sphere = (CX3DCylinderNode*)pG;
					float radius = sphere->getRadius()->getValue();
					float height = sphere->getHeight()->getValue();
					node->setScale(radius,radius,height);
				}

				Ogre::MaterialPtr tmp = Ogre::MaterialManager::getSingleton().getByName("MA_Magenta");
				Ogre::MaterialPtr mptr = tmp->clone(objName);
				mptr->setDiffuse(polyColor);
				mptr->setSpecular(sColor);
				mptr->setShininess(shinn);
				mptr->setSelfIllumination(emis);
				mptr->setLightingEnabled(true);
				if(polyColor.a != 1.0)
				{
					mptr->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
					mptr->setDepthCheckEnabled(false);
				}

				ent->setMaterialName(objName);
				//ent->setNormaliseNormals(true);

				o = (ManualObject*)ent;
				m_allManualObject.insert(std::map<Ogre::Entity*, Ogre::String>::value_type(ent, objName));
				o->setVisible(true);
				//o->setUserObject(ei);
			}

			else if(type == INDEXED_FACE_SET_NODE)
			{
				CX3DIndexedFaceSetNode *pIndexedFaceSetNode = (CX3DIndexedFaceSetNode *)(geometry->getNode());
				if (pIndexedFaceSetNode)
				{
					//Sgv::Log::printIndent(indent);
					//Sgv::Log::println("INDEXED_FACE_SET");

					Vector3 gout;

					if (bTextureSpecified)
					{

						o = createOgreManualObjectFromIndexedFaceSet(
							mgr,
							pIndexedFaceSetNode,
							//ei,
							(char *)matName.c_str(),
							true,
							pCenterOfMass,
							gout,
							NULL);

						m_allManualObject.insert(std::map<Ogre::Entity*, Ogre::String>::value_type((Entity*)o, matName));
					}
					else
					{
						o = createOgreManualObjectFromIndexedFaceSet(
							mgr,
							pIndexedFaceSetNode,
							//ei,
							"MA_White",
							true,
							pCenterOfMass,
							gout,
							bPolyColorSpecified ? &polyColor : NULL);
						m_allManualObject.insert(std::map<Ogre::Entity*, Ogre::String>::value_type((Entity*)o, "MA_White"));
					}
				}
			 }

			 node->setInitialState();
			 //Ogre::Vector3 pos = parentNode->getWorldPosition();
			 //Ogre::Vector3 pos = parentNode->getwo;
			 //Ogre::Quaternion q = parentNode->getWorldOrientation();

			 //node->rotate(q, Ogre::Node::TransformSpace::TS_WORLD);
			 //node->translate(pos, Ogre::Node::TransformSpace::TS_WORLD);

			 node->attachObject(o);

			 mapPartsNameToSceneNode[partsName] = node;

#if 0
			 Sgv::Log::println("#### %s ####", parentName.c_str());

			 Sgv::Log::println("orig(parent) pos and q : (%f %f %f) (%f %f %f %f)",
				 pos.x, pos.y, pos.z, q.w, q.x, q.y, q.z);

			 Ogre::Vector3 pp = partsNode->getWorldPosition();
			 Ogre::Quaternion qq = partsNode->getWorldOrientation();
			 Sgv::Log::println("new(parts)   pos and q : (%f %f %f) (%f %f %f %f)",
				 pp.x, pp.y, pp.z, qq.w, qq.x, qq.y, qq.z);
#endif
			
		}

		return node;
	}

	void X3D::setTransAndRot(SceneNode *pSceneNode, SFVec3f *trans, SFRotation *rot)
	{
		Vector3 axis(rot->x(), rot->y(), rot->z());

		// ********************************************
		axis.normalise();
		// ********************************************
#if 1
		pSceneNode->translate(Vector3(trans->x(), trans->y(), trans->z()));
		pSceneNode->rotate(axis, Radian(rot->rot()));
#else
		pSceneNode->translate(Vector3(trans->x(), trans->y(), trans->z()), Ogre::Node::TransformSpace::TS_PARENT);
		pSceneNode->rotate(axis, Radian(rot->rot()), Ogre::Node::TransformSpace::TS_PARENT);
#endif
	}

	void X3D::printAllChildrenOfNode(SceneNode *node, int level)
	{
		if (!node) return;

		std::string name = node->getName();

		//Sgv::Log::printIndent(level);
		//Sgv::Log::println("%s", name.c_str());

		Ogre::Node::ChildNodeIterator it = node->getChildIterator();
		while (it.hasMoreElements())
		{
			SceneNode *child = (SceneNode *)(it.getNext());
			if (child)
			{
				printAllChildrenOfNode(child, level+1);
			}
		}
	}

	void X3D::printExtEntityInfo(SceneNode *node, int level)
	{
		if (!node) return;

		unsigned short nEntities = node->numAttachedObjects();
		for (unsigned short i=0; i<nEntities; i++)
		{
			MovableObject *ent = node->getAttachedObject(i);
			if (ent)
			{
				//ExtEntityInfo *ei = (ExtEntityInfo *)(ent->getUserObject());
//				if (ei)
//				{
//					Sgv::Log::printIndent(level);
//					Sgv::Log::println("visObjElemName(%s) partsName(%s)", ei->getVisObjElemName(), ei->getPartsName());
//				}
			}
		}

		Ogre::Node::ChildNodeIterator it = node->getChildIterator();
		while (it.hasMoreElements())
		{
			SceneNode *child = (SceneNode *)(it.getNext());
			if (child)
			{
				printExtEntityInfo(child, level+1);
			}
		}
	}

	void X3D::deleteAllExtEntityInfo(SceneNode *node)
	{
		if (!node) return;

		unsigned short nEntities = node->numAttachedObjects();
		for (unsigned short i=0; i<nEntities; i++)
		{
			MovableObject *ent = node->getAttachedObject(i);
			if (ent)
			{
				//ExtEntityInfo *ei = (ExtEntityInfo *)(ent->getUserObject());
//				if (ei)
//				{
//					delete ei;

//					ent->setUserObject(NULL);
//				}
			}
		}

		Ogre::Node::ChildNodeIterator it = node->getChildIterator();
		while (it.hasMoreElements())
		{
			SceneNode *child = (SceneNode *)(it.getNext());
			if (child)
			{
				deleteAllExtEntityInfo(child);
			}
		}
	}

	void X3D::destroyAllManualObjects(SceneManager *mgr, SceneNode *node)
	{
		if (!mgr || !node) return;

		unsigned short nEntities = node->numAttachedObjects();
		for (unsigned short i=0; i<nEntities; i++)
		{
			MovableObject *ent = node->getAttachedObject(i);
			if (ent)
			{
				mgr->destroyManualObject((ManualObject *)ent);
			}
		}

		Ogre::Node::ChildNodeIterator it = node->getChildIterator();
		while (it.hasMoreElements())
		{
			SceneNode *child = (SceneNode *)(it.getNext());
			if (child)
			{
				destroyAllManualObjects(mgr, child);
			}
		}
	}

	//////////////////////////////////////////////////
	//

	bool X3D::generateSimplifiedShapeFromX3DParseResult(CSimplifiedShape::SHAPE_TYPE stype)
	{
		CSimplifiedShape *ss = CSimplifiedShapeFactory::calcAutoFromTree(m_pTransNodes);
		if (!ss) false;

		setSimplifiedShape(ss);

		return true;
	}

	void X3D::setSimplifiedShape(CSimplifiedShape *ss)
	{
		const char *name = getCurrentX3DFile();

		setValueToCache_SimplifiedShape(name, ss);

		m_pSimplifiedShape = getValueFromCache_SimplifiedShape(name);
	}

	CSimplifiedShape *X3D::getSimplifiedShape()
	{
		return m_pSimplifiedShape;
	}

	SceneNode *X3D::createSimplifiedShapeNode(SceneManager *mgr, SceneNode *parentNode)
	{
		if (!m_pSimplifiedShape) return NULL;

		SceneNode *node = NULL;
		char *entName = genEntityName();

		switch (m_pSimplifiedShape->getType())
		{
		case CSimplifiedShape::SPHERE:
			{
				CSimplifiedSphere *sp = (CSimplifiedSphere *)m_pSimplifiedShape;

				float x = sp->x();
				float y = sp->y();
				float z = sp->z();
				float r = sp->radius();
				float hizumi = sp->hizumi();

				CX3DParser::printLog("**** Simplified Sphere Parameters ****\n");
				CX3DParser::printLog("center : (%f %f %f)\n", x, y, z);
				CX3DParser::printLog("r      : (%f)\n", r);
				CX3DParser::printLog("hizumi : (%f)\n", hizumi);

				Entity *sphere = mgr->createEntity(entName, "konao/MySphere.mesh");
				//sphere->setNormaliseNormals(true);

				//sphere->setQueryFlags(Sgv::MainWindow::NO_SELECTION_TARGET);

				node = parentNode->createChildSceneNode(entName);
				node->attachObject(sphere);
				//node->setPosition(Vector3(x, y, z));
				node->scale(Vector3(r));
			}
			break;

		case CSimplifiedShape::CYLINDER:
			{
				CSimplifiedCylinder *cy = (CSimplifiedCylinder *)m_pSimplifiedShape;

				float x = cy->x();
				float y = cy->y();
				float z = cy->z();
				float r = cy->radius();
				float h = cy->height();
				float hizumi = cy->hizumi();

				CX3DParser::printLog("**** Simplified Cylinder Parameters ****\n");
				CX3DParser::printLog("r : (%f)\n", r);
				CX3DParser::printLog("h : (%f)\n", h);
				CX3DParser::printLog("center of circle : (%f, %f, %f)\n", x, y, z);
				CX3DParser::printLog("yugami : (%f)\n", hizumi);

				Entity *cylinder = mgr->createEntity(entName, "konao/MyCylinder.mesh");
				//cylinder->setNormaliseNormals(true);
				//cylinder->setQueryFlags(Sgv::MainWindow::NO_SELECTION_TARGET);

				node = parentNode->createChildSceneNode(entName);
				node->attachObject(cylinder);
				//node->setPosition(x, y, z);
				node->scale(r, r, h/2);
				node->setDirection(0, 1, 0);	
			}
			break;

		case CSimplifiedShape::BOX:
			{
				CSimplifiedBox *bx = (CSimplifiedBox *)m_pSimplifiedShape;

				float x1 = bx->x1();
				float y1 = bx->y1();
				float z1 = bx->z1();
				float x2 = bx->x2();
				float y2 = bx->y2();
				float z2 = bx->z2();
				float hizumi = bx->hizumi();

				CX3DParser::printLog("**** Simplified Box Parameters ****\n");
				CX3DParser::printLog("(x1, y1, z1) : (%f %f %f)\n", x1, y1, z1);
				CX3DParser::printLog("(x2, y2, z2) : (%f %f %f)\n", x2, y2, z2);

				float cx = (x1 + x2) / 2;
				float cy = (y1 + y2) / 2;
				float cz = (z1 + z2) / 2;
				float sx = (float)fabs(x2 - x1);
				float sy = (float)fabs(y2 - y1);
				float sz = (float)fabs(z2 - z1);

				Entity *box = mgr->createEntity(entName, "konao/MyBox.mesh");
				//box->setNormaliseNormals(true);
				//box->setQueryFlags(Sgv::MainWindow::NO_SELECTION_TARGET);

				node = parentNode->createChildSceneNode(entName);
				node->attachObject(box);
				//node->setPosition(Vector3(cx, cy, cz));
				node->scale(Vector3(sx, sy, sz));
			}
			break;
		}

		return node;
	}

	bool X3D::calcShapeHizumis(float& hizumiSphere, float& hizumiCylinder, float& hizumiBox)
	{
		hizumiSphere = 0.0f;
		hizumiCylinder = 0.0f;
		hizumiBox = 0.0f;

		// ------------------------------------
		// ------------------------------------
		CSimplifiedSphere *sp = CSimplifiedShapeFactory::calcSphereFromTree(m_pTransNodes);
		if (!sp) return false;
		hizumiSphere = sp->hizumi();
		delete sp;
		sp = NULL;

		CSimplifiedCylinder *cy = CSimplifiedShapeFactory::calcCylinderFromTree(m_pTransNodes);
		if (!cy) return false;
		hizumiCylinder = cy->hizumi();
		delete cy;
		cy = NULL;

		CSimplifiedBox *bx = CSimplifiedShapeFactory::calcBoxFromTree(m_pTransNodes);
		if (!bx) return false;
		hizumiBox = bx->hizumi();
		delete bx;
		bx = NULL;

		return true;
	}

	//////////////////////////////////////////////////
	//

	int *allocTempIntArray(int num)
	{
		int *p = (int *)malloc(sizeof(int) * num);

		return p;
	}

	TempVec2f **allocTempVec2fArray(int num)
	{
		TempVec2f **pp = (TempVec2f **)malloc(sizeof(TempVec2f *) * num);
		if (!pp) return NULL;

		for (int i=0; i<num; i++)
		{
			TempVec2f *p = (TempVec2f *)malloc(sizeof(TempVec2f));
			if (!p)
			{
				free(pp);
				return NULL;
			}

			p->x = 0.0f;
			p->y = 0.0f;

			pp[i] = p;
		}

		return pp;
	}

	TempVec3f **allocTempVec3fArray(int num)
	{
		TempVec3f **pp = (TempVec3f **)malloc(sizeof(TempVec3f *) * num);
		if (!pp) return NULL;

		for (int i=0; i<num; i++)
		{
			TempVec3f *p = (TempVec3f *)malloc(sizeof(TempVec3f));
			if (!p)
			{
				free(pp);
				return NULL;
			}

			p->x = 0.0f;
			p->y = 0.0f;
			p->z = 0.0f;

			pp[i] = p;
		}

		return pp;
	}

	void freeTempIntArray(int *p)
	{
		if (p) free(p);
	}

	void freeTempVec2fArray(TempVec2f **pp, int num)
	{
		for (int i=0; i<num; i++)
		{
			TempVec2f *p = pp[i];
			if (p) free(p);
		}
		free(pp);
	}

	void freeTempVec3fArray(TempVec3f **pp, int num)
	{
		for (int i=0; i<num; i++)
		{
			TempVec3f *p = pp[i];
			if (p) free(p);
		}
		free(pp);
	}
}

