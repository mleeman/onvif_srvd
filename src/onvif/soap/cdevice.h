#ifndef SERVICEDEVICE_H
#define SERVICEDEVICE_H

#include "onvifDeviceBindingService.h"
namespace osrvd
{
namespace soap
{

class CDevice : public DeviceBindingService
{
public:
    CDevice();
    virtual ~CDevice();
    tds__DeviceServiceCapabilities *serviceCapabilities();
    tt__SystemDateTime *dateTime();

private:
    std::string timeZone() const;
    tt__DateTime *dateTime(tm *time_info);
};

}
}
#endif // SERVICEDEVICE_H
