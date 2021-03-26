#ifndef CEthernetDevice_H
#define CEthernetDevice_H

#include <string>
#include <stdint.h>
#include <net/if.h>

namespace osrvd
{

class CEthernetDevice
{
public:
    CEthernetDevice();
    virtual ~CEthernetDevice();
    std::string ip();
    std::string ipMask();
    std::string interfaceName();
    std::string interfaceHardwareAddress();
    int ipMaskPrefix();

    bool check();

private:
    std::string m_interfaceName;
    std::string m_interfaceHardwareAddress;
    std::string m_ipMask;
    int m_ipMaskPrefix;

    void mineInterface();
};

}
#endif // CEthernetDevice_H
