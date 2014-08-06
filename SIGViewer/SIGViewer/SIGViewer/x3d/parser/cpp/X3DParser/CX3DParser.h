#ifndef _CX3D_PARSER_H_
#define _CX3D_PARSER_H_

#include "CX3DNode.h"
#include "CX3DBoxNode.h"
#include "CX3DColorNode.h"
#include "CX3DConeNode.h"
#include "CX3DCoordinateNode.h"
#include "CX3DCylinderNode.h"
#include "CX3DDirectionalLightNode.h"
#include "CX3DGroupNode.h"
#include "CX3DHAnimDisplacerNode.h"
#include "CX3DHAnimHumanoidNode.h"
#include "CX3DHAnimJointNode.h"
#include "CX3DHAnimSegmentNode.h"
#include "CX3DHAnimSiteNode.h"
#include "CX3DImageTextureNode.h"
#include "CX3DIndexedFaceSetNode.h"
#include "CX3DIndexedLineSetNode.h"
#include "CX3DMaterialNode.h"
#include "CX3DNormalNode.h"
#include "CX3DOpenHRPHumanoidNode.h"
#include "CX3DOpenHRPJointNode.h"
#include "CX3DOpenHRPSegmentNode.h"
#include "CX3DPointLightNode.h"
#include "CX3DShapeNode.h"
#include "CX3DSphereNode.h"
#include "CX3DSpotLightNode.h"
#include "CX3DTextureCoordinateNode.h"
#include "CX3DTextureTransformNode.h"
#include "CX3DTransformNode.h"
#include "CX3DViewpointNode.h"
#include "CJNIUtil.h"

#include <jni.h>
#include <vector>

class CX3DParser
{
public:
	CX3DParser();
	virtual ~CX3DParser();

	// =================================================
	//	X3D Parse Methods
	// =================================================

	// ---------------------------------------------
	// ---------------------------------------------
	bool parse(char *fname);

	// ---------------------------------------------
	// ---------------------------------------------
	std::string getFileName() {return m_fname;}

	// ---------------------------------------------
	// ---------------------------------------------
	void print();

	void printNodeTypeList();

	// ---------------------------------------------
	// ---------------------------------------------
	MFNode *getChildrenOfRootNode();

	// ---------------------------------------------
	//
	// ---------------------------------------------
	MFNode *searchNodesFromDirectChildrenOfRoot(char *nodeName);

	// ---------------------------------------------
	// ---------------------------------------------
	MFNode *searchNodesFromAllChildrenOfRoot(char *nodeName);

	// ---------------------------------------------
	// ---------------------------------------------
	std::vector<std::string> getDefNames();

	// ---------------------------------------------
	// ---------------------------------------------
	CX3DNode *getDefNode(char *defName);


	// =================================================
	// =================================================

	// ---------------------------------------------
	//
	// ---------------------------------------------
	static void openDebugLog(char *fname, bool bAppend=false);

	// ---------------------------------------------
	//
	// ---------------------------------------------
	static void closeDebugLog();

	// ---------------------------------------------
	//
	//
	// ---------------------------------------------
	static void setDebugLogFp(FILE *fp);

	// ---------------------------------------------
	//
	// ---------------------------------------------
	static void resetDebugLogFp();

	// ---------------------------------------------
	// ---------------------------------------------
	static FILE *getDebugLogFp();

	// ---------------------------------------------
	// ---------------------------------------------
	static void printIndent(int indent);

	// -----------------------------------------------------
	//
	// -----------------------------------------------------
	static void printLog(char *format, ...);

	// ---------------------------------------------
	// ---------------------------------------------
	static void printIndentLog(int indentLevel, char *format, ...);

	// ---------------------------------------------
	// ---------------------------------------------
	static void flushLog();

	// ----------------------------------------------------------
	//
	// 
	//
	// ----------------------------------------------------------
	static void setMaxPrintElemsForMFField(int n);

	static int getMaxPrintElemsForMFField();

private:
	std::string m_fname;
	jobject m_X3DParser;
};

#endif

