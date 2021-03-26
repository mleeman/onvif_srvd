#ifndef SOAP_DEFINES_H
#define SOAP_DEFINES_H

#include "st.h"
#include "networking.h"

#define OSRVD_UNUSED(_argument) (void)_argument

#define OSRVD_SOAP_REQUEST_VARIABLE(_method, _parametr_prefix) _parametr_prefix##__##_method
#define OSRVD_SOAP_RESPONSE_VARIABLE(_method, _parametr_prefix) _parametr_prefix##__##_method##Response

#define OSRVD_SOAP_METHOD_BEGIN(_method, _parametr_prefix, _class) \
    int _class::_method \
        ( \
        _##_parametr_prefix##__##_method           *OSRVD_SOAP_REQUEST_VARIABLE(_method, _parametr_prefix), \
        _##_parametr_prefix##__##_method##Response &OSRVD_SOAP_RESPONSE_VARIABLE(_method, _parametr_prefix) \
        ) \
    { \
        OSRVD_UNUSED(OSRVD_SOAP_REQUEST_VARIABLE(_method, _parametr_prefix));            \
        OSRVD_UNUSED(OSRVD_SOAP_RESPONSE_VARIABLE(_method, _parametr_prefix));  \
        OSRVD_LOG_DBG("SOAP method call: " #_class":"#_method " from " << osrvd::tools::networking::integerToIpAddr(OSRVD_SOAP_REQUEST_VARIABLE(_method, _parametr_prefix)->soap->ip) << ":" << OSRVD_SOAP_REQUEST_VARIABLE(_method, _parametr_prefix)->soap->port);

#define OSRVD_SOAP_METHOD_END return SOAP_OK; }

#define OSRVD_MASK_UNUSED_SOAP_CALL_DEFINE(_method, _parametr_prefix, _class) \
    OSRVD_SOAP_METHOD_BEGIN(_method, _parametr_prefix, _class) \
    OSRVD_LOG_DBG("Mocked. ["#_class":"#_method"]");  \
    return SOAP_FAULT; \
    OSRVD_SOAP_METHOD_END


#define OSRVD_SOAP_LOG_ERR(_soap, _message) \
    { \
        std::stringstream soap_ss; \
        soap_stream_fault(_soap, soap_ss); \
        OSRVD_LOG_ERR("[SOAP] " _message ": " << soap_ss.str()) \
    }

#define OSRVD_SOAP_CLEANUP(_soap) \
    soap_destroy(_soap); \
    soap_end(_soap);

#define OSRVD_SOAP_SERVICE_DISPATCH(_soap, _service) \
    if(_service->dispatch() != SOAP_NO_METHOD) { OSRVD_SOAP_CLEANUP(_soap); continue; }

#define OSRVD_SOAPSERVICE_SELF_SOAP this->soap
#define OSRVD_SOAP OSRVD_ST.soap()()

#endif // SOAP_DEFINES_H
