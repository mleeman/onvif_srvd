#include <stdlib.h> // defines getenv in POSIX
#include <sstream>
#include "StreamProfile.h"
#include "ServiceContext.h"
#include "st.h"


tt__VideoSourceConfiguration* StreamProfile::get_video_src_cnf(struct soap *soap) const
{
    tt__VideoSourceConfiguration* src_cfg = soap_new_tt__VideoSourceConfiguration(soap);

    src_cfg->token       = name;
    src_cfg->SourceToken = name;
    src_cfg->Bounds      = soap_new_req_tt__IntRectangle(soap, 0, 0, width, height);

    return src_cfg;
}



tt__VideoEncoderConfiguration* StreamProfile::get_video_enc_cfg(struct soap *soap) const
{
    tt__VideoEncoderConfiguration* enc_cfg = soap_new_tt__VideoEncoderConfiguration(soap);

    enc_cfg->Name               = name;
    enc_cfg->token              = name;
    enc_cfg->Resolution         = soap_new_req_tt__VideoResolution(soap, width, height);
    enc_cfg->RateControl        = soap_new_req_tt__VideoRateControl(soap, 0, 0, 0);
    enc_cfg->Multicast          = soap_new_tt__MulticastConfiguration(soap);
    enc_cfg->Multicast->Address = soap_new_tt__IPAddress(soap);
    enc_cfg->Encoding           = static_cast<tt__VideoEncoding>(type);

    return enc_cfg;
}



tt__PTZConfiguration* StreamProfile::get_ptz_cfg(struct soap *soap) const
{
    tt__PTZConfiguration* ptz_cfg = soap_new_tt__PTZConfiguration(soap);

    ptz_cfg->Name               = "PTZ";
    ptz_cfg->token              = "PTZToken";
    ptz_cfg->NodeToken          = "PTZNodeToken";

    ptz_cfg->DefaultAbsolutePantTiltPositionSpace    = soap_new_std__string(soap);
    *ptz_cfg->DefaultAbsolutePantTiltPositionSpace   = "http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace";
    ptz_cfg->DefaultAbsoluteZoomPositionSpace        = soap_new_std__string(soap);
    *ptz_cfg->DefaultAbsoluteZoomPositionSpace       = "http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace";
    ptz_cfg->DefaultRelativePanTiltTranslationSpace  = soap_new_std__string(soap);
    *ptz_cfg->DefaultRelativePanTiltTranslationSpace = "http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationGenericSpace";
    ptz_cfg->DefaultRelativeZoomTranslationSpace     = soap_new_std__string(soap);
    *ptz_cfg->DefaultRelativeZoomTranslationSpace    = "http://www.onvif.org/ver10/tptz/ZoomSpaces/TranslationGenericSpace";
    ptz_cfg->DefaultContinuousPanTiltVelocitySpace   = soap_new_std__string(soap);
    *ptz_cfg->DefaultContinuousPanTiltVelocitySpace  = "http://www.onvif.org/ver10/tptz/PanTiltSpaces/VelocityGenericSpace";
    ptz_cfg->DefaultContinuousZoomVelocitySpace      = soap_new_std__string(soap);
    *ptz_cfg->DefaultContinuousZoomVelocitySpace     = "http://www.onvif.org/ver10/tptz/ZoomSpaces/VelocityGenericSpace";

    ptz_cfg->DefaultPTZSpeed                   = soap_new_tt__PTZSpeed(soap);
    ptz_cfg->DefaultPTZSpeed->PanTilt          = soap_new_req_tt__Vector2D(soap, 0.1f, 0.1f);
    ptz_cfg->DefaultPTZSpeed->Zoom             = soap_new_req_tt__Vector1D(soap, 1.0f);

    ptz_cfg->DefaultPTZTimeout                 = osrvd::CSoap::valuePointer(soap, std::chrono::nanoseconds(1000000000));

    ptz_cfg->PanTiltLimits                     = soap_new_tt__PanTiltLimits(soap);
    ptz_cfg->PanTiltLimits->Range              = soap_new_tt__Space2DDescription(soap);
    ptz_cfg->PanTiltLimits->Range->URI         = "http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace";
    ptz_cfg->PanTiltLimits->Range->XRange      = soap_new_req_tt__FloatRange(soap, -INFINITY, INFINITY);
    ptz_cfg->PanTiltLimits->Range->YRange      = soap_new_req_tt__FloatRange(soap, -INFINITY, INFINITY);

    ptz_cfg->ZoomLimits                        = soap_new_tt__ZoomLimits(soap);
    ptz_cfg->ZoomLimits->Range                 = soap_new_tt__Space1DDescription(soap);
    ptz_cfg->ZoomLimits->Range->URI            = "http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace";
    ptz_cfg->ZoomLimits->Range->XRange         = soap_new_req_tt__FloatRange(soap, -INFINITY, INFINITY);

    return ptz_cfg;
}



