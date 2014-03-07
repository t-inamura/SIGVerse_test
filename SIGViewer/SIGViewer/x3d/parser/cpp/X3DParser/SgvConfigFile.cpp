#include "SgvConfigFile.h"
#include <string>
#include <iostream>
#include <fstream>
#include <string.h>

namespace Sgv
{
	ConfigFile::ConfigFile()
	{
		m_values.clear();
		m_otherValues.clear();
	}

	ConfigFile::~ConfigFile()
	{
	}

	bool ConfigFile::load(const char *configFile)
	{
		if (!configFile) return false;

		m_values.clear();
		m_otherValues.clear();

		std::ifstream ifs(configFile);
		if (ifs.fail())
		{
			fprintf(stderr, "file not found : (%s)\n", configFile);
			return false;
		}

		std::string line;
		bool bOtherSection = false;

#if 1
		// begin(bugfix)(2009/1/16)
		//
		//
		// [test1.cfg]
		// NAME=kuma<CRLF>
		// AGE=3
		//
		//
		//
		// std::ifstream ifs(fileName);
		// std::string line;
		//
		// getline(ifs, line);
		// while (!ifs.eof())
		// {
		//		printf("%s\n", line.c_str());
		//		getline(ifs, line);
		// }
		//
		// ifs.close();
		//
		// 
		// std::ifstream ifs(fileName);
		// std::string line;
		//
		// getline(ifs, line);
		// while (true)
		// {
		//		printf("%s\n", line.c_str());
		//		if (ifs.eof()) break;
		//
		//		getline(ifs, line);
		// }
		//
		// ifs.close();
		//
		//
		getline(ifs, line);
		while (true)
		{
			if (line.size() > 0)
			{
				std::string s = line.substr(0, 1);

				if (strcmp(s.c_str(), "#") != 0)	// skip comment
				{
					if (strcmp(line.c_str(), "[Others]") == 0)
					{
						bOtherSection = true;
					}
					else
					{
						if (!bOtherSection)
						{
							int i = line.find("=");
							if (i>=0)
							{
								std::string name = ConfigFile::trim(line.substr(0, i));
								std::string value = ConfigFile::trim(line.substr(i+1));
								if (name.length() > 0)
								{
//									printf("name=(%s), value=(%s)\n", name.c_str(), value.c_str());
									m_values[name] = value;
								}
							}
						}
						else
						{
							std::string otherValue = Sgv::ConfigFile::trim(line);
							if (otherValue.size() > 0)
							{
								m_otherValues.push_back(otherValue);
							}
						}
					}
				}
			}

			if (ifs.eof()) break;

			getline(ifs, line);
		}
		// end(bugfix)
#endif

		ifs.close();

		return true;
	}

	std::string ConfigFile::trim(std::string str)
	{
		int i, j;
		
		i = str.find_first_not_of(" ");
		if (i==-1) return "";	// all chars are space

		if (i>0) str = str.substr(i);	// cut top spaces
		
		j = str.find_last_not_of(" ");
		if (j>0) str = str.substr(0, j+1);	// cut bottom spaces

		return str;
	}

	const char *ConfigFile::getStringValue(const char *name)
	{
		if (!name) return NULL;

		const char *value = m_values[name].c_str();

		return (value && *value) ? value : NULL;
	}

	int ConfigFile::countOtherOptions()
	{
		return (int)m_otherValues.size();
	}

	const char *ConfigFile::getOtherOption(int i)
	{
		if (i<0 || i>=countOtherOptions()) return NULL;

		const char *value = m_otherValues[i].c_str();

		return (value && *value) ? value : NULL;
	}
}

