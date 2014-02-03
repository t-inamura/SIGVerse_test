// -------------------------------------------------------------------
//
//  Java VM configration file interface
//
//  (( config file's format ))
//
//  # comment (# must be placed at 1st char in line)
//  KEY1=VALUE1
//  KEY2=VALUE2
//  ....
//  [Others]
//  -XXX1		# under [Others] line, any description will
//  -XXX2		# pass to JavaVM's option string directly.
//
//  (( example ))
//
//  #
//	# Java Class Path
//	#
//	java.class.path=/Somewhere/xj3d-all.jar
//	#
//	# Other Java VM options
//	#
//	[Others]
//	-Xmx450M
//	-Xms10M
//
// -------------------------------------------------------------------

#ifndef _SGV_CONFIG_FILE_H_
#define _SGV_CONFIG_FILE_H_

#include <vector>
#include <map>
#include <string>

namespace Sgv
{
	class ConfigFile
	{
	public:
		ConfigFile();
		~ConfigFile();

		bool load(const char *configFile);

		// returns NULL if specified name doesn't exist in config file.
		const char *getStringValue(const char *name);

		// begin(add)(2009/11/30)
		int countOtherOptions();
		const char *getOtherOption(int i);
		// end(add)

		static std::string trim(std::string str);

	private:
		std::map<std::string, std::string> m_values;
		std::vector<std::string> m_otherValues;
	};
}

#endif
