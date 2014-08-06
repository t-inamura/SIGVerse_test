#include "CX3DHAnimJointNode.h"
#include "CX3DParserUtil.h"
#include "CX3DParser.h"
#include "CJNIUtil.h"

/////////////////////////////////////////////////////////
//
//  CX3DHAnimJointNode

CX3DHAnimJointNode::CX3DHAnimJointNode(jobject vrmlNode)
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

		if (nodeName && strcmp(nodeName, "Joint")==0)
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
						else if (strcmp(fieldName.c_str(), "center") == 0)
						{
							m_center.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "rotation") == 0)
						{
							m_rotation.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "scale") == 0)
						{
							m_scale.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "scaleOrientation") == 0)
						{
							m_scaleOrientation.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "translation") == 0)
						{
							m_translation.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "name") == 0)
						{
							m_name.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "limitOrientation") == 0)
						{
							m_limitOrientation.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "llimit") == 0)
						{
							m_llimit.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "skinCoordIndex") == 0)
						{
							m_skinCoordIndex.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "skinCoordWeight") == 0)
						{
							m_skinCoordWeight.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "stiffness") == 0)
						{
							m_stiffness.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "ulimit") == 0)
						{
							m_ulimit.setValue(data);
						}

						delete data;
					}
				}
			}
		}
		delete tmpNode;
	}
}

void CX3DHAnimJointNode::print(int indent)
{
	FILE *fp = CX3DParser::getDebugLogFp();
	int nMax = CX3DParser::getMaxPrintElemsForMFField();
	bool bPartialPrint = false;

	char *nodeName = getNodeName();
	if (nodeName)
	{
		float x, y, z, rot;
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
		getCenter()->getValue(x, y, z);
		fprintf(fp, "center : (%f %f %f)\n", x, y, z);

		CX3DParser::printIndent(indent+1);
		getRotation()->getValue(x, y, z, rot);
		fprintf(fp, "rotation : (%f %f %f)(%f)\n", x, y, z, rot);

		CX3DParser::printIndent(indent+1);
		getScale()->getValue(x, y, z);
		fprintf(fp, "scale : (%f %f %f)\n", x, y, z);

		CX3DParser::printIndent(indent+1);
		getScaleOrientation()->getValue(x, y, z, rot);
		fprintf(fp, "scaleOrientation : (%f %f %f)(%f)\n", x, y, z, rot);

		CX3DParser::printIndent(indent+1);
		getTranslation()->getValue(x, y, z);
		fprintf(fp, "translation : (%f %f %f)\n", x, y, z);

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "name (%s)\n", m_name.getValue());

		CX3DParser::printIndent(indent+1);
		getLimitOrientation()->getValue(x, y, z, rot);
		fprintf(fp, "limitOrientation : (%f %f %f)(%f)\n", x, y, z, rot);

		CX3DParser::printIndent(indent+1);
		n = m_llimit.count();
		fprintf(fp, "llimit [%d]\n", n);
		if ((nMax > 0) && (n > nMax)) { n = nMax; bPartialPrint = true; }
		else { bPartialPrint = false; }
		CX3DParser::printIndent(indent+2);
		fprintf(fp, "(");
		for (i=0; i<n; i++)
		{
			fprintf(fp, "%f ", m_llimit.getValue(i));
		}
		if (bPartialPrint) fprintf(fp, "...");
		fprintf(fp, ")\n");

		CX3DParser::printIndent(indent+1);
		n = m_skinCoordIndex.count();
		fprintf(fp, "skinCoordIndex [%d]\n", n);
		if ((nMax > 0) && (n > nMax)) { n = nMax; bPartialPrint = true; }
		else { bPartialPrint = false; }
		CX3DParser::printIndent(indent+2);
		fprintf(fp, "(");
		for (i=0; i<n; i++)
		{
			fprintf(fp, "%d ", m_skinCoordIndex.getValue(i));
		}
		if (bPartialPrint) fprintf(fp, "...");
		fprintf(fp, ")\n");

		CX3DParser::printIndent(indent+1);
		n = m_skinCoordWeight.count();
		fprintf(fp, "skinCoordWeight [%d]\n", n);
		if ((nMax > 0) && (n > nMax)) { n = nMax; bPartialPrint = true; }
		else { bPartialPrint = false; }
		CX3DParser::printIndent(indent+2);
		fprintf(fp, "(");
		for (i=0; i<n; i++)
		{
			fprintf(fp, "%f ", m_skinCoordWeight.getValue(i));
		}
		if (bPartialPrint) fprintf(fp, "...");
		fprintf(fp, ")\n");

		CX3DParser::printIndent(indent+1);
		n = m_stiffness.count();
		fprintf(fp, "stiffness [%d]\n", n);
		if ((nMax > 0) && (n > nMax)) { n = nMax; bPartialPrint = true; }
		else { bPartialPrint = false; }
		CX3DParser::printIndent(indent+2);
		fprintf(fp, "(");
		for (i=0; i<n; i++)
		{
			fprintf(fp, "%f ", m_stiffness.getValue(i));
		}
		if (bPartialPrint) fprintf(fp, "...");
		fprintf(fp, ")\n");

		CX3DParser::printIndent(indent+1);
		n = m_ulimit.count();
		fprintf(fp, "ulimit [%d]\n", n);
		if ((nMax > 0) && (n > nMax)) { n = nMax; bPartialPrint = true; }
		else { bPartialPrint = false; }
		CX3DParser::printIndent(indent+2);
		fprintf(fp, "(");
		for (i=0; i<n; i++)
		{
			fprintf(fp, "%f ", m_ulimit.getValue(i));
		}
		if (bPartialPrint) fprintf(fp, "...");
		fprintf(fp, ")\n");
	}
}

