#include "CX3DOpenHRPJointNode.h"
#include "CX3DParserUtil.h"
#include "CX3DParser.h"
#include "CJNIUtil.h"

/////////////////////////////////////////////////////////
//
//	CX3DOpenHRPJointNode

CX3DOpenHRPJointNode::CX3DOpenHRPJointNode(jobject vrmlNode)
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
						else if (strcmp(fieldName.c_str(), "stiffness") == 0)
						{
							m_stiffness.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "ulimit") == 0)
						{
							m_ulimit.setValue(data);
						}

						else if (strcmp(fieldName.c_str(), "jointType") == 0)
						{
							m_jointType.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "jointId") == 0)
						{
							m_jointId.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "jointAxis") == 0)
						{
							m_jointAxis.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "equivalentInertia") == 0)
						{
							m_equivalentInertia.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "gearRatio") == 0)
						{
							m_gearRatio.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "rotorInertia") == 0)
						{
							m_rotorInertia.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "rotorResistor") == 0)
						{
							m_rotorResistor.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "torqueConst") == 0)
						{
							m_torqueConst.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "encoderPulse") == 0)
						{
							m_encoderPulse.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "gearEfficiency") == 0)
						{
							m_gearEfficiency.setValue(data);
						}

						delete data;
					}
				}
			}
		}
		delete tmpNode;
	}
}

void CX3DOpenHRPJointNode::print(int indent)
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

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "jointType (%s)\n", m_jointType.getValue());

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "jointId (%d)\n", m_jointId.getValue());

		CX3DParser::printIndent(indent+1);
		getJointAxis()->getValue(x, y, z);
		fprintf(fp, "jointAxis : (%f %f %f)\n", x, y, z);

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "equivalentInertia (%f)\n", m_equivalentInertia.getValue());

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "gearRatio (%f)\n", m_gearRatio.getValue());

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "rotorInertia (%f)\n", m_rotorInertia.getValue());

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "rotorResistor (%f)\n", m_rotorResistor.getValue());

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "torqueConst (%f)\n", m_torqueConst.getValue());

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "encoderPulse (%f)\n", m_encoderPulse.getValue());

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "gearEfficiency (%f)\n", m_gearEfficiency.getValue());

	}
}

