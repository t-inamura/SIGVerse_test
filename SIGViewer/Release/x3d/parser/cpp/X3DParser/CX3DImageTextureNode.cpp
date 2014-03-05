#include "CX3DImageTextureNode.h"
#include "CX3DParserUtil.h"
#include "CX3DParser.h"
#include "CJNIUtil.h"

/////////////////////////////////////////////////////////
//
//  CX3DImageTextureNode

CX3DImageTextureNode::CX3DImageTextureNode(jobject vrmlNode)
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

		if (nodeName && strcmp(nodeName, "ImageTexture")==0)
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
						if (strcmp(fieldName.c_str(), "url") == 0)
						{
							m_url.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "repeatS") == 0)
						{
							m_repeatS.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "repeatT") == 0)
						{
							m_repeatT.setValue(data);
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

void CX3DImageTextureNode::print(int indent)
{
	FILE *fp = CX3DParser::getDebugLogFp();
	int nMax = CX3DParser::getMaxPrintElemsForMFField();
	bool bPartialPrint = false;

	char *nodeName = getNodeName();
	if (nodeName)
	{
		int i, n;

		CX3DParser::printIndent(indent);
		fprintf(fp, "%s (%s)\n", nodeName, CX3DNode::getNodeTypeString(getNodeType()));

		CX3DParser::printIndent(indent+1);
		n = m_url.count();
		fprintf(fp, "url [%d]\n", n);
		if ((nMax > 0) && (n > nMax)) { n = nMax; bPartialPrint = true; }
		else { bPartialPrint = false; }
		for (i=0; i<n; i++)
		{
			CX3DParser::printIndent(indent+2);
			fprintf(fp, "%s\n", m_url.getValue(i));
		}
		if (bPartialPrint)
		{
			CX3DParser::printIndent(indent+2);
			fprintf(fp, "...\n");
		}

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "repeatS : %s\n", getRepeatS()->getValue() ? "TRUE" : "FALSE");

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "repearT : %s\n", getRepeatT()->getValue() ? "TRUE" : "FALSE");

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "textureProperties\n");
		CX3DNode *pTexProp = getTextureProperties()->getNode();
		if (pTexProp) pTexProp->print(indent+2);
	}
}

