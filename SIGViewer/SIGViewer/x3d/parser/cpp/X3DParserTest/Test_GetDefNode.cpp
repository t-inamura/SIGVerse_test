#include "CX3DParser.h"
#include "Test.h"
#include <vector>
#include <string>

// ==========================================
// ==========================================

void Test_printDefNode(CX3DParser& parser, char *vrmlFile, char *defName)
{
	// ---------------------------------------------
	// ---------------------------------------------
	if (!parser.parse(vrmlFile))
	{
		fprintf(stderr, "%s parse failed\n", vrmlFile);
		return;
	}

	CX3DParser::printLog("**** Node contents of defName(%s) ****\n", defName);

	// ---------------------------------------------
	// ---------------------------------------------
	CX3DNode *node = parser.getDefNode(defName);
	if (node)
	{
		node->print();

		delete node;
		node = NULL;
	}
}

