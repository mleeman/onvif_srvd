#include <arpa/inet.h>
#include <iomanip>
#include <sstream>
#include "ServiceContext.h"
#include "st.h"


ServiceContext::ServiceContext():
    tz_format(TZ_UTC_OFFSET)
{
}



std::string ServiceContext::get_time_zone() const
{
    #define HH_FORMAT   std::setfill('0') << std::internal << std::setw(3) << std::showpos
    #define MM_FORMAT   ':' << std::setw(2) << std::noshowpos

    std::ostringstream res;
    char* TZ_env;
    const time_t  timestamp = time(NULL);
    struct tm    *now       = localtime(&timestamp);


    //global var timezone is not adjusted for daylight saving!
    //In GNU programs it is better to use tm_gmtoff, since it contains the correct
    //offset even when it is not the latest one.
    int hh = now->tm_gmtoff/3600;
    int mm = abs(now->tm_gmtoff%60);

    switch (tz_format)
    {
        case TZ_UTC_OFFSET:
            res << HH_FORMAT << hh << MM_FORMAT << mm;
            return res.str();


        case TZ_GMT_OFFSET:
            res << HH_FORMAT << -hh << MM_FORMAT << mm;
            return res.str();


        case TZ_UTC:
            res << "UTC" << HH_FORMAT << hh << MM_FORMAT << mm;
            return res.str();


        case TZ_GMT:
            res << "GMT" << HH_FORMAT << -hh << MM_FORMAT << mm;
            return res.str();


        case TZ_ENV:
            TZ_env = getenv("TZ");
            return (TZ_env ? TZ_env : "UTC+00");


        default:
            TZ_env = getenv("TZ_ONVIF");
            return (TZ_env ? TZ_env : "UTC+00");
    }

    //Note: for GMT sign
    //According to the definition, GMT and UTC mean the same thing!
    //But when offsets are specified, the sign of the GMT is inverted!
    //This feature is described here: https://en.wikipedia.org/wiki/Tz_database#Area
}



tt__SystemDateTime* ServiceContext::get_SystemDateAndTime(soap* soap)
{
    const time_t  timestamp = time(NULL);
    struct tm    *time_info = localtime(&timestamp);

    auto res = soap_new_req_tt__SystemDateTime(soap,
                                               tt__SetDateTimeType__Manual,
                                               (time_info->tm_isdst > 0));

    if(res)
    {
        res->TimeZone      = soap_new_req_tt__TimeZone(soap, get_time_zone());
        res->LocalDateTime = get_DateTime(soap, time_info);
        res->UTCDateTime   = get_DateTime(soap, gmtime(&timestamp));
    }

    return res;

    //About tm_isdst see https://man7.org/linux/man-pages//man3/ctime.3.html
}



tt__DateTime* ServiceContext::get_DateTime(soap* soap, tm* time_info)
{
    return soap_new_req_tt__DateTime(
               soap,
               soap_new_req_tt__Time(
                   soap,
                   time_info->tm_hour,
                   time_info->tm_min,
                   time_info->tm_sec
               ),
               soap_new_req_tt__Date(
                   soap,
                   time_info->tm_year+1900,
                   time_info->tm_mon+1,
                   time_info->tm_mday
               )
           );
}



bool ServiceContext::set_tz_format(const char* new_val)
{
    std::istringstream ss(new_val);
    unsigned int tmp_val;
    ss >> tmp_val;


    if( tmp_val >= TZ_CNT_FORMATS )
    {
        str_err = "tz_format is not supported";
        return false;
    }


    tz_format = static_cast<TimeZoneForamt>(tmp_val);
    return true;
}



std::string ServiceContext::getServerIpFromClientIp(uint32_t client_ip) const
{
    char server_ip[INET_ADDRSTRLEN];


    if (eth_ifs.size() == 1)
    {
        eth_ifs[0].get_ip(server_ip);
        return server_ip;
    }

    for(size_t i = 0; i < eth_ifs.size(); ++i)
    {
        uint32_t if_ip, if_mask;
        eth_ifs[i].get_ip(&if_ip);
        eth_ifs[i].get_mask(&if_mask);

        if( (if_ip & if_mask) == (client_ip & if_mask) )
        {
            eth_ifs[i].get_ip(server_ip);
            return server_ip;
        }
    }


    return "127.0.0.1";  //localhost
}



std::string ServiceContext::getXAddr(soap *soap) const
{
    std::ostringstream os;

    os << "http://" << getServerIpFromClientIp(htonl(soap->ip)) << ":" << port;

    return os.str();
}



bool ServiceContext::add_profile(const StreamProfile &profile)
{
    if( !profile.is_valid() )
    {
        str_err = "profile has unset parameters";
        return false;
    }


    if( profiles.find(profile.get_name()) != profiles.end() )
    {
        str_err = "profile: " + profile.get_name() +  " already exist";
        return false;
    }


    profiles[profile.get_name()] = profile;
    return true;
}



std::string ServiceContext::get_stream_uri(const std::string &profile_url, uint32_t client_ip) const
{
    std::string uri(profile_url);
    std::string template_str("%s");


    auto it = uri.find(template_str, 0);

    if( it != std::string::npos )
        uri.replace(it, template_str.size(), getServerIpFromClientIp(client_ip));


    return uri;
}



std::string ServiceContext::get_snapshot_uri(const std::string &profile_url, uint32_t client_ip) const
{
    std::string uri(profile_url);
    std::string template_str("%s");


    auto it = uri.find(template_str, 0);

    if( it != std::string::npos )
        uri.replace(it, template_str.size(), getServerIpFromClientIp(client_ip));


    return uri;
}



