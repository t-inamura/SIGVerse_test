#include "CVRMLFieldData.h"
#include "CX3DField.h"
#include "CX3DNode.h"
#include "CX3DMaterialNode.h"
#include "CX3DTransformNode.h"
#include "CX3DGroupNode.h"
#include "CX3DShapeNode.h"
#include "CX3DAppearanceNode.h"
#include "CX3DBoxNode.h"
#include "CX3DConeNode.h"
#include "CX3DCylinderNode.h"
#include "CX3DSphereNode.h"
#include "CX3DColorNode.h"
#include "CX3DCoordinateNode.h"
#include "CX3DNormalNode.h"
#include "CX3DIndexedLineSetNode.h"
#include "CX3DIndexedFaceSetNode.h"
#include "CX3DDirectionalLightNode.h"
#include "CX3DPointLightNode.h"
#include "CX3DSpotLightNode.h"
#include "CX3DImageTextureNode.h"
#include "CX3DTextureTransformNode.h"
#include "CX3DTextureCoordinateNode.h"
#include "CX3DViewpointNode.h"
#include "CX3DOpenHRPHumanoidNode.h"
#include "CX3DOpenHRPJointNode.h"
#include "CX3DOpenHRPSegmentNode.h"
#include "CX3DHAnimDisplacerNode.h"
#include "CX3DHAnimHumanoidNode.h"
#include "CX3DHAnimJointNode.h"
#include "CX3DHAnimSegmentNode.h"
#include "CX3DHAnimSiteNode.h"
#include "CX3DParserUtil.h"
#include "CJNIUtil.h"
#include "CX3DParser.h"
#include <jni.h>

char *CX3DNode::getNodeTypeString(CX3DNodeType ntype)
{
	switch (ntype)
	{
		case BASE_NODE:
			return "BASE_NODE";
			
		case MATERIAL_NODE:
			return "MATERIAL_NODE";
			
		case TRANSFORM_NODE:
			return "TRANSFORM_NODE";
			
		case GROUP_NODE:
			return "GROUP_NODE";
			
		case APPEARANCE_NODE:
			return "APPEARANCE_NODE";
			
		case SCENE_NODE:
			return "SCENE_NODE";
			
		case SHAPE_NODE:
			return "SHAPE_NODE";
			
		case BOX_NODE:
			return "BOX_NODE";
			
		case CONE_NODE:
			return "CONE_NODE";
			
		case CYLINDER_NODE:
			return "CYLINDER_NODE";
			
		case SPHERE_NODE:
			return "SPHERE_NODE";
			
		case COLOR_NODE:
			return "COLOR_NODE";

		case COORDINATE_NODE:
			return "COORDINATE_NODE";

		case NORMAL_NODE:
			return "NORMAL_NODE";

		case INDEXED_LINE_SET_NODE:
			return "INDEXED_LINE_SET_NODE";
			
		case INDEXED_FACE_SET_NODE:
			return "INDEXED_FACE_SET_NODE";
			
		case DIRECTIONAL_LIGHT_NODE:
			return "DIRECTIONAL_LIGHT_NODE";

		case POINT_LIGHT_NODE:
			return "POINT_LIGHT_NODE";

		case SPOT_LIGHT_NODE:
			return "SPOT_LIGHT_NODE";

		case TEXTURE_TRANSFORM_NODE:
			return "TEXTURE_TRANSFORM_NODE";
			
		case TEXTURE_COORDINATE_NODE:
			return "TEXTURE_COORDINATE_NODE";
			
		case IMAGE_TEXTURE_NODE:
			return "IMAGE_TEXTURE_NODE";
			
		case VIEWPOINT_NODE:
			return "VIEWPOINT_NODE";

		case OPENHRP_HUMANOID_NODE:
			return "OPENHRP_HUMANOID_NODE";

		case OPENHRP_JOINT_NODE:
			return "OPENHRP_JOINT_NODE";

		case OPENHRP_SEGMENT_NODE:
			return "OPENHRP_SEGMENT_NODE";

		case HANIM_DISPLACER_NODE:
			return "HANIM_DISPLACER_NODE";

		case HANIM_HUMANOID_NODE:
			return "HANIM_HUMANOID_NODE";

		case HANIM_JOINT_NODE:
			return "HANIM_JOINT_NODE";

		case HANIM_SEGMENT_NODE:
			return "HANIM_SEGMENT_NODE";

		case HANIM_SITE_NODE:
			return "HANIM_SITE_NODE";

		default:
			return "** UNKNOWN_NODE **";
	}
}

