#include "CX3DParser.h"
#include "Test.h"
#include <vector>
#include <string>

// ==========================================
// ==========================================

void Test_printChildrenOfRoot(CX3DParser& parser, char *vrmlFile)
{
	// ---------------------------------------------
	// ---------------------------------------------
	if (!parser.parse(vrmlFile))
	{
		fprintf(stderr, "%s parse failed\n", vrmlFile);
		return;
	}

	CX3DParser::printLog("**** Children of Root Node ****\n");

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
				printf("%s\n", node->getNodeName());
			}
		}

		// ---------------------------------------------
		// ---------------------------------------------
		delete nodes;
		nodes = NULL;
	}
}

