#ifndef CSINGLETONE_H
#define CSINGLETONE_H
#include "tools/logger/clogger.h"
#include "tools/options/options.h"
#include "csoap.h"
#include "defines/singletone.h"
#include "ccameras.h"
#include "cethernetdevice.h"

namespace osrvd
{

class CSingleTone
{
    OSRVD_SIMPLE_SINGLTON_DECLARATION(osrvd::tools::logger::CLogger, logger)
    OSRVD_SIMPLE_SINGLTON_DECLARATION(osrvd::tools::options::Options, options)
    OSRVD_SIMPLE_SINGLTON_DECLARATION(osrvd::soap::CSoap, soap)
    OSRVD_SIMPLE_SINGLTON_DECLARATION(osrvd::entity::CCameras, camears)
    OSRVD_SIMPLE_SINGLTON_DECLARATION(osrvd::CEthernetDevice, eth)

public:
   static CSingleTone &instance();
   void destruct();
   void quit(const std::string &message = "");
   const char *applicationName() const;

private:
    CSingleTone();
    ~CSingleTone();

#ifdef OSRVD_DEBUG
public:
    static unsigned int debug_counter;
#endif
};

}
#endif // CSINGLETONE_H