CX3DNode::CX3DNode()
{
	m_vrmlNode = NULL;
}

CX3DNode::CX3DNode(jobject vrmlNode)
{
	CJNIUtil *ju = CJNIUtil::getUtil();
	JNIEnv *env = ju->env();

	if (vrmlNode)
	{
		if (!ju->isInstanceOfVRMLNode(vrmlNode))
		{
			fprintf(stderr, "vrmlNode is not instance of VRMLNode [%s:%d]\n", __FILE__, __LINE__);
			exit(1);
		}
		
		m_vrmlNode = env->NewGlobalRef(vrmlNode);
	}
	else
	{
//		fprintf(stderr, "vrmlNode is NULL [%s:%d]\n", __FILE__, __LINE__);
		m_vrmlNode = NULL;
	}
}

CX3DNode::~CX3DNode()
{
	JNIEnv *env = CJNIUtil::getEnv();

	if (m_vrmlNode)
	{
		env->DeleteGlobalRef(m_vrmlNode);
		m_vrmlNode = NULL;
	}
}

char *CX3DNode::getNodeName()
{
	CJNIUtil *ju = CJNIUtil::getUtil();

	return ju->VRMLNode_getNodeName(m_vrmlNode);
}

int CX3DNode::countFields()
{
	CJNIUtil *ju = CJNIUtil::getUtil();

	return ju->VRMLNode_getNumFields(m_vrmlNode);
}

char *CX3DNode::getFieldName(int iField)
{
	CJNIUtil *ju = CJNIUtil::getUtil();

	if (!m_vrmlNode) return NULL;

	jobject decl = ju->VRMLNode_getFieldDeclaration(m_vrmlNode, iField);

	return decl ? ju->VRMLFieldDeclaration_getName(decl) : NULL;
}

int CX3DNode::getFieldType(int iField)
{
	CJNIUtil *ju = CJNIUtil::getUtil();

	if (!m_vrmlNode) return -1;

	jobject decl = ju->VRMLNode_getFieldDeclaration(m_vrmlNode, iField);

	return decl ? ju->VRMLFieldDeclaration_getFieldType(decl) : -1;
}

char *CX3DNode::getFieldTypeString(int iField)
{
	CJNIUtil *ju = CJNIUtil::getUtil();

	if (!m_vrmlNode) return NULL;

	jobject decl = ju->VRMLNode_getFieldDeclaration(m_vrmlNode, iField);

	return decl ? ju->VRMLFieldDeclaration_getFieldTypeString(decl) : NULL;
}

int CX3DNode::getFieldIndex(char *fieldName)
{
	CJNIUtil *ju = CJNIUtil::getUtil();

	if (!m_vrmlNode) return -1;

	return ju->VRMLNode_getFieldIndex(m_vrmlNode, fieldName);
}

CVRMLFieldData *CX3DNode::createFieldValue(int iField)
{
	CVRMLFieldData *data = NULL;

	CJNIUtil *ju = CJNIUtil::getUtil();

	if (!m_vrmlNode) return data;

	jobject vrmlFieldData = ju->VRMLNodeType_getFieldValue(m_vrmlNode, iField);
	if (vrmlFieldData)
	{
		CVRMLFieldDataFactory *factory = CVRMLFieldDataFactory::getFactory();

		data = factory->createField(vrmlFieldData);
	}

	return data;
}

CVRMLFieldData *CX3DNode::createFieldValue(char *fieldName)
{
	CJNIUtil *ju = CJNIUtil::getUtil();

	if (!m_vrmlNode) return NULL;

	int iField = getFieldIndex(fieldName);

	return (iField >= 0) ? createFieldValue(iField) : NULL;
}

//#define DEBUG_SEARCH_NODES

