#include "CX3DParser.h"
#include "Test.h"
#include <vector>
#include <string>

// ==========================================
// ==========================================

void Test_printFieldOfNode(CX3DParser& parser, char *vrmlFile)
{
	// ---------------------------------------------
	// ---------------------------------------------
	if (!parser.parse(vrmlFile))
	{
		fprintf(stderr, "%s parse failed\n", vrmlFile);
		return;
	}

	CX3DParser::printLog("**** Fields of Node in (%s) ****\n", vrmlFile);

	// ---------------------------------------------
	// ---------------------------------------------
	MFNode *nodes = parser.getChildrenOfRootNode();

	if (nodes)
	{
		// ---------------------------------------------
		// ---------------------------------------------
		for (int i=0; i<nodes->count(); i++)
		{
			CX3DNode *node = nodes->getNode(i);
			if (node)
			{
				// ------------------------------------
				// ------------------------------------
				char *nodeName = node->getNodeName();
				printf("%s\n", nodeName);

				// ------------------------------------
				// ------------------------------------
				int nFields = node->countFields();
				for (int iField=0; iField<nFields; iField++)
				{
					char *fieldName = node->getFieldName(iField);

					int fieldType = node->getFieldType(iField);

					char *fieldTypeName = node->getFieldTypeString(iField);

					printf("\t%s [%d:%s]\n", fieldName, fieldType, fieldTypeName);
				}
			}
		}

		// ---------------------------------------------
		// ---------------------------------------------
		delete nodes;
		nodes = NULL;
	}
}

