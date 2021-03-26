#ifndef SERVICEMEDIA_H
#define SERVICEMEDIA_H

#include "onvifMediaBindingService.h"
namespace osrvd
{
namespace soap
{

class CMedia : public MediaBindingService
{
public:
    CMedia();
    virtual ~CMedia();
    trt__Capabilities*  serviceCapabilities();
};

}
}
#endif // SERVICEMEDIA_H
