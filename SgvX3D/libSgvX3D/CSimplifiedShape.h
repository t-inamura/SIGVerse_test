/* $Id: CSimplifiedShape.h,v 1.3 2011-05-12 08:33:56 msi Exp $ */

#ifndef _CSIMPLIFIED_SHAPE_H_
#define _CSIMPLIFIED_SHAPE_H_

#include "CX3DNode.h"
#include "CX3DIndexedFaceSetNode.h"
#include "CX3DTransformNode.h"
#include "CX3DShapeNode.h"
#include "CX3DCylinderNode.h"
#include <float.h>
#include <vector>

//////////////////////////////////////////////////////////////////
//

class CSimplifiedShape
{
public:
	enum SHAPE_TYPE {
		NONE,
		SPHERE,
		CYLINDER,
		BOX
	};

	CSimplifiedShape() {}
	virtual ~CSimplifiedShape() {}

	virtual SHAPE_TYPE getType() = 0;

	float hizumi() { return m_hizumi; }
	void hizumi(float hizumi) { m_hizumi = hizumi; }

protected:
	float m_hizumi;
};


//////////////////////////////////////////////////////////////////
//

class CSimplifiedSphere : public CSimplifiedShape
{
public:
	CSimplifiedSphere() {}
	virtual ~CSimplifiedSphere() {}

	SHAPE_TYPE getType() { return SPHERE; }

	float radius() { return m_r; }
	void radius(float r) { m_r = r; }

	float x() { return m_x; }
	void x(float x) { m_x = x; }

	float y() { return m_y; }
	void y(float y) { m_y = y; }

	float z() { return m_z; }
	void z(float z) { m_z = z; }

private:
	// radius
	float m_r;

	// center position
	float m_x, m_y, m_z;
};


//////////////////////////////////////////////////////////////////
//

class CSimplifiedCylinder : public CSimplifiedShape
{
public:
	CSimplifiedCylinder() {}
	virtual ~CSimplifiedCylinder() {}

	SHAPE_TYPE getType() { return CYLINDER; }

	float radius() { return m_r; }
	void radius(float r) { m_r = r; }

	float height() { return m_h; }
	void height(float h) { m_h = h; }

	float x() { return m_x; }
	void x(float x) { m_x = x; }

	float y() { return m_y; }
	void y(float y) { m_y = y; }

	float z() { return m_z; }
	void z(float z) { m_z = z; }

private:
	// radius
	float m_r;

	// height
	float m_h;

	// center of bottom circle
	float m_x, m_y, m_z;
};


//////////////////////////////////////////////////////////////////
//

class CSimplifiedBox : public CSimplifiedShape
{
public:
	CSimplifiedBox() {}
	virtual ~CSimplifiedBox() {}

	SHAPE_TYPE getType() { return BOX; }

	float x1() { return m_x1; }
	void x1(float x) { m_x1 = x; }

	float y1() { return m_y1; }
	void y1(float y) { m_y1 = y; }

	float z1() { return m_z1; }
	void z1(float z) { m_z1 = z; }

	float x2() { return m_x2; }
	void x2(float x) { m_x2 = x; }

	float y2() { return m_y2; }
	void y2(float y) { m_y2 = y; }

	float z2() { return m_z2; }
	void z2(float z) { m_z2 = z; }

private:
	// positions
	float m_x1, m_y1, m_z1;
	float m_x2, m_y2, m_z2;
};


//////////////////////////////////////////////////////////////////
//

class CSimplifiedShapeFactory
{
public:
	// ---------------------------------------------------------
	//
	//
	//	+--T
	//	|  +--S
	//	|  :
	//	|  +--S
	//	|  +--T
	//	|  :
	//	|  +--T
	//	|	  +--S
	//	|	  :
	//	|	  +--S
	//	|	  +--T
	//	|	  :
	//	|	  +--T
	//	|  :
	//	+--T
	//
	// ---------------------------------------------------------
	static CSimplifiedSphere *calcSphereFromTree(MFNode *tree);
	static CSimplifiedCylinder *calcCylinderFromTree(MFNode *tree);
	static CSimplifiedBox *calcBoxFromTree(MFNode *tree);
	static CSimplifiedShape *calcAutoFromTree(MFNode *tree);
	static CSimplifiedShape *calcFromTree(MFNode *tree, CSimplifiedShape::SHAPE_TYPE hint=CSimplifiedShape::NONE);

