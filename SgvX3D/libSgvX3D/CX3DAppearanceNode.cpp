#include "CX3DAppearanceNode.h"
#include "CX3DParserUtil.h"
#include "CX3DParser.h"
#include "CJNIUtil.h"
#include <string.h>

/////////////////////////////////////////////////////////
//
//  CX3DAppearanceNode

CX3DAppearanceNode::CX3DAppearanceNode(jobject vrmlNode)
{
	if (!vrmlNode)
	{
		m_vrmlNode = NULL;
		return;
	}

	CJNIUtil *ju = CJNIUtil::getUtil();

	if (!ju->isInstanceOfVRMLNode(vrmlNode))
	{
		fprintf(stderr, "vrmlNode is not instance of VRMLNode [%s:%d]\n", __FILE__, __LINE__);
		exit(1);
	}

	m_vrmlNode = ju->env()->NewGlobalRef(vrmlNode);
	
	CX3DNode *tmpNode = new CX3DNode(vrmlNode);
	if (tmpNode)
	{
		char *nodeName = tmpNode->getNodeName();

		if (nodeName && strcmp(nodeName, "Appearance")==0)
		{
			int n = tmpNode->countFields();
			for (int i=0; i<n; i++)
			{
				char *pFieldName = tmpNode->getFieldName(i);
				if (pFieldName)
				{
					std::string fieldName = pFieldName;

					CVRMLFieldData *data = tmpNode->createFieldValue((char *)(fieldName.c_str()));
					if (data)
					{
						if (strcmp(fieldName.c_str(), "material") == 0)
						{
							m_material.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "texture") == 0)
						{
							m_texture.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "textureTransform") == 0)
						{
							m_textureTransform.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "pointProperties") == 0)
						{
							m_pointProperties.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "lineProperties") == 0)
						{
							m_lineProperties.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "fillProperties") == 0)
						{
							m_fillProperties.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "textureProperties") == 0)
						{
							m_textureProperties.setValue(data);
						}

						delete data;
					}
				}
			}
		}
		delete tmpNode;
	}
}

void CX3DAppearanceNode::print(int indent)
{
	FILE *fp = CX3DParser::getDebugLogFp();

	char *nodeName = getNodeName();
	if (nodeName)
	{
		CX3DParser::printIndent(indent);
		fprintf(fp, "%s (%s)\n", nodeName, CX3DNode::getNodeTypeString(getNodeType()));

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "material\n");
		CX3DNode *pMat = getMaterial()->getNode();
		if (pMat) pMat->print(indent+2);

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "texture\n");
		CX3DNode *pTex = getTexture()->getNode();
		if (pTex) pTex->print(indent+2);

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "textureTransform\n");
		CX3DNode *pTexTrans = getTextureTransform()->getNode();
		if (pTexTrans) pTexTrans->print(indent+2);

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "pointProperties\n");
		CX3DNode *pPointProp = getPointProperties()->getNode();
		if (pPointProp) pPointProp->print(indent+2);

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "lineProperties\n");
		CX3DNode *pLineProp = getLineProperties()->getNode();
		if (pLineProp) pLineProp->print(indent+2);

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "fillProperties\n");
		CX3DNode *pFillProp = getFillProperties()->getNode();
		if (pFillProp) pFillProp->print(indent+2);

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "textureProperties\n");
		CX3DNode *pTexProp = getTextureProperties()->getNode();
		if (pTexProp) pTexProp->print(indent+2);
	}
}

