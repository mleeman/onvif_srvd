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
    int gniResult;
    char host[NI_MAXHOST];


    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        OSRVD_LOG_ERR("Search interface name failed: getifaddrs failed: " << strerror(errno));
    }

    for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL) { continue; }
        if (ifa->ifa_addr->sa_family == AF_INET)
        {
            gniResult = getnameinfo(ifa->ifa_addr,
                    sizeof(struct sockaddr_in),
                    host, NI_MAXHOST,
                    NULL, 0, NI_NUMERICHOST);
            if (gniResult == 0)
            {
                OSRVD_LOG_DBG("Candidate to interface name of address " << host << ": " << ifa->ifa_name);
                if(std::string(host) == ip())
                {
                    freeifaddrs(ifaddr);

                    setInterfaceName(ifa->ifa_name);
                    mineInterfaceHardwareAddress();
                    mineIpMask();

                    return;
                }
            }
            else { OSRVD_LOG_ERR("Search interface name failed: " << gai_strerror(gniResult)) };
        }
    }
    freeifaddrs(ifaddr);
    OSRVD_LOG_ERR("Can not find interface name if address " << ip());
}

void CEthernetDevice::mineInterfaceHardwareAddress()
{
    struct ifreq ifRequest;
    int ifRequestFd, ioctlResult;
    char buff[3];
    std::string hwAddress;

    ifRequestFd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    strcpy(ifRequest.ifr_name, interfaceName().c_str());
    strcpy(ifRequest.ifr_addr.sa_data, ip().c_str());
    ioctlResult == ioctl(ifRequestFd, SIOCGIFHWADDR, &ifRequest);

    if (ioctlResult != 0)
    {
      int i;
      for (i = 0; i < 6; ++i)
      {
        sprintf(buff, "%02x", (unsigned char) ifRequest.ifr_addr.sa_data[i]);
        hwAddress += std::string(buff) + std::string(i < 5 ? ":" : "");
      }
      setInterfaceHardwareAddress(hwAddress);
    }
    else { OSRVD_LOG_ERR("Search interface hardware address failed: " << strerror(errno)); }
}

void CEthernetDevice::mineIpMask()
{
    struct ifreq ifRequest;
    int ifRequestFd, ioctlResult;
    char mask[INET_ADDRSTRLEN];
    int maskPrefix, prefix = 0;

    ifRequestFd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    strcpy(ifRequest.ifr_name, interfaceName().c_str());
    strcpy(ifRequest.ifr_addr.sa_data, ip().c_str());
    ioctlResult = ioctl(ifRequestFd, SIOCGIFNETMASK, &ifRequest);

    if (ioctlResult == 0)
    {
        struct sockaddr_in* addr = (struct sockaddr_in*)&ifRequest.ifr_addr;
        if(inet_ntop(AF_INET, &addr->sin_addr, mask, INET_ADDRSTRLEN))
        {
            setIpMask(mask);

            maskPrefix = ntohl(addr->sin_addr.s_addr);
            while(maskPrefix != 0)
            {
                maskPrefix <<= 1;
                prefix++;
            }
            setIpMaskPrefix(prefix);
        }
        else { OSRVD_LOG_ERR("Search interface mask failed: " << strerror(errno)); }
    }
    else { OSRVD_LOG_ERR("Search interface mask failed: " << strerror(errno)); }
}

void CEthernetDevice::setInterfaceName(const std::string &ifName)
{
    m_interfaceName = ifName;
    OSRVD_LOG_NFO("Interface of address " << ip() << " found: " << m_interfaceName);
}

void CEthernetDevice::setInterfaceHardwareAddress(const std::string &hwAddress)
{
    m_interfaceHardwareAddress = hwAddress;
    OSRVD_LOG_NFO("Interface " << interfaceName() << " hardware address: " << m_interfaceHardwareAddress);
}

void CEthernetDevice::setIpMask(const std::string &mask)
{
    m_ipMask = mask;
    OSRVD_LOG_NFO("Mask of ip " << ip() << ": " << m_ipMask);
}

void CEthernetDevice::setIpMaskPrefix(const int &maskPrefix)
{
    m_ipMaskPrefix = maskPrefix;
    OSRVD_LOG_NFO("Prefix of ip " << ip() << ": " << m_ipMaskPrefix);
}

}
