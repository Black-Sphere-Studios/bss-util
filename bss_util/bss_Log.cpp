// Copyright ©2012 Black Sphere Studios
// For conditions of distribution and use, see copyright notice in "bss_util.h"

#include "bss_Log.h"
#include "StreamSplitter.h"
#include "bss_util.h"
#include <fstream>
#include <iomanip>

using namespace bss_util;
using namespace std;


bss_Log::bss_Log(const bss_Log& copy) : _split(new StreamSplitter()), _stream(_split), _tz(GetTimeZoneMinutes()) { assert(false); }
//bss_Log::bss_Log(const bss_Log& copy) : _split(new StreamSplitter(*copy._split)), _stream(_split), _tz(GetTimeZoneMinutes())
//{
//}
bss_Log::bss_Log(bss_Log&& mov) : _split(mov._split), _backup(std::move(mov._backup)), _tz(GetTimeZoneMinutes()), _files(std::move(mov._files)),
#ifdef BSS_COMPILER_GCC // GCC has a bug where they haven't implemented move constructors for streams yet, which is not standards-compliant.
  _stream(mov._split)
#else
  _stream(std::move(mov._stream))
#endif
{
  mov._split=0;
}

bss_Log::bss_Log(std::ostream* log) : _split(new StreamSplitter()), _stream(_split), _tz(GetTimeZoneMinutes())
{
  if(log!=0)
    AddTarget(*log);
}
bss_Log::bss_Log(const char* logfile, std::ostream* log) : _split(new StreamSplitter()), _stream(_split), _tz(GetTimeZoneMinutes())
{
  AddTarget(logfile);
  if(log!=0)
    AddTarget(*log);
}
bss_Log::bss_Log(const wchar_t* logfile, std::ostream* log) : _split(new StreamSplitter()), _stream(_split), _tz(GetTimeZoneMinutes())
{
  AddTarget(logfile);
  if(log!=0)
    AddTarget(*log);
}
bss_Log::~bss_Log()
{
  ClearTargets();
  for(size_t i = 0; i < _backup.size(); ++i) //restore stream buffer backups so we don't blow up someone else's stream when destroying ourselves (suicide bombing is bad for your health)
    _backup[i].first.rdbuf(_backup[i].second);
  if(_split!=0) delete _split;
}
void BSS_FASTCALL bss_Log::Assimilate(std::ostream& stream)
{
  _backup.push_back(std::pair<std::ostream&, std::streambuf*>(stream,stream.rdbuf()));
  if(_split!=0) stream.rdbuf(_split);
}
void BSS_FASTCALL bss_Log::AddTarget(std::ostream& stream)
{
  if(_split!=0) _split->AddTarget(&stream);
}
void BSS_FASTCALL bss_Log::AddTarget(const char* file)
{
  if(!file) return;
#ifdef BSS_COMPILER_GCC 
  _files.push_back(std::unique_ptr<std::ofstream>(new ofstream(BSSPOSIX_WCHAR(file),ios_base::out|ios_base::trunc)));
  AddTarget(*_files.back());
#else
  _files.push_back(ofstream(BSSPOSIX_WCHAR(file),ios_base::out|ios_base::trunc));
  AddTarget(_files.back());
#endif
}
void BSS_FASTCALL bss_Log::AddTarget(const wchar_t* file)
{
  if(!file) return;
#ifdef BSS_COMPILER_GCC 
  _files.push_back(std::unique_ptr<std::ofstream>(new ofstream(BSSPOSIX_CHAR(file),ios_base::out|ios_base::trunc)));
  AddTarget(*_files.back());
#else
  _files.push_back(ofstream(BSSPOSIX_CHAR(file),ios_base::out|ios_base::trunc));
  AddTarget(_files.back());
#endif
}
void bss_Log::ClearTargets()
{
  if(_split!=0) _split->ClearTargets();
  for(size_t i = 0; i < _files.size(); ++i)
#ifdef BSS_COMPILER_GCC 
    _files[i]->close();
#else
    _files[i].close();
#endif
  _files.clear();
}
bool BSS_FASTCALL bss_Log::_writedatetime(long timez, std::ostream& log, bool timeonly)
{
  time_t rawtime;
  TIME64(&rawtime);
  tm stm;
  tm* ptm=&stm;
  if(GMTIMEFUNC(&rawtime, ptm)!=0)
    return false;

  long m=ptm->tm_hour*60 + ptm->tm_min + 1440 + timez; //+1440 ensures this is never negative because % does not properly respond to negative numbers.
  long h=((m/60)%24);
  m%=60;

  char logchar = log.fill();
  std::streamsize logwidth = log.width();
  log.fill('0');
  //Write date if we need to
  if(!timeonly) log << std::setw(4) << ptm->tm_year+1900 << std::setw(1) << '-' << std::setw(2) << ptm->tm_mon+1 << std::setw(1) << '-' << std::setw(2) << ptm->tm_mday << std::setw(1) << ' ';
  //Write time and flush stream
  log << std::setw(1) << h << std::setw(0) << ':' << std::setw(2) << m << std::setw(0) << ':' << std::setw(2) << ptm->tm_sec;
  
  //Reset values of stream
  log.width(logwidth);
  log.fill(logchar);

  return true;
}
bss_Log& bss_Log::operator=(const bss_Log& right)
{
//  ClearTargets();
//  *_split=*right._split;
  assert(false);
  return *this; //We don't copy _stream because its attached to _split which gets copied anyway and we want it to stay that way.
}
bss_Log& bss_Log::operator=(bss_Log&& right)
{
  _tz=GetTimeZoneMinutes();
  _files=std::move(right._files);
  _backup=std::move(right._backup);
  _split=right._split;
  right._split=0;
#ifdef BSS_COMPILER_GCC
  _stream.~basic_ostream(); // This stupid craziness is required due to GCC not implementing move constructors for streams like its supposed to >C
  new(&_stream) ostream(_split);
#else
  _stream=std::move(right._stream);
#endif
  return *this;
}
const char* BSS_FASTCALL bss_Log::_trimpath(const char* path)
{
	const char* r=strrchr(path,'/');
	const char* r2=strrchr(path,'\\');
  r=bssmax(r,r2);
  return (!r)?path:(r+1);
}
//*/
//const wchar_t* BSS_FASTCALL bss_Log::_trimpath(const wchar_t* path)
//{
//	const wchar_t* retval=wcsrchr(path,'/');
//	if(!retval)
//	{
//		retval=wcsrchr(path,'\\');
//		if(!retval) retval=path;
//		else ++retval;
//	}
//	else
//	{
//		path=wcsrchr(path,'\\');
//		retval=path>retval?++path:++retval;
//	}
//	return retval;
//}