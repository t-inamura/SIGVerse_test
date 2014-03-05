#include "CX3DHAnimSegmentNode.h"
#include "CX3DParserUtil.h"
#include "CX3DParser.h"
#include "CJNIUtil.h"

/////////////////////////////////////////////////////////
//
//  CX3DHAnimSegmentNode

CX3DHAnimSegmentNode::CX3DHAnimSegmentNode(jobject vrmlNode)
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

		if (nodeName && strcmp(nodeName, "Segment")==0)
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
						if (strcmp(fieldName.c_str(), "children") == 0)
						{
							m_children.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "name") == 0)
						{
							m_name.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "coord") == 0)
						{
							m_coord.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "centerOfMass") == 0)
						{
							m_centerOfMass.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "displacers") == 0)
						{
							m_displacers.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "mass") == 0)
						{
							m_mass.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "momentsOfInertia") == 0)
						{
							m_momentsOfInertia.setValue(data);
						}

						delete data;
					}
				}
			}
		}
		delete tmpNode;
	}
}

void CX3DHAnimSegmentNode::print(int indent)
{
	FILE *fp = CX3DParser::getDebugLogFp();
	int nMax = CX3DParser::getMaxPrintElemsForMFField();
	bool bPartialPrint = false;

	char *nodeName = getNodeName();
	if (nodeName)
	{
		float x, y, z;
		MFNode *nodes;
		int i, n;

		CX3DParser::printIndent(indent);
		fprintf(fp, "%s (%s)\n", nodeName, CX3DNode::getNodeTypeString(getNodeType()));

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "children\n");
		nodes = getChildren();
		n = nodes->count();
		for (i=0; i<n; i++)
		{
			CX3DNode *child = nodes->getNode(i);
			if (child)
			{
				child->print(indent+2);
			}
		}

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "name (%s)\n", m_name.getValue());

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "coord\n");
		CX3DNode *pCoord = getCoord()->getNode();
		if (pCoord) pCoord->print(indent+2);

		CX3DParser::printIndent(indent+1);
		getCenterOfMass()->getValue(x, y, z);
		fprintf(fp, "centerOfMass : (%f %f %f)\n", x, y, z);

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "displacers\n");
		nodes = getDisplacers();
		n = nodes->count();
		for (i=0; i<n; i++)
		{
			CX3DNode *child = nodes->getNode(i);
			if (child)
			{
				child->print(indent+2);
			}
		}

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "mass (%f)\n", getMass()->getValue());

		CX3DParser::printIndent(indent+1);
		n = m_momentsOfInertia.count();
		fprintf(fp, "momentsOfInertia [%d]\n", n);
		if ((nMax > 0) && (n > nMax)) { n = nMax; bPartialPrint = true; }
		else { bPartialPrint = false; }
		CX3DParser::printIndent(indent+2);
		fprintf(fp, "(");
		for (i=0; i<n; i++)
		{
			fprintf(fp, "%f ", m_momentsOfInertia.getValue(i));
		}
		if (bPartialPrint) fprintf(fp, "...");
		fprintf(fp, ")\n");
	}
}

