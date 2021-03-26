#ifndef SERVICEPTZ_H
#define SERVICEPTZ_H

#include "onvifPTZBindingService.h"

namespace osrvd
{
namespace soap
{

class CPtz : public PTZBindingService
{
public:
    CPtz();
    virtual ~CPtz();
    tptz__Capabilities *serviceCapabilities();
};

}
}

#endif // SERVICEPTZ_H
