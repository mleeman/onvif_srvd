#include "cdevice.h"
#include <ctime>
#include <iomanip>
#include "soap_defines.h"
#include "st.h"
#include "version.h"

namespace osrvd
{
namespace soap
{

CDevice::CDevice() : DeviceBindingService(OSRVD_SOAP)
{}

CDevice::~CDevice()
{}

tds__DeviceServiceCapabilities *CDevice::serviceCapabilities()
{
    tds__DeviceServiceCapabilities *capabilities = soap_new_tds__DeviceServiceCapabilities(OSRVD_SOAP);

    capabilities->Network = soap_new_tds__NetworkCapabilities(OSRVD_SOAP);
    capabilities->Network->IPFilter            = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->Network->ZeroConfiguration   = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->Network->IPVersion6          = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->Network->DynDNS              = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->Network->Dot11Configuration  = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->Network->Dot1XConfigurations = osrvd::soap::valuePointer(OSRVD_SOAP, 0);
    capabilities->Network->HostnameFromDHCP    = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->Network->NTP                 = osrvd::soap::valuePointer(OSRVD_SOAP, 0);
    capabilities->Network->DHCPv6              = osrvd::soap::valuePointer(OSRVD_SOAP, false);

    capabilities->Security = soap_new_tds__SecurityCapabilities(OSRVD_SOAP);
    capabilities->Security->TLS1_x002e0          = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->Security->TLS1_x002e1          = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->Security->TLS1_x002e2          = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->Security->OnboardKeyGeneration = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->Security->AccessPolicyConfig   = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->Security->DefaultAccessPolicy  = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->Security->Dot1X                = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->Security->RemoteUserHandling   = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->Security->X_x002e509Token      = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->Security->SAMLToken            = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->Security->KerberosToken        = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->Security->UsernameToken        = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->Security->HttpDigest           = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->Security->RELToken             = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->Security->MaxUsers             = osrvd::soap::valuePointer(OSRVD_SOAP, 1);
    capabilities->Security->MaxUserNameLength    = osrvd::soap::valuePointer(OSRVD_SOAP, static_cast<int>(OSRVD_ST.options()["network"]["user"].asString().length() + 1));
    capabilities->Security->MaxPasswordLength    = osrvd::soap::valuePointer(OSRVD_SOAP, static_cast<int>(OSRVD_ST.options()["network"]["password"].asString().length() + 1));

    capabilities->System = soap_new_tds__SystemCapabilities(OSRVD_SOAP);
    capabilities->System->DiscoveryResolve       = osrvd::soap::valuePointer(OSRVD_SOAP, true);
    capabilities->System->DiscoveryBye           = osrvd::soap::valuePointer(OSRVD_SOAP, true);
    capabilities->System->RemoteDiscovery        = osrvd::soap::valuePointer(OSRVD_SOAP, true);
    capabilities->System->SystemBackup           = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->System->SystemLogging          = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->System->FirmwareUpgrade        = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->System->HttpFirmwareUpgrade    = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->System->HttpSystemBackup       = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->System->HttpSystemLogging      = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->System->HttpSupportInformation = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->System->StorageConfiguration   = osrvd::soap::valuePointer(OSRVD_SOAP, false);

    return capabilities;
}

tt__SystemDateTime* CDevice::dateTime()
{
    const time_t  timestamp = time(NULL);
    struct tm    *time_info = localtime(&timestamp);

    tt__SystemDateTime *dt = soap_new_req_tt__SystemDateTime(OSRVD_SOAP,
                                                             tt__SetDateTimeType::Manual,
                                                             (time_info->tm_isdst > 0));

    if(dt)
    {
        dt->TimeZone      = soap_new_req_tt__TimeZone(OSRVD_SOAP, timeZone());
        dt->LocalDateTime = dateTime(time_info);
        dt->UTCDateTime   = dateTime(gmtime(&timestamp));
    }

    return dt;

    //About tm_isdst see https://man7.org/linux/man-pages//man3/ctime.3.html
}

#define HH_FORMAT   std::setfill('0') << std::internal << std::setw(3) << std::showpos
#define MM_FORMAT   ':' << std::setw(2) << std::noshowpos
std::string CDevice::timeZone() const
{
    std::ostringstream res;
    char* TZ_env;
    const time_t  timestamp = time(NULL);
    struct tm    *now       = localtime(&timestamp);

    int hh = now->tm_gmtoff/3600;
    int mm = abs(now->tm_gmtoff%60);

    res << HH_FORMAT << hh << MM_FORMAT << mm;
    return res.str();
}

tt__DateTime* CDevice::dateTime(tm* time_info)
{
    return soap_new_req_tt__DateTime(
               OSRVD_SOAP,
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

}
}

#define REQUEST_VARIABLE(_method)  OSRVD_SOAP_REQUEST_VARIABLE(_method, tds)
#define RESPONSE_VARIABLE(_method) OSRVD_SOAP_RESPONSE_VARIABLE(_method, tds)
#define SOAP_METHOD_BEGIN(_method) OSRVD_SOAP_METHOD_BEGIN(_method, tds, DeviceBindingService)
#define SOAP_METHOD_END OSRVD_SOAP_METHOD_END
#define MASK_UNUSED_SOAP_CALL(_method) OSRVD_MASK_UNUSED_SOAP_CALL_DEFINE(_method, tds, DeviceBindingService)

/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(GetServices)
{
    tds__Service *deviceService = soap_new_tds__Service(OSRVD_SOAP);
    deviceService->Namespace    = "http://www.onvif.org/ver10/device/wsdl";
    deviceService->XAddr        = OSRVD_ST.soap().serverUri();
    deviceService->Version      = soap_new_req_tt__OnvifVersion(OSRVD_SOAP, 2, 5);
    if( REQUEST_VARIABLE(GetServices)->IncludeCapability )
    {
        deviceService->Capabilities                  = soap_new__tds__Service_Capabilities(OSRVD_SOAP);
        tds__DeviceServiceCapabilities *capabilities = OSRVD_ST.soap().device()->serviceCapabilities();
        deviceService->Capabilities->__any           = soap_dom_element(OSRVD_SOAP, NULL, "tds:Capabilities", capabilities, capabilities->soap_type());
    }
    RESPONSE_VARIABLE(GetServices).Service.push_back(deviceService);

    tds__Service *mediaService = soap_new_tds__Service(OSRVD_SOAP);
    mediaService->Namespace    = "http://www.onvif.org/ver10/media/wsdl";
    mediaService->XAddr        = OSRVD_ST.soap().serverUri();
    mediaService->Version      = soap_new_req_tt__OnvifVersion(OSRVD_SOAP, 2, 6);
    if (REQUEST_VARIABLE(GetServices)->IncludeCapability)
    {
        mediaService->Capabilities        = soap_new__tds__Service_Capabilities(OSRVD_SOAP);
        trt__Capabilities *capabilities   = OSRVD_ST.soap().media()->serviceCapabilities();
        mediaService->Capabilities->__any = soap_dom_element(OSRVD_SOAP, NULL, "trt:Capabilities", capabilities, capabilities->soap_type());
    }
    RESPONSE_VARIABLE(GetServices).Service.push_back(mediaService);


//    if (OSRVD_ST.options()["ptz"]["enabled"].asBool())
//    {
//        tds__GetServicesResponse.Service.push_back(soap_new_tds__Service(OSRVD_SOAP));
//        tds__GetServicesResponse.Service.back()->Namespace  = "http://www.onvif.org/ver20/ptz/wsdl";
//        tds__GetServicesResponse.Service.back()->XAddr      = XAddr;
//        tds__GetServicesResponse.Service.back()->Version    = soap_new_req_tt__OnvifVersion(OSRVD_SOAP, 2, 4);
//        if (tds__GetServices->IncludeCapability)
//        {
//            tds__GetServicesResponse.Service.back()->Capabilities        = soap_new__tds__Service_Capabilities(OSRVD_SOAP);
//            tptz__Capabilities *capabilities                             = ctx->getPTZServiceCapabilities(OSRVD_SOAP);
//            tds__GetServicesResponse.Service.back()->Capabilities->__any = soap_dom_element(OSRVD_SOAP, NULL, "tptz:Capabilities", capabilities, capabilities->soap_type());
//        }
//    }
}
SOAP_METHOD_END
/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(GetServiceCapabilities)
{
    RESPONSE_VARIABLE(GetServiceCapabilities).Capabilities = OSRVD_ST.soap().device()->serviceCapabilities();
}
SOAP_METHOD_END
/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(GetDeviceInformation)
{
    RESPONSE_VARIABLE(GetDeviceInformation).Manufacturer    = OSRVD_ST.options()["device"].get("manufacturer", "Software Emulation").asString();
    RESPONSE_VARIABLE(GetDeviceInformation).Model           = OSRVD_ST.options()["device"].get("model", "onvif_srvd").asString();
    RESPONSE_VARIABLE(GetDeviceInformation).FirmwareVersion = OSRVD_VERSION_STR;
    RESPONSE_VARIABLE(GetDeviceInformation).SerialNumber    = OSRVD_ST.options()["device"].get("serial_number", "1337").asString();
    RESPONSE_VARIABLE(GetDeviceInformation).HardwareId      = OSRVD_ST.options()["device"].get("hardware_id", "DEADBEEF").asString();
}
SOAP_METHOD_END
/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(GetSystemDateAndTime)
{
    RESPONSE_VARIABLE(GetSystemDateAndTime).SystemDateAndTime = OSRVD_ST.soap().device()->dateTime();
}
SOAP_METHOD_END
/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(GetScopes)
{
    Json::Value scopes = OSRVD_ST.options()["scopes"];
    for ( int index = 0; index < scopes.size(); ++index )
    {
       RESPONSE_VARIABLE(GetScopes).Scopes.push_back(soap_new_req_tt__Scope(OSRVD_SOAP, tt__ScopeDefinition::Fixed, scopes[index].asString()));
    }
}
SOAP_METHOD_END
/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(GetUsers)
{
    tt__User *user = soap_new_tt__User(OSRVD_SOAP);
    user->Username = OSRVD_ST.options()["network"]["user"].asString();
    user->Password = osrvd::soap::valuePointer(OSRVD_SOAP, OSRVD_ST.options()["network"]["password"].asString());
    user->UserLevel = tt__UserLevel::Administrator;
    RESPONSE_VARIABLE(GetUsers).User.push_back(user);
}
SOAP_METHOD_END
/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(GetCapabilities)
{
    RESPONSE_VARIABLE(GetCapabilities).Capabilities = soap_new_tt__Capabilities(OSRVD_SOAP);
    std::vector<tt__CapabilityCategory>& categories(REQUEST_VARIABLE(GetCapabilities)->Category);
    if(categories.empty())
    {
        categories.push_back(tt__CapabilityCategory::All);
    }

    for(tt__CapabilityCategory category : categories)
    {
        if(!RESPONSE_VARIABLE(GetCapabilities).Capabilities->Device && ( (category == tt__CapabilityCategory::All) || (category == tt__CapabilityCategory::Device) ) )
        {
            RESPONSE_VARIABLE(GetCapabilities).Capabilities->Device           = soap_new_tt__DeviceCapabilities(OSRVD_SOAP);
            RESPONSE_VARIABLE(GetCapabilities).Capabilities->Device->XAddr    = OSRVD_ST.soap().serverUri();
            RESPONSE_VARIABLE(GetCapabilities).Capabilities->Device->Network  = soap_new_tt__NetworkCapabilities(OSRVD_SOAP);
            RESPONSE_VARIABLE(GetCapabilities).Capabilities->Device->Security = soap_new_tt__SecurityCapabilities(OSRVD_SOAP);
            RESPONSE_VARIABLE(GetCapabilities).Capabilities->Device->IO       = soap_new_tt__IOCapabilities(OSRVD_SOAP);
            RESPONSE_VARIABLE(GetCapabilities).Capabilities->Device->System   = soap_new_tt__SystemCapabilities(OSRVD_SOAP);
            RESPONSE_VARIABLE(GetCapabilities).Capabilities->Device->System->SupportedVersions.push_back(soap_new_req_tt__OnvifVersion(OSRVD_SOAP, 2, 0));
        }


        if(!RESPONSE_VARIABLE(GetCapabilities).Capabilities->Media && ( (category == tt__CapabilityCategory::All) || (category == tt__CapabilityCategory::Media) ) )
        {
            RESPONSE_VARIABLE(GetCapabilities).Capabilities->Media  = soap_new_tt__MediaCapabilities(OSRVD_SOAP);
            RESPONSE_VARIABLE(GetCapabilities).Capabilities->Media->XAddr = OSRVD_ST.soap().serverUri();
            RESPONSE_VARIABLE(GetCapabilities).Capabilities->Media->StreamingCapabilities = soap_new_tt__RealTimeStreamingCapabilities(OSRVD_SOAP);
        }

//        if (OSRVD_ST.options()["ptz"]["enabled"].asBool()) {
//            if(!tds__GetCapabilitiesResponse.Capabilities->PTZ && ( (category == tt__CapabilityCategory__All) || (category == tt__CapabilityCategory__PTZ) ) )
//            {
//                tds__GetCapabilitiesResponse.Capabilities->PTZ  = soap_new_tt__PTZCapabilities(OSRVD_SOAP);
//                tds__GetCapabilitiesResponse.Capabilities->PTZ->XAddr = OSRVD_ST.soap().serverUri();
//            }
//        }

    }
}
SOAP_METHOD_END
/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(GetNetworkInterfaces)
{
    tt__NetworkInterface *interface = soap_new_tt__NetworkInterface(OSRVD_SOAP);
    tt__IPv4NetworkInterface *ipv4 = soap_new_tt__IPv4NetworkInterface(OSRVD_SOAP);
    tt__PrefixedIPv4Address *ipv4Prefixed = soap_new_tt__PrefixedIPv4Address(OSRVD_SOAP);

    interface->Enabled = true;
    interface->Info = soap_new_tt__NetworkInterfaceInfo(OSRVD_SOAP);
    interface->Info->Name = soap_new_std__string(OSRVD_SOAP);
    interface->Info->Name->assign(OSRVD_ST.eth().interfaceName());
    interface->Info->HwAddress = OSRVD_ST.eth().interfaceHardwareAddress();
    ipv4Prefixed->Address = OSRVD_ST.eth().ip();
    ipv4Prefixed->PrefixLength = OSRVD_ST.eth().ipMaskPrefix();

    ipv4->Config->Manual.push_back(ipv4Prefixed);
    interface->IPv4 = ipv4;
    RESPONSE_VARIABLE(GetNetworkInterfaces).NetworkInterfaces.push_back(interface);

}
SOAP_METHOD_END

MASK_UNUSED_SOAP_CALL(SetSystemDateAndTime)
MASK_UNUSED_SOAP_CALL(AddIPAddressFilter)
MASK_UNUSED_SOAP_CALL(AddScopes)
MASK_UNUSED_SOAP_CALL(CreateCertificate)
MASK_UNUSED_SOAP_CALL(CreateDot1XConfiguration)
MASK_UNUSED_SOAP_CALL(CreateStorageConfiguration)
MASK_UNUSED_SOAP_CALL(CreateUsers)
MASK_UNUSED_SOAP_CALL(DeleteCertificates)
MASK_UNUSED_SOAP_CALL(DeleteDot1XConfiguration)
MASK_UNUSED_SOAP_CALL(DeleteGeoLocation)
MASK_UNUSED_SOAP_CALL(DeleteStorageConfiguration)
MASK_UNUSED_SOAP_CALL(DeleteUsers)
MASK_UNUSED_SOAP_CALL(GetAccessPolicy)
MASK_UNUSED_SOAP_CALL(GetCACertificates)
MASK_UNUSED_SOAP_CALL(GetCertificateInformation)
MASK_UNUSED_SOAP_CALL(GetCertificates)
MASK_UNUSED_SOAP_CALL(GetCertificatesStatus)
MASK_UNUSED_SOAP_CALL(GetClientCertificateMode)
MASK_UNUSED_SOAP_CALL(GetDiscoveryMode)
MASK_UNUSED_SOAP_CALL(GetDNS)
MASK_UNUSED_SOAP_CALL(GetDot11Capabilities)
MASK_UNUSED_SOAP_CALL(GetDot11Status)
MASK_UNUSED_SOAP_CALL(GetDot1XConfiguration)
MASK_UNUSED_SOAP_CALL(GetDot1XConfigurations)
MASK_UNUSED_SOAP_CALL(GetDPAddresses)
MASK_UNUSED_SOAP_CALL(GetDynamicDNS)
MASK_UNUSED_SOAP_CALL(GetEndpointReference)
MASK_UNUSED_SOAP_CALL(GetGeoLocation)
MASK_UNUSED_SOAP_CALL(GetHostname)
MASK_UNUSED_SOAP_CALL(GetIPAddressFilter)
MASK_UNUSED_SOAP_CALL(GetNetworkDefaultGateway)
MASK_UNUSED_SOAP_CALL(GetNetworkProtocols)
MASK_UNUSED_SOAP_CALL(GetNTP)
MASK_UNUSED_SOAP_CALL(GetPkcs10Request)
MASK_UNUSED_SOAP_CALL(GetRelayOutputs)
MASK_UNUSED_SOAP_CALL(GetRemoteDiscoveryMode)
MASK_UNUSED_SOAP_CALL(GetRemoteUser)
MASK_UNUSED_SOAP_CALL(GetStorageConfiguration)
MASK_UNUSED_SOAP_CALL(GetStorageConfigurations)
MASK_UNUSED_SOAP_CALL(GetSystemBackup)
MASK_UNUSED_SOAP_CALL(GetSystemLog)
MASK_UNUSED_SOAP_CALL(GetSystemSupportInformation)
MASK_UNUSED_SOAP_CALL(GetSystemUris)
MASK_UNUSED_SOAP_CALL(GetWsdlUrl)
MASK_UNUSED_SOAP_CALL(GetZeroConfiguration)
MASK_UNUSED_SOAP_CALL(LoadCACertificates)
MASK_UNUSED_SOAP_CALL(LoadCertificates)
MASK_UNUSED_SOAP_CALL(LoadCertificateWithPrivateKey)
MASK_UNUSED_SOAP_CALL(RemoveIPAddressFilter)
MASK_UNUSED_SOAP_CALL(RemoveScopes)
MASK_UNUSED_SOAP_CALL(RestoreSystem)
MASK_UNUSED_SOAP_CALL(ScanAvailableDot11Networks)
MASK_UNUSED_SOAP_CALL(SendAuxiliaryCommand)
MASK_UNUSED_SOAP_CALL(SetAccessPolicy)
MASK_UNUSED_SOAP_CALL(SetCertificatesStatus)
MASK_UNUSED_SOAP_CALL(SetClientCertificateMode)
MASK_UNUSED_SOAP_CALL(SetDiscoveryMode)
MASK_UNUSED_SOAP_CALL(SetDNS)
MASK_UNUSED_SOAP_CALL(SetDot1XConfiguration)
MASK_UNUSED_SOAP_CALL(SetDPAddresses)
MASK_UNUSED_SOAP_CALL(SetDynamicDNS)
MASK_UNUSED_SOAP_CALL(SetGeoLocation)
MASK_UNUSED_SOAP_CALL(SetHostname)
MASK_UNUSED_SOAP_CALL(SetHostnameFromDHCP)
MASK_UNUSED_SOAP_CALL(SetIPAddressFilter)
MASK_UNUSED_SOAP_CALL(SetNetworkDefaultGateway)
MASK_UNUSED_SOAP_CALL(SetNetworkInterfaces)
MASK_UNUSED_SOAP_CALL(SetNetworkProtocols)
MASK_UNUSED_SOAP_CALL(SetNTP)
MASK_UNUSED_SOAP_CALL(SetRelayOutputSettings)
MASK_UNUSED_SOAP_CALL(SetRelayOutputState)
MASK_UNUSED_SOAP_CALL(SetRemoteDiscoveryMode)
MASK_UNUSED_SOAP_CALL(SetRemoteUser)
MASK_UNUSED_SOAP_CALL(SetScopes)
MASK_UNUSED_SOAP_CALL(SetStorageConfiguration)
MASK_UNUSED_SOAP_CALL(SetSystemFactoryDefault)
MASK_UNUSED_SOAP_CALL(SetUser)
MASK_UNUSED_SOAP_CALL(SetZeroConfiguration)
MASK_UNUSED_SOAP_CALL(StartFirmwareUpgrade)
MASK_UNUSED_SOAP_CALL(StartSystemRestore)
MASK_UNUSED_SOAP_CALL(SystemReboot)
MASK_UNUSED_SOAP_CALL(UpgradeSystemFirmware)
