#ifndef CLOGGER_H
#define CLOGGER_H
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <mutex>

namespace osrvd
{
namespace tools
{
namespace logger
{

#define OSRVD_DECLARE_LOGGER_OP_SIMPLE(_type) CLogger &operator<<(_type value)

class CLogger
{
    // CLogger(const CLogger &) = delete;

public:
    CLogger();
    ~CLogger();

    CLogger &start();
    void      stop();
    template <class T> CLogger &operator<<(const T &value) { m_logstream << value; std::cout << value; return *this; }
    const std::string &logFilePart() const;

    OSRVD_DECLARE_LOGGER_OP_SIMPLE(const bool &);

private:
    std::string   m_filename;
    std::string   m_filepart;
    std::ofstream m_logstream;
    std::mutex    m_mutex;
    void write(const std::stringstream &value);
};

}
}
}
#endif // CLOGGER_H
