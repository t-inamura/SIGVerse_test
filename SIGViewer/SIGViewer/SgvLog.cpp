
#include <memory>

#include "SgvLog.h"



namespace Sgv{



using namespace std;

//
LogFactory::LogMap LogFactory::m_logMap;


//DelayFileWriterBase--------------------------------------


void DelayFileWriterBase::createTmpFile()
{
	m_tmpfile = createFileName();
	m_ofs.open(m_tmpfile.c_str(), ios_base::app);
	m_isStart = true;
};

const string DelayFileWriterBase::createFileName() const
{
	srand((unsigned int)time(NULL));
	ostringstream oss;
	oss << hex << uppercase << setfill('0');
	
	char tmp[30];
	time_t t = time(NULL);
	tm ti;
	localtime_s(&ti,&t);
	//tm ti = *localtime(&t);
	strftime(tmp, 30, "%Y%m%d%H%M%S", &ti);

	oss << tmp << '_' << (void*)auto_ptr<char>(new char).get();

	for(int i=0; i<5; ++i){
		int c = (int)((float)rand()/RAND_MAX*255);
		oss << setw(2) << c;
	}
	return oss.str();
};



//
void DelayFileWriterBase::writeLog()
{
	m_ofs.close();

	ofstream ofs(m_file.c_str(), ios_base::app);
	ifstream ifs(m_tmpfile.c_str());
	ofs << ifs.rdbuf();
	ofs.close();
	ifs.close();

	remove(m_tmpfile.c_str());
};





//FileLock--------------------------------------
//
const bool FileLock::lock()
{
	if(m_isLocked) return true;
	for(int i=0; i<3; ++i){
		if( !makeDir(m_folderpath.c_str()) ){
			m_isLocked = true;		
			return true;
		}
		sleep(m_sleepMsec);
	}
	return false;
};


//
const bool FileLock::unlock()
{
	if(!m_isLocked) return true;
	if( !removeDir(m_folderpath.c_str()) ){
		m_isLocked = false;
		return true;
	}
	return false;
};



//------------------------

//
void makeMsg(ostream& oss, const char* src, va_list& ap)
{
	char c;

	while(c = *src++){
		if(c == '%'){
			switch(c = *src++){
			case 's':
				oss << va_arg(ap, char*);
				break;
			case 'd':
				oss << va_arg(ap, int);
				break;
			case 'l':
				oss << va_arg(ap, long);
				break;
			case 'f':
				oss << va_arg(ap, double);
				break;
			case 'c':
				oss << static_cast<char>(va_arg(ap, int));
				break;
			}
		}else{
			oss << c;
		}
	}

};


//
const char* logLevelToStr(const LogBase::LogLevel level)
{
	static const char* err = "ERR";
	static const char* info = "INFO";
	static const char* debug = "DEBUG";
	static const char* warning = "WARNING";
	static const char* other = "?";

	switch(level){
	case Log::ERR:
		return err;
	case Log::INFO:
		return info;
	case Log::DEBUG:
		return debug;
	case Log::WARNING:
		return warning;
	}

	//
	return other;
};


//
Log::LogLevel StrToLogLevel(const string& levelStr)
{
	if(levelStr == "ERR"){
		return Log::ERR;
	}else if(levelStr == "INFO"){
		return  Log::INFO;
	}else if(levelStr == "DEBUG"){
		return Log::DEBUG;
	}else if(levelStr == "WARNING"){
		return Log::WARNING;
	}

	//
	return Log::INFO;
};

//
void writeLogHeader(ostream& os)
{
	char tmp[30];
	time_t t = time(NULL);
	tm ti;
	localtime_s(&ti,&t);
	strftime(tmp, 30, "%Y/%m/%d %H:%M:%S", &ti);

	os << tmp << " "; 
};


}; //namespace nana{


