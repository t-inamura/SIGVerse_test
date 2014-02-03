#include "CX3DMaterialNode.h"
#include "CX3DParserUtil.h"
#include "CX3DParser.h"
#include "CJNIUtil.h"

/////////////////////////////////////////////////////////
//
//  CX3DMaterialNode

CX3DMaterialNode::CX3DMaterialNode(jobject vrmlNode)
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

		if (nodeName && strcmp(nodeName, "Material")==0)
		{
			m_isTrans = false;
			int n = tmpNode->countFields();
			for (int i=0; i<n; i++)
			{
				char *fieldName = tmpNode->getFieldName(i);
				if (fieldName)
				{
					CVRMLFieldData *data = tmpNode->createFieldValue(fieldName);
					if (data)
					{

						if (strcmp(fieldName, "diffuseColor") == 0)
						{
							m_diffuseColor.setValue(data);
						}
						else if (strcmp(fieldName, "specularColor") == 0)
						{
							m_specularColor.setValue(data);
						}
						else if (strcmp(fieldName, "emissiveColor") == 0)
						{
							m_emissiveColor.setValue(data);
						}
						else if (strcmp(fieldName, "shininess") == 0)
						{
							m_shininess.setValue(data);
						}
						else if (strcmp(fieldName, "transparency") == 0)
						{
							m_transparency.setValue(data);
							m_isTrans = true;
						}
						else if (strcmp(fieldName, "ambientIntensity") == 0)
						{
							m_ambientIntensity.setValue(data);
						}

						delete data;
					}
				}
			}
		}

		delete tmpNode;
	}
}

void CX3DMaterialNode::print(int indent)
{
	FILE *fp = CX3DParser::getDebugLogFp();

	char *nodeName = getNodeName();
	if (nodeName)
	{
		float r, g, b;

		CX3DParser::printIndent(indent);
		fprintf(fp, "%s (%s)\n", nodeName, CX3DNode::getNodeTypeString(getNodeType()));

		getDiffuseColor()->getValue(r, g, b);
		CX3DParser::printIndent(indent+1);
		fprintf(fp, "diffuseColor : (%f %f %f)\n", r, g, b);

		getSpecularColor()->getValue(r, g, b);
		CX3DParser::printIndent(indent+1);
		fprintf(fp, "specularColor : (%f %f %f)\n", r, g, b);

		getEmissiveColor()->getValue(r, g, b);
		CX3DParser::printIndent(indent+1);
		fprintf(fp, "emissiveColor : (%f %f %f)\n", r, g, b);

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "shininess : %f\n", getShininess()->getValue());

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "transparency : %f\n", getTransparency()->getValue());

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "ambientIntensity : %f\n", getAmbientIntensity()->getValue());
	}
}

