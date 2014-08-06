#include "CX3DIndexedLineSetNode.h"
#include "CX3DParserUtil.h"
#include "CX3DParser.h"
#include "CJNIUtil.h"

/////////////////////////////////////////////////////////
//
//  CX3DIndexedLineSetNode

CX3DIndexedLineSetNode::CX3DIndexedLineSetNode(jobject vrmlNode)
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

		if (nodeName && strcmp(nodeName, "IndexedLineSet")==0)
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
						if (strcmp(fieldName.c_str(), "coord") == 0)
						{
							m_coord.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "color") == 0)
						{
							m_color.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "colorPerVertex") == 0)
						{
							m_colorPerVertex.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "coordIndex") == 0)
						{
							m_coordIndex.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "colorIndex") == 0)
						{
							m_colorIndex.setValue(data);
						}

						delete data;
					}
				}
			}
		}
		delete tmpNode;
	}
}

void CX3DIndexedLineSetNode::print(int indent)
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
		fprintf(fp, "coord\n");
		CX3DNode *pCoord = getCoord()->getNode();
		if (pCoord) pCoord->print(indent+2);

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "color\n");
		CX3DNode *pColor = getColor()->getNode();
		if (pColor) pColor->print(indent+2);

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "colorPerVertex : %s\n", getColorPerVertex()->getValue() ? "TRUE" : "FALSE");

		CX3DParser::printIndent(indent+1);
		n = m_coordIndex.count();
		fprintf(fp, "coordIndex [%d]\n", n);
		if ((nMax > 0) && (n > nMax)) { n = nMax; bPartialPrint = true; }
		CX3DParser::printIndent(indent+2);
		fprintf(fp, "(");
		for (i=0; i<n; i++)
		{
			fprintf(fp, "%d ", m_coordIndex.getValue(i));
		}
		if (bPartialPrint) fprintf(fp, "...");
		fprintf(fp, ")\n");

		CX3DParser::printIndent(indent+1);
		n = m_colorIndex.count();
		fprintf(fp, "colorIndex [%d]\n", n);
		if ((nMax > 0) && (n > nMax)) { n = nMax; bPartialPrint = true; }
		CX3DParser::printIndent(indent+2);
		fprintf(fp, "(");
		for (i=0; i<n; i++)
		{
			fprintf(fp, "%d ", m_colorIndex.getValue(i));
		}
		if (bPartialPrint) fprintf(fp, "...");
		fprintf(fp, ")\n");
	}
}

