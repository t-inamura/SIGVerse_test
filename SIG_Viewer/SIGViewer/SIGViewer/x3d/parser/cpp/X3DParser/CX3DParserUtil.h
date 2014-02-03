#ifndef _CX3DPARSERUTIL_H_
#define _CX3DPARSERUTIL_H_

#include <string>
#include <string.h>

class CX3DParserUtil
{
public:
	// -------------------------------------------------------
	//
	// (ex)
	// ""
	// ---> ""
	// "aiueo"
	// ---> "aiueo"
	// "aiueo.txt"
	// ---> "aiueo"
	// "G:/aiueo/kaki/xyz.jpg"
	// ---> "xyz"
	// -------------------------------------------------------
	static std::string extractFileBaseName(const char *pathName);
};

#endif