	// --------------------------------------------------------
	//
	//
	//
	//	(ex)
	//	CSimplifiedShape *ss = CSimplifiedShapeFactory::calcAutoFromShapeNodes(shapes);
	//	if (ss) {
	//	   ....
	//	}
	// --------------------------------------------------------
	static CSimplifiedSphere *calcSphereFromShapeNodes(MFNode *shapes);
	static CSimplifiedCylinder *calcCylinderFromShapeNodes(MFNode *shapes);
	static CSimplifiedBox *calcBoxFromShapeNodes(MFNode *shapes);
	static CSimplifiedShape *calcAutoFromShapeNodes(MFNode *shapes);
	static CSimplifiedShape *calcFromShapeNodes(MFNode *shapes, CSimplifiedShape::SHAPE_TYPE hint=CSimplifiedShape::NONE);

	// --------------------------------------------------------
	//
	//
	//	(ex)
	//	CSimplifiedShape *ss = CSimplifiedShapeFactory::calcAutoFromShapeNode(pShape);
	//	if (ss) {
	//	   ....
	//	}
	// --------------------------------------------------------
	static CSimplifiedSphere *calcSphereFromShapeNode(CX3DShapeNode *pShape);
	static CSimplifiedCylinder *calcCylinderFromShapeNode(CX3DShapeNode *pShape);
	static CSimplifiedBox *calcBoxFromShapeNode(CX3DShapeNode *pShape);
	static CSimplifiedShape *calcAutoFromShapeNode(CX3DShapeNode *pShape);
	static CSimplifiedShape *calcFromShapeNode(CX3DShapeNode *pShape, CSimplifiedShape::SHAPE_TYPE hint=CSimplifiedShape::NONE);

	// --------------------------------------------------------
	//
	//
	//	(ex)
	//	CSimplifiedSphere *ss = CSimplifiedShapeFactory::calcSphere(pFaceSet);
	//	if (ss) {
	//	   ....
	//	}
	// --------------------------------------------------------
	static CSimplifiedSphere *calcSphere(CX3DIndexedFaceSetNode *pFaceSet);
	static CSimplifiedCylinder *calcCylinder(CX3DIndexedFaceSetNode *pFaceSet);
	static CSimplifiedBox *calcBox(CX3DIndexedFaceSetNode *pFaceSet);

private:
	static void extractPointsFromTree(CX3DTransformNode *pTrans, SFVec3f t, SFVec4f q, std::vector<SFVec3f>& vecPos);

	static void extractPointsFromShapeNodes(MFNode *shapeNodes, std::vector<SFVec3f>& vecPos);

	static void extractPointsFromShapeNode(CX3DShapeNode *pShape, std::vector<SFVec3f>& vecPos);

	static void extractPointsFromIndexedFaceSetNode(CX3DIndexedFaceSetNode *pFaceSet, std::vector<SFVec3f>& vecPos);

	static CSimplifiedSphere *calcSphere(std::vector<SFVec3f>& vecPos);
	static CSimplifiedCylinder *calcCylinder(std::vector<SFVec3f>& vecPos);
	static CSimplifiedBox *calcBox(std::vector<SFVec3f>& vecPos);

	static double calcHizumiWithSphere(float r, float gx, float gy, float gz, std::vector<SFVec3f>& vecPos);
	static double calcHizumiWithCylinder(float r, float cx, float cz, std::vector<SFVec3f>& vecPos);
	static double calcHizumiWithBox(float x1, float y1, float z1, float x2, float y2, float z2, std::vector<SFVec3f>& vecPos);

public:
	static CSimplifiedCylinder *calcCylinder(CX3DCylinderNode *pCylinderNode);

	static bool isCylinder(
			CX3DIndexedFaceSetNode *pNodeData,
			float &radiusEstimated,
			float &heightEstimated,
			float xAxis[],
			float yAxis[],
			float zAxis[]
		);

private:
	#define INVALID_VALUE (FLT_MAX)
	#define SMALL_VALUE (0.001f)
	#define SLIGHT_SMALL_VALUE (0.01f)

public:
	static float min6Float(float *f);

	static float sqrDist(float px, float py, float pz, float qx, float qy, float qz);

	static float getDistance(float px, float py, float pz, float qx, float qy, float qz);

	static void getCenter(MFVec3f *coords,float &gx,float &gy,float &gz);

	static void getNearPoint(MFVec3f *coords,float gx,float gy,float gz,float destX[],float destY[],float destZ[],int num);
};

#endif

