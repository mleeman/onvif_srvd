#include "csoap.h"
#include "soap_defines.h"
#include "st.h"

namespace osrvd
{
namespace soap
{

CSoap::CSoap()
{
    m_soap = soap_new();
    if(!m_soap)
    {
        OSRVD_ST.quit("Can't get mem for SOAP");
    }
    m_soap->bind_flags       = SO_REUSEADDR;
    m_soap->send_timeout     = OSRVD_ST.options()["network"].get("timeout", 3).asInt(); // timeout in sec
    m_soap->recv_timeout     = OSRVD_ST.options()["network"].get("timeout", 3).asInt(); // timeout in sec
    m_soap->transfer_timeout = OSRVD_ST.options()["network"].get("timeout", 3).asInt() * 4;

    m_device = nullptr;
    m_media  = nullptr;
    m_ptz    = nullptr;
}

CSoap::~CSoap()
{
    delete m_device;
    delete m_media;
    delete m_ptz;
    soap_free(m_soap);
}

bool CSoap::listen()
{
    m_soapSocket = soap_bind(m_soap, OSRVD_ST.eth().ip().c_str(), OSRVD_ST.options()["network"].get("port", 6688).asInt(), 10);
    if( !soap_valid_socket(m_soapSocket) )
    {
        OSRVD_SOAP_LOG_ERR(m_soap, "Can not listen")
        return false;
    }
    OSRVD_LOG_NFO("Listen at " << OSRVD_ST.eth().ip() << ":" << OSRVD_ST.options()["network"].get("port", 6688).asString());
    return true;
}

bool CSoap::run()
{
    m_device = new CDevice();
    m_media = new CMedia();
    m_ptz = new CPtz();
    if(listen())
    {
        while( soap_valid_socket(soap_accept(m_soap)) )
        {
            if( soap_begin_serve(m_soap) )
            {
                OSRVD_SOAP_LOG_ERR(m_soap, "Can not serve");
                OSRVD_SOAP_CLEANUP(m_soap);
                continue;
            }
            OSRVD_SOAP_SERVICE_DISPATCH(m_soap, m_ptz);
            OSRVD_SOAP_SERVICE_DISPATCH(m_soap, m_media);
            OSRVD_SOAP_SERVICE_DISPATCH(m_soap, m_device);
            if(m_soap->error)
            {
                soap_send_fault(m_soap);
                OSRVD_SOAP_CLEANUP(m_soap);
                continue;
            }
            OSRVD_LOG_WRN("Not realized entity");
            OSRVD_SOAP_CLEANUP(m_soap);
        }
        OSRVD_SOAP_LOG_ERR(m_soap, "Can not accept connection")
    }
    return false;
}

std::string CSoap::serverUri()
{
    return "http://" + OSRVD_ST.eth().ip() + ":" + OSRVD_ST.options()["network"].get("port", 6688).asString();
}

/*
APPLY(ImagingBindingService, soap)               \
APPLY(PTZBindingService, soap)                   \
APPLY(RecordingBindingService, soap)             \
APPLY(ReplayBindingService, soap)                \
APPLY(SearchBindingService, soap)                \
APPLY(ReceiverBindingService, soap)              \
APPLY(DisplayBindingService, soap)               \
APPLY(EventBindingService, soap)                 \
APPLY(PullPointSubscriptionBindingService, soap) \
APPLY(NotificationProducerBindingService, soap)  \
APPLY(SubscriptionManagerBindingService, soap)   \
*/



}
}
