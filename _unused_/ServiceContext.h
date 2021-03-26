#ifndef SERVICECONTEXT_H
#define SERVICECONTEXT_H


#include <string>
#include <vector>
#include <map>

#include "ServiceDevice.h"
#include "soapH.h"
#include "StreamProfile.h"
#include "PTZNode.h"
#include "eth_dev_param.h"


class ServiceContext
{
    public:

        enum TimeZoneForamt : unsigned int
        {
            TZ_UTC_OFFSET,   //[+|-]hh:mm
            TZ_GMT_OFFSET,   //[+|-]hh:mm     (reverse sign of TZ_UTC_OFFSET)
            TZ_UTC,          //UTC[+|-]hh:mm
            TZ_GMT,          //GMT[+|-]hh:mm  (reverse sign of TZ_UTC)
            TZ_ENV,          //value from TZ environment variable
            TZ_ONVIF_ENV,    //value from TZ_ONVIF environment variable(its not std)

            TZ_CNT_FORMATS   //Its not format! Its counter for use in code (max index)
        };


        ServiceContext();

        std::vector<Eth_Dev_Param> eth_ifs; //ethernet interfaces

        std::string  get_time_zone() const;

        tt__SystemDateTime *get_SystemDateAndTime(struct soap* soap);
        tt__DateTime *get_DateTime(struct soap* soap, struct tm *time_info);

        TimeZoneForamt get_tz_format() const { return tz_format; }
        bool set_tz_format(const char *new_val);

        std::string getServerIpFromClientIp(uint32_t client_ip) const;
        std::string getXAddr(struct soap* soap) const;



        std::string get_str_err() const { return str_err;         }
        const char* get_cstr_err()const { return str_err.c_str(); }


        bool add_profile(const StreamProfile& profile);


        std::string get_stream_uri(const std::string& profile_url, uint32_t client_ip) const;
        std::string get_snapshot_uri(const std::string& profile_url, uint32_t client_ip) const;


        const std::map<std::string, StreamProfile> &get_profiles(void) { return profiles; }

        // service capabilities
        tds__DeviceServiceCapabilities* getDeviceServiceCapabilities(struct soap* soap);
        trt__Capabilities*  getMediaServiceCapabilities    (struct soap* soap);
        tptz__Capabilities*  getPTZServiceCapabilities     (struct soap* soap);
//        timg__Capabilities* getImagingServiceCapabilities  (struct soap* soap);
//        trc__Capabilities*  getRecordingServiceCapabilities(struct soap* soap);
//        tse__Capabilities*  getSearchServiceCapabilities   (struct soap* soap);
//        trv__Capabilities*  getReceiverServiceCapabilities (struct soap* soap);
//        trp__Capabilities*  getReplayServiceCapabilities   (struct soap* soap);
//        tev__Capabilities*  getEventServiceCapabilities    (struct soap* soap);
//        tls__Capabilities*  getDisplayServiceCapabilities  (struct soap* soap);
//        tmd__Capabilities*  getDeviceIOServiceCapabilities (struct soap* soap);

    private:

        std::map<std::string, StreamProfile> profiles;

        TimeZoneForamt tz_format;

        std::string  str_err;
};





#endif // SERVICECONTEXT_H