MFNode *CX3DNode::searchNodesFromDirectChildren(char *nodeName)
{
	MFNode *ret = new MFNode();
	if (!ret)
	{
		CX3DParser::printLog("out of memory [%s:%d]\n", __FILE__, __LINE__);
		return NULL;
	}

#ifdef DEBUG_SEARCH_NODES
	CX3DParser::printLog("*** searching for (%s) childNode in (%s) node ***\n", nodeName, getNodeName());
#endif

	int n = countFields();

	for (int i=0; i<n; i++)
	{
#ifdef DEBUG_SEARCH_NODES
		CX3DParser::printLog("[%d] ", i);
#endif

		char *fieldName = getFieldName(i);

		if (fieldName)
		{
#ifdef DEBUG_SEARCH_NODES
			CX3DParser::printLog("fieldName=(%s) ", fieldName);
#endif

			CVRMLFieldData *data = createFieldValue(fieldName);
			if (data)
			{
				if (data->isNodeField())
				{
#ifdef DEBUG_SEARCH_NODES
					CX3DParser::printLog("is node. ");
#endif
					jobject vrmlNode = ((CVRMLNodeData *)data)->getValue();
					CX3DNode *node = CX3DNodeFactory::createNode(vrmlNode);
					if (node)
					{
#ifdef DEBUG_SEARCH_NODES
						CX3DParser::printLog("nodeName=(%s) ", node->getNodeName());
#endif
						if (strcmp(node->getNodeName(), nodeName) == 0)
						{
#ifdef DEBUG_SEARCH_NODES
							CX3DParser::printLog("*** found! ***");
#endif
							ret->addValue(node);
						}
						else
						{
							delete node;
						}
					}
					else
					{
#ifdef DEBUG_SEARCH_NODES
						CX3DParser::printLog("(NULL)");
#endif
					}
				}
				else if (data->isNodeArrayField())
				{
#ifdef DEBUG_SEARCH_NODES
					CX3DParser::printLog("is node array.\n");
#endif
					CVRMLNodeArrayData *nodeArray = (CVRMLNodeArrayData *)data;

					for (int j=0; j<nodeArray->count(); j++)
					{
#ifdef DEBUG_SEARCH_NODES
						CX3DParser::printLog("  (%d) ", j);
#endif
						jobject vrmlNode = nodeArray->getValue(j);
						CX3DNode *child = CX3DNodeFactory::createNode(vrmlNode);
						if (child)
						{
#ifdef DEBUG_SEARCH_NODES
							CX3DParser::printLog("nodeName=(%s) ", child->getNodeName());
#endif
							if (strcmp(child->getNodeName(), nodeName) == 0)
							{
#ifdef DEBUG_SEARCH_NODES
								CX3DParser::printLog("*** found! ***");
#endif
								ret->addValue(child);
							}
							else
							{
								delete child;
							}
						}
						else
						{
#ifdef DEBUG_SEARCH_NODES
							CX3DParser::printLog("(NULL)");
#endif
						}

#ifdef DEBUG_SEARCH_NODES
						CX3DParser::printLog("\n");
#endif
					}
				}

				delete data;
			}
		}
#ifdef DEBUG_SEARCH_NODES
			CX3DParser::printLog("\n");
#endif
	}

	return ret;
}