tt__Profile* StreamProfile::get_profile(struct soap *soap) const
{
    ServiceContext* ctx = (ServiceContext*)soap->user;

    tt__Profile* profile = soap_new_tt__Profile(soap);

    profile->Name  = name;
    profile->token = name;

    profile->VideoSourceConfiguration  = get_video_src_cnf(soap);
    profile->VideoEncoderConfiguration = get_video_enc_cfg(soap);
    if (OSRVD_ST.options()["ptz"]["enabled"].asBool()) {
        profile->PTZConfiguration = get_ptz_cfg(soap);
    }

    return profile;
}



tt__VideoSource* StreamProfile::get_video_src(soap *soap) const
{
    tt__VideoSource* video_src = soap_new_tt__VideoSource(soap);

    video_src->token      = name;
    video_src->Resolution = soap_new_req_tt__VideoResolution(soap, width, height);
    video_src->Imaging    = soap_new_tt__ImagingSettings(soap);

    return video_src;
}



bool StreamProfile::set_name(const char *new_val)
{
    if(!new_val)
    {
        str_err = "Name is empty";
        return false;
    }


    name = new_val;
    return true;
}



bool StreamProfile::set_width(const char *new_val)
{

    std::istringstream ss(new_val);
    int tmp_val;
    ss >> tmp_val;


    if( (tmp_val < 100) || (tmp_val >= 10000) )
    {
        str_err = "width is bad, correct range: 100-10000";
        return false;
    }


    width = tmp_val;
    return true;
}



bool StreamProfile::set_height(const char *new_val)
{
    std::istringstream ss(new_val);
    int tmp_val;
    ss >> tmp_val;


    if( (tmp_val < 100) || (tmp_val >= 10000) )
    {
        str_err = "height is bad, correct range: 100-10000";
        return false;
    }


    height = tmp_val;
    return true;
}



bool StreamProfile::set_url(const char *new_val)
{
    if(!new_val)
    {
        str_err = "URL is empty";
        return false;
    }


    url = new_val;
    return true;
}



bool StreamProfile::set_snapurl(const char *new_val)
{
    if(!new_val)
    {
        str_err = "URL is empty";
        return false;
    }


    snapurl = new_val;
    return true;
}



bool StreamProfile::set_type(const char *new_val)
{
    std::string new_type(new_val);


    if( new_type == "JPEG" )
        type = tt__VideoEncoding__JPEG;
    else if( new_type == "MPEG4" )
        type = tt__VideoEncoding__MPEG4;
    else if( new_type == "H264" )
        type = tt__VideoEncoding__H264;
    else
    {
        str_err = "type dont support";
        return false;
    }


    return true;
}



void StreamProfile::clear()
{
    name.clear();
    url.clear();
    snapurl.clear();

    width  = -1;
    height = -1;
    type   = -1;
}



bool StreamProfile::is_valid() const
{
    return ( !name.empty()  &&
             !url.empty()   &&
             (width  != -1) &&
             (height != -1) &&
             (type   != -1)
           );
}
