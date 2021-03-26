#include "ccamerastream.h"
#include "soap_defines.h"
#include "st.h"

namespace osrvd
{
namespace entity
{


CCameraStream::CCameraStream(const int &cameraIndex, const int &streamIndex) :
    m_cameraIndex(cameraIndex),
    m_streamIndex(streamIndex)
{}

CCameraStream::~CCameraStream()
{}

std::string CCameraStream::name() const
{
    return OSRVD_ST.options()["cameras"][m_cameraIndex]["streams"][m_streamIndex]["name"].asString();
}

std::string CCameraStream::profileName() const
{
    return "Profile " + parentCamera()->name();
}

std::string CCameraStream::encoderName() const
{
    return parentCamera()->name() + " " + name() + " Encoder";
}

std::string CCameraStream::token() const
{
    return parentCamera()->name() + "::" + name();
}

std::string CCameraStream::sourceToken() const
{
    return token();
}

std::string CCameraStream::url() const
{
    OSRVD_LOG_NFO("Requested " << token() << " stream url");
    return OSRVD_ST.options()["cameras"][m_cameraIndex]["streams"][m_streamIndex]["url"].asString();
}

int CCameraStream::width() const
{
    return OSRVD_ST.options()["cameras"][m_cameraIndex]["streams"][m_streamIndex]["width"].asInt();
}

int CCameraStream::height() const
{
    return OSRVD_ST.options()["cameras"][m_cameraIndex]["streams"][m_streamIndex]["height"].asInt();
}

int CCameraStream::framerate() const
{
    return OSRVD_ST.options()["cameras"][m_cameraIndex]["streams"][m_streamIndex].get("framerate", 25).asInt();
}

bool CCameraStream::check()
{
    bool result = true;
    if(name().empty())
    {
        OSRVD_LOG_ERR("Stream name is empty");
        result = false;
    }
    if(profileName().length() > 64)
    {
        OSRVD_LOG_ERR("Stream profile name '" << profileName() << "' too big. Reduce camera name (" << parentCamera()->name() << ") size max to 64 symbols");
        result = false;
    }
    if(encoderName().length() > 64)
    {
        OSRVD_LOG_ERR("Stream encoder name '" << encoderName() << "' too big. Reduce camera name (" << parentCamera()->name() << ") or/and stream name (" << name() << ") size max to 64 symbols");
        result = false;
    }
    if(token().length() > 64)
    {
        OSRVD_LOG_ERR("Stream token name '" << token() << "' too big. Reduce camera name (" << parentCamera()->name() << ") or/and stream name (" << name() << ") size max to 64 symbols");
        result = false;
    }
    if(width() <= 100 or width() >= 10000)
    {
        OSRVD_LOG_ERR("Stream '" << name() << "' width (" << width() << ") not in bounds (100,10000)");
        result = false;
    }
    if(height() <= 100 or height() >= 10000)
    {
        OSRVD_LOG_ERR("Stream '" << name() << "' height (" << height() << ") not in bounds (100,10000)");
        result = false;
    }
    return result;
}

tt__VideoSource *CCameraStream::videoSource() const
{
    tt__VideoSource* videoSource = soap_new_tt__VideoSource(OSRVD_SOAP);

    videoSource->token      = token();
    videoSource->Framerate  = framerate();
    videoSource->Resolution = soap_new_req_tt__VideoResolution(OSRVD_SOAP, width(), height());
    videoSource->Imaging    = soap_new_tt__ImagingSettings(OSRVD_SOAP);

    OSRVD_LOG_DBG("[" << __func__ << "] token: " << videoSource->token);
    return videoSource;
}

tt__VideoSourceConfiguration* CCameraStream::videoSourceConfiguration() const
{
    tt__VideoSourceConfiguration* videoSourceConfiguration = soap_new_tt__VideoSourceConfiguration(OSRVD_SOAP);

    videoSourceConfiguration->Name        = profileName();
    videoSourceConfiguration->token       = token();
    videoSourceConfiguration->SourceToken = sourceToken();
    videoSourceConfiguration->Bounds      = soap_new_req_tt__IntRectangle(OSRVD_SOAP, 0, 0, width(), height());

    OSRVD_LOG_DBG("[" << __func__ << "] Name: " << videoSourceConfiguration->Name << "; token: " << videoSourceConfiguration->token << "; sourceToken: " << videoSourceConfiguration->SourceToken);
    return videoSourceConfiguration;
}

tt__VideoEncoderConfiguration* CCameraStream::videoEncoderConfiguration() const
{
    tt__VideoEncoderConfiguration* videoEncoderConfiguration = soap_new_tt__VideoEncoderConfiguration(OSRVD_SOAP);

    videoEncoderConfiguration->Name        = encoderName();
    videoEncoderConfiguration->token       = token();
    videoEncoderConfiguration->Resolution  = soap_new_req_tt__VideoResolution(OSRVD_SOAP, width(), height());
    videoEncoderConfiguration->RateControl = soap_new_req_tt__VideoRateControl(OSRVD_SOAP, framerate(), 1, framerate()*width()*height());
//    videoEncoderConfiguration->Multicast          = soap_new_tt__MulticastConfiguration(OSRVD_SOAP);
//    videoEncoderConfiguration->Multicast->Address = soap_new_tt__IPAddress(OSRVD_SOAP);
    videoEncoderConfiguration->Encoding    = OSRVD_ST.camears().camera(m_cameraIndex)->type();


//    OSRVD_LOG_DBG("w: " << videoEncoderConfiguration->Resolution->Width << "; h: " << videoEncoderConfiguration->Resolution->Height)
    OSRVD_LOG_DBG("[" << __func__ << "] token: " << videoEncoderConfiguration->token << "; encoderName: " << videoEncoderConfiguration->Name);
    return videoEncoderConfiguration;
}

tt__Profile* CCameraStream::profile() const
{
    tt__Profile* profile = soap_new_tt__Profile(OSRVD_SOAP);

    profile->Name  = profileName();
    profile->token = token();
    profile->fixed = osrvd::soap::valuePointer(OSRVD_SOAP, true);

    profile->VideoSourceConfiguration  = videoSourceConfiguration();
    profile->VideoEncoderConfiguration = videoEncoderConfiguration();
    if (OSRVD_ST.camears().camera(m_cameraIndex)->ptz()->enabled())
    {
//        profile->PTZConfiguration = get_ptz_cfg(OSRVD_SOAP);
    }
    OSRVD_LOG_DBG("[" << __func__ << "] name: " << profile->Name << "; token: " << profile->token);
    return profile;
}

CCameraProfile *CCameraStream::parentCamera() const
{
    return OSRVD_ST.camears().camera(m_cameraIndex);
}


}
}
