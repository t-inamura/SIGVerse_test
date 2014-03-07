#include "CX3DConeNode.h"
#include "CX3DParserUtil.h"
#include "CX3DParser.h"
#include "CJNIUtil.h"
#include <string.h>

/////////////////////////////////////////////////////////
//
//  CX3DConeNode

CX3DConeNode::CX3DConeNode(jobject vrmlNode)
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

		if (nodeName && strcmp(nodeName, "Cone")==0)
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
						if (strcmp(fieldName.c_str(), "solid") == 0)
						{
							m_solid.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "bottomRadius") == 0)
						{
							m_bottomRadius.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "height") == 0)
						{
							m_height.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "bottom") == 0)
						{
							m_bottom.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "side") == 0)
						{
							m_side.setValue(data);
						}

						delete data;
					}
				}
			}
		}
		delete tmpNode;
	}
}

void CX3DConeNode::print(int indent)
{
	FILE *fp = CX3DParser::getDebugLogFp();

	char *nodeName = getNodeName();
	if (nodeName)
	{
		CX3DParser::printIndent(indent);
		fprintf(fp, "%s (%s)\n", nodeName, CX3DNode::getNodeTypeString(getNodeType()));

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "solid : %s\n", getSolid()->getValue() ? "TRUE" : "FALSE");

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "bottomRadius : (%f)\n", getBottomRadius()->getValue());

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "height       : (%f)\n", getHeight()->getValue());

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "bottom : %s\n", getBottom()->getValue() ? "TRUE" : "FALSE");

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "side   : %s\n", getSide()->getValue() ? "TRUE" : "FALSE");
	}
}

