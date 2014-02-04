//
#ifndef __SGVLOG__
#define __SGVLOG__


#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <map>
#include <stdexcept>
#include <memory>
#include <cstdarg>

#include "boost/shared_ptr.hpp"
#include "boost/utility.hpp"
#include "boost/config.hpp"



#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
	#include <windows.h> 
	#include <direct.h>
#else
	#include <unistd.h>
	#include <sys/stat.h>
#endif


using namespace std;
using namespace boost;



namespace Sgv{

	
using namespace std;


//http://detail.chiebukuro.yahoo.co.jp/qa/question_detail/q1216095137

#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)

inline void sleep(const long& msec){
	::Sleep(static_cast<DWORD>(msec));
};

inline const int makeDir(const char* file)
{
	return static_cast<int>(::_mkdir(file));
};

inline const int removeDir(const char* file)
{
	return static_cast<int>(::_rmdir(file));
};

#else

//http://www.linux.or.jp/JM/html/LDP_man-pages/man3/sleep.3.html
inline void sleep(const long& msec){
	::usleep(static_cast<useconds_t>(msec*1000));
};

inline const int makeDir(const char* file)
{
	return static_cast<int>(::mkdir(file, 0));
};

inline const int removeDir(const char* file)
{
	return static_cast<int>(::rmdir(file));
};

#endif


//
class LogFileWriter;
//
typedef shared_ptr<LogFileWriter> LogFileWriterPtr;

//
class Log;
typedef shared_ptr<Log> LogPtr;


class FileLock : noncopyable
{
public:
	FileLock(const string& file)
		: m_filepath(file), m_folderpath(file + string("__lock_")), m_isLocked(false), m_sleepMsec(200)
	{};

	~FileLock(){};

	const bool lock();

	const bool unlock();

private:
	string m_filepath;
	string m_folderpath;
	bool m_isLocked;
	long m_sleepMsec;

};


class LogFileWriter : noncopyable
{
public:
	virtual ~LogFileWriter(){};
	
	virtual void write(const char* msg)=0;

	virtual void flush()=0;

	virtual LogFileWriter* clone() const =0;

protected:
	LogFileWriter(){};
};


///
typedef auto_ptr<LogFileWriter> LogFileWriterAutoPtr;


template <class TFileLock>
class SimpleFileWriter : public LogFileWriter
{
public:
	SimpleFileWriter(const string& file)
		: m_file(file), m_lock(file)
	{};

	///
	~SimpleFileWriter()
	{};

	///
	void write(const char* msg)
	{
		if(!m_lock.lock()){
			cerr << "SimpleFileWriter::write error, msg=" << msg << endl;
			return;
		}

		//
		ofstream ofs(m_file.c_str(), ios_base::app);
		ofs << msg << endl;
		
		m_lock.unlock();
		return;
	};

	void flush(){};

	//
	LogFileWriter* clone() const
	{
		return new SimpleFileWriter<TFileLock>(m_file);
	};

private:
	string m_file;
	TFileLock m_lock;
};


class DelayFileWriterBase : public LogFileWriter
{
public:

	void write(const char* msg)
	{
		if(!m_isStart){
			createTmpFile();
		}
		//
		m_ofs << msg << endl;
	};


protected:
	DelayFileWriterBase(const string& file)
		: m_file(file), m_isStart(false)
	{};

	///
	virtual ~DelayFileWriterBase(){};

	void writeLog();

	void createTmpFile();

	const string createFileName() const;

	const string& getFile() const
	{ return m_file;};

	inline const bool& getIsStart() const
	{ return m_isStart; };

	inline void close()
	{ 
		m_ofs.close(); 
		m_isStart = false;
	};

private:
	string m_file;
	string m_tmpfile;
	ofstream m_ofs;
	bool m_isStart;
};


template <class TFileLock>
class DelayFileWriter : public DelayFileWriterBase
{
public:
	///
	DelayFileWriter(const string& file)
		: DelayFileWriterBase(file), m_lock(file)
	{};

