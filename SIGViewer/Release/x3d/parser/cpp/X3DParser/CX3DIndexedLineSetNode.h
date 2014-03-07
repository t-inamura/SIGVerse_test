#ifndef _CX3DINDEXEDLINESETNODE_H_
#define _CX3DINDEXEDLINESETNODE_H_

#include "CX3DNode.h"

class CX3DIndexedLineSetNode : public CX3DNode
{
public:
	CX3DIndexedLineSetNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return INDEXED_LINE_SET_NODE; }

	SFNode *getCoord() { return &m_coord; }
	SFNode *getColor() { return &m_color; }
	//SFNode *getFogCoord() { return &m_fogCoord; }
	//SFNode *getAttrib() { return &m_attrib; }
	SFBool *getColorPerVertex() { return &m_colorPerVertex; }
	MFInt32 *getCoordIndex() { return &m_coordIndex; }
	MFInt32 *getColorIndex() { return &m_colorIndex; }

	void print(int indent=0);

private:
	SFNode m_coord;
	SFNode m_color;
	//SFNode m_fogCoord;
	//SFNode m_attrib;
	SFBool m_colorPerVertex;
	MFInt32 m_coordIndex;
	MFInt32 m_colorIndex;
};

#endif