tds__DeviceServiceCapabilities *ServiceContext::getDeviceServiceCapabilities(soap *soap)
{
    tds__DeviceServiceCapabilities *capabilities = soap_new_tds__DeviceServiceCapabilities(soap);

    capabilities->Network = soap_new_tds__NetworkCapabilities(soap);
    capabilities->Network->IPFilter            = osrvd::CSoap::valuePointer(soap, false);
    capabilities->Network->ZeroConfiguration   = osrvd::CSoap::valuePointer(soap, false);
    capabilities->Network->IPVersion6          = osrvd::CSoap::valuePointer(soap, false);
    capabilities->Network->DynDNS              = osrvd::CSoap::valuePointer(soap, false);
    capabilities->Network->Dot11Configuration  = osrvd::CSoap::valuePointer(soap, false);
    capabilities->Network->Dot1XConfigurations = osrvd::CSoap::valuePointer(soap, 0);
    capabilities->Network->HostnameFromDHCP    = osrvd::CSoap::valuePointer(soap, false);
    capabilities->Network->NTP                 = osrvd::CSoap::valuePointer(soap, 0);
    capabilities->Network->DHCPv6              = osrvd::CSoap::valuePointer(soap, false);

    capabilities->Security = soap_new_tds__SecurityCapabilities(soap);
    capabilities->Security->TLS1_x002e0          = osrvd::CSoap::valuePointer(soap, false);
    capabilities->Security->TLS1_x002e1          = osrvd::CSoap::valuePointer(soap, false);
    capabilities->Security->TLS1_x002e2          = osrvd::CSoap::valuePointer(soap, false);
    capabilities->Security->OnboardKeyGeneration = osrvd::CSoap::valuePointer(soap, false);
    capabilities->Security->AccessPolicyConfig   = osrvd::CSoap::valuePointer(soap, false);
    capabilities->Security->DefaultAccessPolicy  = osrvd::CSoap::valuePointer(soap, false);
    capabilities->Security->Dot1X                = osrvd::CSoap::valuePointer(soap, false);
    capabilities->Security->RemoteUserHandling   = osrvd::CSoap::valuePointer(soap, false);
    capabilities->Security->X_x002e509Token      = osrvd::CSoap::valuePointer(soap, false);
    capabilities->Security->SAMLToken            = osrvd::CSoap::valuePointer(soap, false);
    capabilities->Security->KerberosToken        = osrvd::CSoap::valuePointer(soap, false);
    capabilities->Security->UsernameToken        = osrvd::CSoap::valuePointer(soap, false);
    capabilities->Security->HttpDigest           = osrvd::CSoap::valuePointer(soap, false);
    capabilities->Security->RELToken             = osrvd::CSoap::valuePointer(soap, false);
    capabilities->Security->MaxUsers             = osrvd::CSoap::valuePointer(soap, 0);
    capabilities->Security->MaxUserNameLength    = osrvd::CSoap::valuePointer(soap, 0);
    capabilities->Security->MaxPasswordLength    = osrvd::CSoap::valuePointer(soap, 0);

    capabilities->System = soap_new_tds__SystemCapabilities(soap);
    capabilities->System->DiscoveryResolve       = osrvd::CSoap::valuePointer(soap, true);
    capabilities->System->DiscoveryBye           = osrvd::CSoap::valuePointer(soap, true);
    capabilities->System->RemoteDiscovery        = osrvd::CSoap::valuePointer(soap, true);
    capabilities->System->SystemBackup           = osrvd::CSoap::valuePointer(soap, false);
    capabilities->System->SystemLogging          = osrvd::CSoap::valuePointer(soap, false);
    capabilities->System->FirmwareUpgrade        = osrvd::CSoap::valuePointer(soap, false);
    capabilities->System->HttpFirmwareUpgrade    = osrvd::CSoap::valuePointer(soap, false);
    capabilities->System->HttpSystemBackup       = osrvd::CSoap::valuePointer(soap, false);
    capabilities->System->HttpSystemLogging      = osrvd::CSoap::valuePointer(soap, false);
    capabilities->System->HttpSupportInformation = osrvd::CSoap::valuePointer(soap, false);
    capabilities->System->StorageConfiguration   = osrvd::CSoap::valuePointer(soap, false);

    return capabilities;
}



trt__Capabilities *ServiceContext::getMediaServiceCapabilities(soap *soap)
{
    trt__Capabilities *capabilities = soap_new_trt__Capabilities(soap);

    auto profiles = this->get_profiles();
    for( auto it = profiles.cbegin(); it != profiles.cend(); ++it ) {
        if (( !it->second.get_snapurl().empty() ) && ( capabilities->SnapshotUri == NULL )) {
            capabilities->SnapshotUri = osrvd::CSoap::valuePointer(soap, true);
        }
    }

    capabilities->ProfileCapabilities = soap_new_trt__ProfileCapabilities(soap);
    capabilities->ProfileCapabilities->MaximumNumberOfProfiles = osrvd::CSoap::valuePointer(soap, 1);

    capabilities->StreamingCapabilities = soap_new_trt__StreamingCapabilities(soap);
    capabilities->StreamingCapabilities->RTPMulticast = osrvd::CSoap::valuePointer(soap, false);


    return capabilities;
}



tptz__Capabilities *ServiceContext::getPTZServiceCapabilities(soap *soap)
{
    tptz__Capabilities *capabilities = soap_new_tptz__Capabilities(soap);

    return capabilities;
}