MFNode *CX3DNode::searchNodesFromAllChildren(char *nodeName)
{
	MFNode *ret = new MFNode();
	if (!ret)
	{
		CX3DParser::printLog("out of memory [%s:%d]\n", __FILE__, __LINE__);
		return NULL;
	}

#ifdef DEBUG_SEARCH_NODES
	static int indent = 0;
	CX3DParser::printIndentLog(indent, "*** searching for all (%s) childNode in (%s) node ***\n", nodeName, getNodeName());
#endif

	int n = countFields();

	for (int i=0; i<n; i++)
	{
#ifdef DEBUG_SEARCH_NODES
		CX3DParser::printIndentLog(indent, "[%d] ", i);
#endif

		char *fieldName = getFieldName(i);

		if (fieldName)
		{
#ifdef DEBUG_SEARCH_NODES
			CX3DParser::printLog("fieldName=(%s) ", fieldName);
#endif

			CVRMLFieldData *data = createFieldValue(fieldName);
			if (data)
			{
				if (data->isNodeField())
				{
#ifdef DEBUG_SEARCH_NODES
					CX3DParser::printLog("is node. ");
#endif
					jobject vrmlNode = ((CVRMLNodeData *)data)->getValue();
					CX3DNode *node = CX3DNodeFactory::createNode(vrmlNode);
					if (node)
					{
						bool bDeleteNode;

#ifdef DEBUG_SEARCH_NODES
						CX3DParser::printLog("nodeName=(%s) ", node->getNodeName());
#endif
						if (strcmp(node->getNodeName(), nodeName) == 0)
						{
#ifdef DEBUG_SEARCH_NODES
							CX3DParser::printLog("*** found! ***");
#endif
							ret->addValue(node);
							bDeleteNode = false;
						}
						else 
						{
							bDeleteNode = true;
						}

#ifdef DEBUG_SEARCH_NODES
							CX3DParser::printLog("\n");
							indent++;
#endif
						MFNode *retNodes = node->searchNodesFromAllChildren(nodeName);

#ifdef DEBUG_SEARCH_NODES
						indent--;
#endif
						if (retNodes)
						{
							for (int j=0; j<retNodes->count(); j++)
							{
								CX3DNode *retNode = retNodes->releaseNode(j);
								if (retNode)
								{
									ret->addValue(retNode);
								}
							}

							delete retNodes;
							retNodes = NULL;
						}
						
						if (bDeleteNode)
						{
							delete node;
							node = NULL;
						}
					}
					else
					{
#ifdef DEBUG_SEARCH_NODES
						CX3DParser::printLog("(NULL)");
#endif
					}
				}
				else if (data->isNodeArrayField())
				{
#ifdef DEBUG_SEARCH_NODES
					CX3DParser::printLog("is node array.\n");
#endif
					CVRMLNodeArrayData *nodeArray = (CVRMLNodeArrayData *)data;

					for (int j=0; j<nodeArray->count(); j++)
					{
#ifdef DEBUG_SEARCH_NODES
						CX3DParser::printIndentLog(indent, "  (%d) ", j);
#endif
						jobject vrmlNode = nodeArray->getValue(j);
						CX3DNode *child = CX3DNodeFactory::createNode(vrmlNode);
						if (child)
						{
							bool bDeleteNode;
#ifdef DEBUG_SEARCH_NODES
							CX3DParser::printLog("nodeName=(%s) ", child->getNodeName());
#endif

							if (strcmp(child->getNodeName(), nodeName) == 0)
							{
#ifdef DEBUG_SEARCH_NODES
								CX3DParser::printLog("*** found! ***");
#endif
								ret->addValue(child);
								bDeleteNode = false;
							}
							else
							{
								bDeleteNode = true;
							}

#ifdef DEBUG_SEARCH_NODES
							CX3DParser::printLog("\n");
							indent++;
#endif
							MFNode *retNodes = child->searchNodesFromAllChildren(nodeName);

#ifdef DEBUG_SEARCH_NODES
							indent--;
#endif
							if (retNodes)
							{
								for (int k=0; k<retNodes->count(); k++)
								{
									CX3DNode *retNode = retNodes->releaseNode(k);
									if (retNode)
									{
										ret->addValue(retNode);
									}
								}

								delete retNodes;
								retNodes = NULL;
							}

							if (bDeleteNode)
							{
								delete child;
								child = NULL;
							}
						}
						else
						{
#ifdef DEBUG_SEARCH_NODES
							CX3DParser::printLog("(NULL)");
#endif
						}

#ifdef DEBUG_SEARCH_NODES
						CX3DParser::printLog("\n");
#endif
					}
				}

				delete data;
				data = NULL;
			}
		}
#ifdef DEBUG_SEARCH_NODES
		CX3DParser::printLog("\n");
#endif
	}

	return ret;
}

void CX3DNode::print(int indent)
{
	FILE *fp = CX3DParser::getDebugLogFp();

	char *nodeName = getNodeName();
	if (nodeName)
	{
		CX3DParser::printIndent(indent);
		fprintf(fp, "%s (%s)\n", nodeName, CX3DNode::getNodeTypeString(getNodeType()));
		
		int n = countFields();

		for (int i=0; i<n; i++)
		{
			char *fieldName = getFieldName(i);

			if (fieldName)
			{
				if (strcmp(fieldName, "metadata") != 0)
				{
					CX3DParser::printIndent(indent+1);
					fprintf(fp, "%s\n", fieldName);

					CVRMLFieldData *data = createFieldValue(fieldName);
					if (data)
					{
						data->print(indent+2);

						delete data;
					}
				}
			}
		}

	}
}

void CX3DNode::traverse()
{
	int n = countFields();

	for (int i=0; i<n; i++)
	{
		char *fieldName = getFieldName(i);

		if (fieldName)
		{
			CVRMLFieldData *data = createFieldValue(fieldName);
			if (data)
			{
				switch (data->getFieldType())
				{
					case NODE_DATA:
						{
							CVRMLNodeData *node = (CVRMLNodeData *)data;
							jobject vrmlNode = ((CVRMLNodeData *)data)->getValue();
							CX3DNode *child = CX3DNodeFactory::createNode(vrmlNode);
							if (child)
							{
								child->traverse();
								delete child;
							}
						}
						break;

					case NODE_ARRAY_DATA:
						{
							CVRMLNodeArrayData *nodeArray = (CVRMLNodeArrayData *)data;
							int m = nodeArray->count();
							for (int j=0; j<m; j++)
							{
								jobject vrmlNode = nodeArray->getValue(j);
								CX3DNode *child = CX3DNodeFactory::createNode(vrmlNode);
								if (child)
								{
									child->traverse();
									delete child;
								}
							}
						}
						break;

					default:
						break;
				}
			
				delete data;
			}
		}
	}
}


