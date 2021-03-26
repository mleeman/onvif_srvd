#ifndef ST_H
#define ST_H
#include <stdexcept>
#include <string>
#include <chrono>
#include "csingletone.h"


#define OSRVD_STR_HELPER(x) #x
#define OSRVD_STR(x) OSRVD_STR_HELPER(x)

#define OSRVD_ST osrvd::CSingleTone::instance()

#define OSRVD_LOG_START(_what) OSRVD_ST.logger().start() << _what;
#define OSRVD_LOG_STOP         OSRVD_ST.logger().stop();
#define OSRVD_LOG(_what) do { OSRVD_LOG_START(_what); OSRVD_LOG_STOP; } while(false)
#define OSRVD_LOG_LINE(_caption) std::string(std::string(_caption " in file " __FILE__ " at line " OSRVD_STR(__LINE__) " in method ") + std::string(__func__))
#ifdef OSRVD_DEBUG
    #define OSRVD_LOG_DBG(_what) { OSRVD_LOG("[Debg] " << _what); }
    #define OSRVD_LOG_DBG_POINT  { OSRVD_LOG("[Debg] {" << OSRVD_ST.debug_counter++ << "} " << OSRVD_LOG_LINE("Point")); }
    #define OSRVD_STDOUT_LOG(_message) std::cout << "!:" << _message << ":!" << std::endl;
#else  // OSRVD_DEBUG
    #define OSRVD_LOG_DBG(_what)
    #define OSRVD_LOG_DBG_POINT
    #define OSRVD_STDOUT_LOG(_message)
#endif // OSRVD_DEBUG
#define OSRVD_LOG_NFO(_what) { OSRVD_LOG("[Info] " << _what); }
#define OSRVD_LOG_WRN(_what) { OSRVD_LOG("[Warn] " << _what); }
#define OSRVD_LOG_ERR(_what) { OSRVD_LOG("[Errr] " << _what); }
#define OSRVD_LOG_CRY(_what) { OSRVD_LOG("[Errr] " << _what); throw std::runtime_error(_what); }

#define OSRVD_NOT_ALL_ENUMS_THROW default: { OSRVD_LOG_CRY(OSRVD_LOG_LINE("Missed some enum members")); } break;


#endif // ST_H
