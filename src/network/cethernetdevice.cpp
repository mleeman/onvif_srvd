//#define _GNU_SOURCE     /* To get defns of NI_MAXSERV and NI_MAXHOST */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if_link.h>
#include <sys/ioctl.h>

#include "cethernetdevice.h"
#include "st.h"

namespace osrvd
{

CEthernetDevice::CEthernetDevice()
{
    m_ipMaskPrefix = 0;
    mineInterface();
}

CEthernetDevice::~CEthernetDevice()
{}

std::string CEthernetDevice::ip()
{
    return OSRVD_ST.options()["network"].get("ip", "127.0.0.1").asString();
}

std::string CEthernetDevice::ipMask()
{
    return m_ipMask;
}

std::string CEthernetDevice::interfaceName()
{
    return m_interfaceName;
}

std::string CEthernetDevice::interfaceHardwareAddress()
{
    return m_interfaceHardwareAddress;
}

int CEthernetDevice::ipMaskPrefix()
{
    return m_ipMaskPrefix;
}

bool CEthernetDevice::check()
{
    return !interfaceName().empty() &&
           !interfaceHardwareAddress().empty() &&
           !ipMask().empty() &&
           m_ipMaskPrefix;
}

void CEthernetDevice::mineInterface()
{
    struct ifaddrs *ifaddr;
    int family, s, ifRequestFd;
    char host[NI_MAXHOST];
    char buff[3];
    char mask[INET_ADDRSTRLEN];
    uint32_t maskPrefix = 0;
    int prefix = 0;
    struct ifreq ifRequest;
    std::string ifName;
    std::string ifAddr;
    std::string hwAddress;

    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        OSRVD_LOG_ERR("Search interface name failed: getifaddrs failed");
    }

    for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL) { continue; }
        family = ifa->ifa_addr->sa_family;
        ifName = ifa->ifa_name;
        if (family == AF_INET || family == AF_INET6)
        {
            s = getnameinfo(ifa->ifa_addr,
                    (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                          sizeof(struct sockaddr_in6),
                    host, NI_MAXHOST,
                    NULL, 0, NI_NUMERICHOST);
            if (s != 0)
            {
                OSRVD_LOG_ERR("Search interface name failed: " << gai_strerror(s));
            }
            ifAddr = host;
            OSRVD_LOG_DBG("Candidate to interface name of address " << ifAddr << ": " << ifName);
            if(ifAddr == ip())
            {
                OSRVD_LOG_DBG("Interface of address " << ip() << " found: " << ifName);
                freeifaddrs(ifaddr);
                m_interfaceName = ifName;

                // hw addr
                int ifRequestFd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
                strcpy(ifRequest.ifr_name, interfaceName().c_str());
                if (0 == ioctl(ifRequestFd, SIOCGIFHWADDR, &ifRequest))
                {
                  int i;
                  for (i = 0; i < 6; ++i)
                  {
                    sprintf(buff, "%02x", (unsigned char) ifRequest.ifr_addr.sa_data[i]);
                    hwAddress += std::string(buff) + std::string(i < 5 ? ":" : "");
                  }
                  OSRVD_LOG_DBG("Interface " << interfaceName() << " hardware address: " << hwAddress);
                  m_interfaceHardwareAddress = hwAddress;
                }
                else { /*err*/ }
                if (0 == ioctl(ifRequestFd, SIOCGIFNETMASK, &ifRequest))
                {
                    struct sockaddr_in* addr = (struct sockaddr_in*)&ifRequest.ifr_addr;
                    if(inet_ntop(AF_INET, &addr->sin_addr, mask, INET_ADDRSTRLEN))
                    {
                        m_ipMask = std::string(mask);
                        OSRVD_LOG_DBG("Mask of ip " << ip() << ": " << m_ipMask);

                        maskPrefix = ntohl(addr->sin_addr.s_addr);
                        while(maskPrefix != 0)
                        {
                            maskPrefix <<= 1;
                            prefix++;
                        }
                        m_ipMaskPrefix = prefix;
                        OSRVD_LOG_DBG("Prefix of ip " << ip() << ": " << m_ipMaskPrefix);

                    } else { /* err */ }
                }
                else { /*err*/ }
                return;
            }
        }
    }
    freeifaddrs(ifaddr);
    OSRVD_LOG_ERR("Can not find interface name if address " << ip());
}

//void CEthernetDevice::mineInterfaceHardwareAddress()
//{
//    std::string hwAddress;
//    struct ifreq s;
//    int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
//    char buff[3];

//    strcpy(s.ifr_name, interfaceName().c_str());
//    if (0 == ioctl(fd, SIOCGIFHWADDR, &s))
//    {
//      int i;
//      for (i = 0; i < 6; ++i)
//      {
//        sprintf(buff, "%02x", (unsigned char) s.ifr_addr.sa_data[i]);
//        hwAddress += std::string(buff) + std::string(i < 5 ? ":" : "");
//      }
//      OSRVD_LOG_DBG("Interface " << interfaceName() << " hardware address: " << hwAddress);
//      m_interfaceHardwareAddress = hwAddress;
//    }
//}

}