/////////////////////////////////////////////////////////
//
//  CX3DNodeFactory

CX3DNode *CX3DNodeFactory::createNode(jobject vrmlNode)
{
	CJNIUtil *ju = CJNIUtil::getUtil();
	CX3DNode *p = NULL;

	if (!ju->isInstanceOfVRMLNode(vrmlNode))
	{
		fprintf(stderr, "vrmlNode is not instance of VRMLNode [%s:%d]\n", __FILE__, __LINE__);
		exit(1);
	}

	char *nodeName = ju->VRMLNode_getNodeName(vrmlNode);
	if (nodeName)
	{
		if (strcmp(nodeName, "Material") == 0)
		{
			p = new CX3DMaterialNode(vrmlNode);
		}
		else if (strcmp(nodeName, "Transform") == 0)
		{
			p = new CX3DTransformNode(vrmlNode);
		}
		else if (strcmp(nodeName, "Group") == 0)
		{
			p = new CX3DGroupNode(vrmlNode);
		}
		else if (strcmp(nodeName, "Shape") == 0)
		{
			p = new CX3DShapeNode(vrmlNode);
		}
		else if (strcmp(nodeName, "Appearance") == 0)
		{
			p = new CX3DAppearanceNode(vrmlNode);
		}
		else if (strcmp(nodeName, "Box") == 0)
		{
			p = new CX3DBoxNode(vrmlNode);
		}
		else if (strcmp(nodeName, "Cone") == 0)
		{
			p = new CX3DConeNode(vrmlNode);
		}
		else if (strcmp(nodeName, "Cylinder") == 0)
		{
			p = new CX3DCylinderNode(vrmlNode);
		}
		else if (strcmp(nodeName, "Sphere") == 0)
		{
			p = new CX3DSphereNode(vrmlNode);
		}
		else if (strcmp(nodeName, "Color") == 0)
		{
			p = new CX3DColorNode(vrmlNode);
		}
		else if (strcmp(nodeName, "Coordinate") == 0)
		{
			p = new CX3DCoordinateNode(vrmlNode);
		}
		else if (strcmp(nodeName, "Normal") == 0)
		{
			p = new CX3DNormalNode(vrmlNode);
		}
		else if (strcmp(nodeName, "IndexedLineSet") == 0)
		{
			p = new CX3DIndexedLineSetNode(vrmlNode);
		}
		else if (strcmp(nodeName, "IndexedFaceSet") == 0)
		{
			p = new CX3DIndexedFaceSetNode(vrmlNode);
		}
		else if (strcmp(nodeName, "DirectionalLight") == 0)
		{
			p = new CX3DDirectionalLightNode(vrmlNode);
		}
		else if (strcmp(nodeName, "PointLight") == 0)
		{
			p = new CX3DPointLightNode(vrmlNode);
		}
		else if (strcmp(nodeName, "SpotLight") == 0)
		{
			p = new CX3DSpotLightNode(vrmlNode);
		}
		else if (strcmp(nodeName, "ImageTexture") == 0)
		{
			p = new CX3DImageTextureNode(vrmlNode);
		}
		else if (strcmp(nodeName, "TextureTransform") == 0)
		{
			p = new CX3DTextureTransformNode(vrmlNode);
		}
		else if (strcmp(nodeName, "TextureCoordinate") == 0)
		{
			p = new CX3DTextureCoordinateNode(vrmlNode);
		}
		else if (strcmp(nodeName, "Viewpoint") == 0)
		{
			p = new CX3DViewpointNode(vrmlNode);
		}
		else if (strcmp(nodeName, "Humanoid") == 0)
		{
			p = new CX3DOpenHRPHumanoidNode(vrmlNode);
		}
		else if (strcmp(nodeName, "Joint") == 0)
		{
			p = new CX3DOpenHRPJointNode(vrmlNode);
		}
		else if (strcmp(nodeName, "Segment") == 0)
		{
			p = new CX3DOpenHRPSegmentNode(vrmlNode);
		}
		//else if (strcmp(nodeName, "Displacer") == 0)
		//{
		//	p = new CX3DHAnimDisplacerNode(vrmlNode);
		//}
		//else if (strcmp(nodeName, "Site") == 0)
		//{
		//	p = new CX3DHAnimSiteNode(vrmlNode);
		//}
		else
		{
			p = new CX3DNode(vrmlNode);
		}
	}

	return p;
}

