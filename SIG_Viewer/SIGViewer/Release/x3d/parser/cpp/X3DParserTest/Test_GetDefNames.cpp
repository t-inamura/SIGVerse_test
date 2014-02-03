#include "CX3DParser.h"
#include "Test.h"
#include <vector>
#include <string>

// ==========================================
// ==========================================

void Test_printDefNames(CX3DParser& parser, char *vrmlFile)
{
	// ---------------------------------------------
	// ---------------------------------------------
	if (!parser.parse(vrmlFile))
	{
		fprintf(stderr, "%s parse failed\n", vrmlFile);
		return;
	}

	// ---------------------------------------------
	// ---------------------------------------------
	std::vector<std::string> defNames = parser.getDefNames();
	std::vector<std::string>::iterator itr;

	printf("*** def names in %s ***\n", vrmlFile);

	for (itr=defNames.begin(); itr!=defNames.end(); itr++)
	{
		std::string defName = *itr;

		printf("%s\n", defName.c_str());
	}
}

