#include "clogger.h"
#include "tools/tools_string.h"
#include "tools/tools_fs.h"
#include "version.h"
#include "st.h"

#include <iomanip>
#include <thread>

#define OSRVD_LOG_DIR "logs"

namespace osrvd
{
namespace tools
{
namespace logger
{

CLogger::CLogger()
{
    std::string logPath = std::string(OSRVD_LOG_DIR "/" OSRVD_NAME "/");
    if (!osrvd::tools::fs::file_exists(logPath)) { osrvd::tools::fs::create_path(logPath); }
    m_filepart = logPath + osrvd::tools::string::formatted_current_datetime("%Y-%m-%d_%H-%M-%S");
    m_filename = m_filepart + ".application.log";
    start() << "Opening log"; stop();
}

CLogger::~CLogger()
{
    start() << "Closing log"; stop();
}

const std::string &CLogger::logFilePart() const
{
    return m_filepart;
}


#define OSRVD_COUT(_what) \
    m_logstream << _what; \
    std::cout << _what;

#define OSRVD_COUT_SS(_what) \
    std::stringstream ss; \
    ss << _what; \
    write(ss);

#define OSRVD_COUT_RET(_what)       OSRVD_COUT(_what); return *this;
#define OSRVD_COUT_SS_RET(_what) OSRVD_COUT_SS(_what); return *this;

#define OSRVD_COUT_SS_NUMFORMAT std::fixed << std::setprecision(2)

CLogger &CLogger::start()
{
    m_mutex.lock();
    #ifdef OSRVD_DEBUG
    if(m_logstream.is_open())
    {
        OSRVD_COUT("---------------------------[You forgot close logstream on finish, bastard!]---------------------------");
        stop();
    }
    #endif
    m_logstream.open(m_filename, std::ofstream::out | std::ofstream::ate | std::ofstream::app);
    std::string time = osrvd::tools::string::formatted_current_datetime("[%Y-%m-%d %H:%M:%S] ");
    #ifdef OSRVD_DEBUG
        OSRVD_COUT_RET(time << "{" << std::this_thread::get_id() << "} ");
    #else
        OSRVD_COUT_RET(time);
    #endif
}

void CLogger::stop()
{
    OSRVD_COUT(std::endl);
//    m_logstream->flush();
    m_logstream.close();
    m_mutex.unlock();
}

void CLogger::write(const std::stringstream &value) { OSRVD_COUT(value.str()); }


#define OSRVD_FUNC_LOGGER_OP_SIMPLE(_type) CLogger &CLogger::operator<<(_type value)

OSRVD_FUNC_LOGGER_OP_SIMPLE(const bool &)            { std::string sBool = std::string(value?"true":"false"); OSRVD_COUT_RET(sBool); }


#define OSRVD_SWITCH_HELPER(_value) case _value: { OSRVD_COUT(#_value) } break;
#define OSRVD_SWITCH_HELPER_DEFAULT default: { OSRVD_COUT("Something missing") } break;
#define OSRVD_FUNC_LOGGER_OP_SWITCH_SIMPLE_BEGIN(_type) OSRVD_FUNC_LOGGER_OP_SIMPLE(_type) { switch(value) {
#define OSRVD_FUNC_LOGGER_OP_SWITCH_SIMPLE_END OSRVD_SWITCH_HELPER_DEFAULT } return *this; }


}
}
}