	virtual ~DelayFileWriter()
	{
		flush();
	};

	void flush()
	{
		if(!getIsStart()) return;

		if(!m_lock.lock()) return;
		
		writeLog();
		close();

		m_lock.unlock();
	};

	//
	LogFileWriter* clone() const
	{
		return new DelayFileWriter<TFileLock>(getFile());
	};

private:
	TFileLock m_lock;
};



void makeMsg(ostream& oss, const char* src, va_list& ap);


void writeLogHeader(ostream& os);





class LogBase
{
public:
	
	enum LogLevel{ERR=0, INFO, DEBUG, WARNING};



	///
	inline const LogLevel& getLevel() const
	{ return m_level; };

	///
	inline const string& getId() const
	{ return m_id; };

protected:
	LogBase(const enum LogLevel level, const string& id)
		: m_level(level), m_id(id)
	{};

	///
	~LogBase(){};

	void write(LogFileWriter& writer, const char* level, const char* msg)
	{
		//
		ostringstream oss;
		writeLogHeader(oss);
		oss << '[' << level << "] " << '[' << m_id << "] " << msg;
		
		//
		writer.write(oss.str().c_str());
	};

private:
	LogLevel m_level;
	string m_id;
};



const char* logLevelToStr(const LogBase::LogLevel level);


LogBase::LogLevel StrToLogLevel(const string& levelStr);


template <class Obj>
void toString(ostream& os, const Obj& obj)
{
	os << obj;
};




class Log : public LogBase
{
public:
	Log(const enum LogLevel level, const string& id, auto_ptr<LogFileWriter> writer)
		: LogBase(level, id), m_writer(writer)
	{};

	~Log(){};
	
	LogPtr clone() const
	{
		return LogPtr(
			new Log(
				getLevel(), getId(), auto_ptr<LogFileWriter>(m_writer->clone())
			)
		);
	};

	/// 
	inline void info(const char* msg)
	{
		if(getLevel() < INFO) return;
		write(*m_writer, logLevelToStr(INFO), msg);
	};

	///
	inline void debug(const char* msg)
	{
		if(getLevel() < DEBUG) return;
		write(*m_writer, logLevelToStr(DEBUG), msg);
	};

	///
	inline void err(const char* msg)
	{
		if(getLevel() < ERR) return;
		write(*m_writer, logLevelToStr(ERR), msg);
	};
	
	///
	inline void warning(const char* msg)
	{
		if(getLevel() < WARNING) return;
		write(*m_writer, logLevelToStr(WARNING), msg);
	};

	
	void printf(const LogLevel level, const char* msg, ...)
	{
		if(getLevel() < level) return;

		va_list ap;
		va_start(ap, msg);
		ostringstream oss;
		makeMsg(oss, msg, ap);
		write(*m_writer, logLevelToStr(level), oss.str().c_str());
		va_end(ap);
	};


	template <class Object>
	void printObj(const LogLevel level, const char* msg, const Object& obj)
	{
		if(getLevel() < level) return;
		
		//
		ostringstream oss;
		oss << msg << " ";
		toString<Object>(oss, obj);
		
		write(*m_writer, logLevelToStr(level), oss.str().c_str());
	};

	inline void flush()
	{
		m_writer->flush();
	};

protected:

private:
	auto_ptr<LogFileWriter> m_writer;
};



class LogFactory
{
	typedef std::map<int, shared_ptr<Log> > LogMap;
public:
	static LogPtr getLog(const int id)
	{
		LogMap::iterator i = m_logMap.find(id);
		if(i == m_logMap.end()) throw runtime_error("the id not exists. error in Logfactory::getLog()");
		return i->second;
	};

	/**
	*/
	static void setLog(const int id, const LogPtr& log)
	{
		m_logMap[id] = log;
	};

protected:
	LogFactory(){};
private:
	static LogMap m_logMap;
};


}; //namespace Sgv{

#endif


