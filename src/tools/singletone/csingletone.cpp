#include "tools/singletone/csingletone.h"
#include <iostream>
#include <stdlib.h>
#include "st.h"

namespace osrvd
{

#ifdef OSRVD_DEBUG
unsigned int CSingleTone::debug_counter = 0;
#endif

CSingleTone &CSingleTone::instance()
{
    static CSingleTone instance;
    return instance;
}

const char *CSingleTone::applicationName() const
{
    return "onvif_srvd";
}

void CSingleTone::destruct()
{
    delete this;
}

void CSingleTone::quit(const std::string &message)
{
    if(!message.empty())
    {
        OSRVD_LOG_ERR("Exit reason: " << message);
        ::exit(EXIT_FAILURE);
    }
    ::exit(0);
}

CSingleTone::CSingleTone()
{
    OSRVD_STDOUT_LOG("+singletone+");
}

CSingleTone::~CSingleTone()
{
    OSRVD_STDOUT_LOG("-singletone-");
}

}
