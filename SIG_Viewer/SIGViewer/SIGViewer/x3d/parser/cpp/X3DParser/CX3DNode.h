#ifndef _CX3DNODE_H_
#define _CX3DNODE_H_

#include "CX3DField.h"
#include "CVRMLFieldData.h"
#include <jni.h>

//#include "CX3DField.h"
class CVRMLFieldData;


/////////////////////////////////////////////////////////////
//
//  Node ID

enum CX3DNodeType
{
	BASE_NODE,	
	APPEARANCE_NODE,
	BOX_NODE,
	COLOR_NODE,
	CONE_NODE,
	COORDINATE_NODE,
	CYLINDER_NODE,
	DIRECTIONAL_LIGHT_NODE,
	GROUP_NODE,
	HANIM_DISPLACER_NODE,
	HANIM_HUMANOID_NODE,
	HANIM_JOINT_NODE,
	HANIM_SEGMENT_NODE,
	HANIM_SITE_NODE,
	IMAGE_TEXTURE_NODE,
	INDEXED_FACE_SET_NODE,
	INDEXED_LINE_SET_NODE,
	MATERIAL_NODE,
	NORMAL_NODE,
	OPENHRP_HUMANOID_NODE,
	OPENHRP_JOINT_NODE,
	OPENHRP_SEGMENT_NODE,
	POINT_LIGHT_NODE,
	SCENE_NODE,
	SHAPE_NODE,
	SPHERE_NODE,
	SPOT_LIGHT_NODE,
	TEXTURE_COORDINATE_NODE,
	TEXTURE_TRANSFORM_NODE,
	TRANSFORM_NODE,
	VIEWPOINT_NODE
};


/////////////////////////////////////////////////////////////
//
//  Base class

class CX3DNode
{
public:
	CX3DNode();
	CX3DNode(jobject vrmlNode);
	virtual ~CX3DNode();

	// ------------------------------------------
	// ------------------------------------------
	virtual CX3DNodeType getNodeType() { return BASE_NODE; }

	// ------------------------------------------
	// ------------------------------------------
	static char *getNodeTypeString(CX3DNodeType ntype);

	// ------------------------------------------
	// ------------------------------------------
	char *getNodeName();

	// ------------------------------------------
	// ------------------------------------------
	int countFields();

	// ------------------------------------------
	//
	// ------------------------------------------
	char *getFieldName(int iField);
	
	// ------------------------------------------
	//
	// ------------------------------------------
	int getFieldType(int iField);
	
	// ------------------------------------------
	//
	// ------------------------------------------
	char *getFieldTypeString(int iField);

	// ------------------------------------------
	// ------------------------------------------
	int getFieldIndex(char *fieldName);

	// --------------------------------------------------------------
	// --------------------------------------------------------------
	CVRMLFieldData *createFieldValue(int iField);
	CVRMLFieldData *createFieldValue(char *fieldName);

	// --------------------------------------------------------
	// --------------------------------------------------------
	MFNode *searchNodesFromDirectChildren(char *nodeName);

	MFNode *searchNodesFromAllChildren(char *nodeName);
	
	// ------------------------------------------
	// ------------------------------------------
	virtual void print(int indent=0);

	// ------------------------------------------
	// ------------------------------------------
	void traverse();

protected:
	jobject m_vrmlNode;
};


/////////////////////////////////////////////////////////////
//
//  Node Factory

class CX3DNodeFactory
{
public:
	static CX3DNode *createNode(jobject vrmlNode);
};

#endif


