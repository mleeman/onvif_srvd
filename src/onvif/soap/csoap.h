#ifndef CSOAP_H
#define CSOAP_H
#include "onvifH.h"
#include "cdevice.h"
#include "cmedia.h"
#include "cptz.h"
namespace osrvd
{
namespace soap
{

class CSoap
{
public:
    CSoap();
    virtual ~CSoap();
    struct soap* operator()() { return m_soap; }
    bool run();

    CDevice *device() { return m_device; }
    CMedia *media() { return m_media; }
    CPtz *ptz() { return m_ptz; }

    std::string serverUri();
private:
    bool listen();

    SOAP_SOCKET m_soapSocket;
    struct soap *m_soap;
    CDevice *m_device;
    CMedia *m_media;
    CPtz *m_ptz;
};

template<typename T> T *valuePointer(struct soap *soap, T value)
{
    T* ptr = (T*)soap_malloc(soap, sizeof(T));
    *ptr = value;

    return ptr;
}

}
}

#endif // CSOAP_H
