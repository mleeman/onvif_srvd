#include "cmedia.h"
#include "smacros.h"
#include "soap_defines.h"
#include "st.h"

namespace osrvd
{
namespace soap
{

CMedia::CMedia() : MediaBindingService(OSRVD_SOAP)
{}

CMedia::~CMedia()
{}

trt__Capabilities *CMedia::serviceCapabilities()
{
    trt__Capabilities *capabilities = soap_new_trt__Capabilities(OSRVD_SOAP);
    capabilities->Rotation          = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->VideoSourceMode   = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->OSD               = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->EXICompression    = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    for (int i_camera = 0; i_camera < OSRVD_ST.camears().count(); i_camera++)
    {
        std::string snapshotUrl = OSRVD_ST.camears().camera(i_camera)->snapshotUrl();
        if (!snapshotUrl.empty() && capabilities->SnapshotUri == NULL)
        {
            capabilities->SnapshotUri = osrvd::soap::valuePointer(OSRVD_SOAP, true);
            break;
        }
    }

    capabilities->ProfileCapabilities = soap_new_trt__ProfileCapabilities(OSRVD_SOAP);
    capabilities->ProfileCapabilities->MaximumNumberOfProfiles = osrvd::soap::valuePointer(OSRVD_SOAP, OSRVD_ST.camears().totalStreamsCount());

    capabilities->StreamingCapabilities = soap_new_trt__StreamingCapabilities(OSRVD_SOAP);
    capabilities->StreamingCapabilities->RTPMulticast             = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->StreamingCapabilities->RTP_USCORERTSP_USCORETCP = osrvd::soap::valuePointer(OSRVD_SOAP, true);
    capabilities->StreamingCapabilities->RTP_USCORETCP            = osrvd::soap::valuePointer(OSRVD_SOAP, true);
    capabilities->StreamingCapabilities->NoRTSPStreaming          = osrvd::soap::valuePointer(OSRVD_SOAP, false);

    return capabilities;
}

}
}

#define REQUEST_VARIABLE(_method)  OSRVD_SOAP_REQUEST_VARIABLE(_method, trt)
#define RESPONSE_VARIABLE(_method) OSRVD_SOAP_RESPONSE_VARIABLE(_method, trt)
#define SOAP_METHOD_BEGIN(_method) OSRVD_SOAP_METHOD_BEGIN(_method, trt, MediaBindingService)
#define SOAP_METHOD_END OSRVD_SOAP_METHOD_END
#define MASK_UNUSED_SOAP_CALL(_method) OSRVD_MASK_UNUSED_SOAP_CALL_DEFINE(_method, trt, MediaBindingService)

/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(GetServiceCapabilities)
{
    RESPONSE_VARIABLE(GetServiceCapabilities).Capabilities = OSRVD_ST.soap().media()->serviceCapabilities();
}
SOAP_METHOD_END
/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(GetVideoSources)
{
    for (int i_camera = 0; i_camera < OSRVD_ST.camears().count(); i_camera++)
    {
        for (int i_stream = 0; i_stream < OSRVD_ST.camears().camera(i_camera)->streams(); i_stream++)
        {
            RESPONSE_VARIABLE(GetVideoSources).VideoSources.push_back(OSRVD_ST.camears().camera(i_camera)->stream(i_stream)->videoSource());
        }
    }
}
SOAP_METHOD_END
/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(GetProfile)
{
    OSRVD_LOG_DBG("Profile token: " << REQUEST_VARIABLE(GetProfile)->ProfileToken);
    osrvd::entity::CCameraStream *stream = OSRVD_ST.camears().findStream(REQUEST_VARIABLE(GetProfile)->ProfileToken);
    if(!stream)
    {
        OSRVD_LOG_ERR("Stream " << REQUEST_VARIABLE(GetProfile)->ProfileToken << " not found");
        return SOAP_FAULT;
    }
    RESPONSE_VARIABLE(GetProfile).Profile = stream->profile();
}
SOAP_METHOD_END
/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(GetProfiles)
{
    for (int i_camera = 0; i_camera < OSRVD_ST.camears().count(); i_camera++)
    {
        for (int i_stream = 0; i_stream < OSRVD_ST.camears().camera(i_camera)->streams(); i_stream++)
        {
            RESPONSE_VARIABLE(GetProfiles).Profiles.push_back(OSRVD_ST.camears().camera(i_camera)->stream(i_stream)->profile());
        }
    }
}
SOAP_METHOD_END
/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(GetStreamUri)
{
    OSRVD_LOG_DBG("Profile token: " << REQUEST_VARIABLE(GetStreamUri)->ProfileToken);
    osrvd::entity::CCameraStream *stream = OSRVD_ST.camears().findStream(REQUEST_VARIABLE(GetStreamUri)->ProfileToken);
    if(!stream)
    {
        OSRVD_LOG_ERR("Stream " << REQUEST_VARIABLE(GetStreamUri)->ProfileToken << " not found");
        return SOAP_FAULT;
    }

    RESPONSE_VARIABLE(GetStreamUri).MediaUri = soap_new_tt__MediaUri(OSRVD_SOAP);
    RESPONSE_VARIABLE(GetStreamUri).MediaUri->Uri = stream->url();
    OSRVD_LOG_DBG("Returning uri: '" << RESPONSE_VARIABLE(GetStreamUri).MediaUri->Uri << "'");
}
SOAP_METHOD_END
/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(GetSnapshotUri)
{
    OSRVD_LOG_DBG("Profile token: " << REQUEST_VARIABLE(GetSnapshotUri)->ProfileToken);
//    int ret = SOAP_FAULT;

//    ServiceContext* ctx = (ServiceContext*)this->soap->user;
//    auto profiles       = ctx->get_profiles();
//    auto it             = profiles.find(trt__GetSnapshotUri->ProfileToken);

//    if( it != profiles.end() )
//    {
//        trt__GetSnapshotUriResponse.MediaUri = soap_new_tt__MediaUri(this->soap);
//        trt__GetSnapshotUriResponse.MediaUri->Uri = ctx->get_snapshot_uri(it->second.get_snapurl(), htonl(this->soap->ip));
//        ret = SOAP_OK;
//    }


//    return ret;

}
SOAP_METHOD_END
/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(GetVideoEncoderConfigurationOptions)
{
    OSRVD_LOG_DBG("Profile token: " << REQUEST_VARIABLE(GetVideoEncoderConfigurationOptions)->ProfileToken << "; Configuration token: " << *REQUEST_VARIABLE(GetVideoEncoderConfigurationOptions)->ConfigurationToken);
    osrvd::entity::CCameraStream *stream = OSRVD_ST.camears().findStream(*REQUEST_VARIABLE(GetVideoEncoderConfigurationOptions)->ConfigurationToken);
    if(!stream)
    {
        OSRVD_LOG_ERR("Stream " << *REQUEST_VARIABLE(GetVideoEncoderConfigurationOptions)->ConfigurationToken << " not found");
        return SOAP_FAULT;
    }
    RESPONSE_VARIABLE(GetVideoEncoderConfigurationOptions).Options = soap_new_req_tt__VideoEncoderConfigurationOptions(OSRVD_SOAP, soap_new_req_tt__IntRange(OSRVD_SOAP, 100, 100));
    switch (stream->parentCamera()->type())
    {
        case tt__VideoEncoding::H264:
        {
            RESPONSE_VARIABLE(GetVideoEncoderConfigurationOptions).Options->H264 = soap_new_tt__H264Options(OSRVD_SOAP);
            RESPONSE_VARIABLE(GetVideoEncoderConfigurationOptions).Options->H264->ResolutionsAvailable.push_back(soap_new_req_tt__VideoResolution(OSRVD_SOAP, stream->width(), stream->height()));
            RESPONSE_VARIABLE(GetVideoEncoderConfigurationOptions).Options->H264->FrameRateRange = soap_new_req_tt__IntRange(OSRVD_SOAP, stream->framerate(), stream->framerate());
            RESPONSE_VARIABLE(GetVideoEncoderConfigurationOptions).Options->H264->EncodingIntervalRange = soap_new_req_tt__IntRange(OSRVD_SOAP, 1, 1);
//            RESPONSE_VARIABLE(GetVideoEncoderConfigurationOptions).Options->H264->GovLengthRange = soap_new_req_tt__IntRange(OSRVD_SOAP, 1, 10);
        }
        break;
        case tt__VideoEncoding::JPEG:
        {
            RESPONSE_VARIABLE(GetVideoEncoderConfigurationOptions).Options->JPEG = soap_new_tt__JpegOptions(OSRVD_SOAP);
            RESPONSE_VARIABLE(GetVideoEncoderConfigurationOptions).Options->JPEG->ResolutionsAvailable.push_back(soap_new_req_tt__VideoResolution(OSRVD_SOAP, stream->width(), stream->height()));
            RESPONSE_VARIABLE(GetVideoEncoderConfigurationOptions).Options->JPEG->EncodingIntervalRange = soap_new_req_tt__IntRange(OSRVD_SOAP, 1, 1);
        }
        break;
        case tt__VideoEncoding::MPEG4:
        {
            RESPONSE_VARIABLE(GetVideoEncoderConfigurationOptions).Options->MPEG4 = soap_new_tt__Mpeg4Options(OSRVD_SOAP);
            RESPONSE_VARIABLE(GetVideoEncoderConfigurationOptions).Options->MPEG4->ResolutionsAvailable.push_back(soap_new_req_tt__VideoResolution(OSRVD_SOAP, stream->width(), stream->height()));
            RESPONSE_VARIABLE(GetVideoEncoderConfigurationOptions).Options->MPEG4->FrameRateRange = soap_new_req_tt__IntRange(OSRVD_SOAP, stream->framerate(), stream->framerate());
            RESPONSE_VARIABLE(GetVideoEncoderConfigurationOptions).Options->MPEG4->EncodingIntervalRange = soap_new_req_tt__IntRange(OSRVD_SOAP, 1, 1);
//            RESPONSE_VARIABLE(GetVideoEncoderConfigurationOptions).Options->MPEG4->GovLengthRange = soap_new_req_tt__IntRange(OSRVD_SOAP, 1, 10);
        }
        break;
    }
}
SOAP_METHOD_END
/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(SetSynchronizationPoint)
{
    OSRVD_LOG_DBG("Profile token: " << REQUEST_VARIABLE(SetSynchronizationPoint)->ProfileToken);
    return SOAP_NO_METHOD;
}
SOAP_METHOD_END

MASK_UNUSED_SOAP_CALL(AddAudioDecoderConfiguration)
MASK_UNUSED_SOAP_CALL(AddAudioEncoderConfiguration)
MASK_UNUSED_SOAP_CALL(AddAudioOutputConfiguration)
MASK_UNUSED_SOAP_CALL(AddAudioSourceConfiguration)
MASK_UNUSED_SOAP_CALL(AddMetadataConfiguration)
MASK_UNUSED_SOAP_CALL(AddPTZConfiguration)
MASK_UNUSED_SOAP_CALL(AddVideoAnalyticsConfiguration)
MASK_UNUSED_SOAP_CALL(AddVideoEncoderConfiguration)
MASK_UNUSED_SOAP_CALL(AddVideoSourceConfiguration)
MASK_UNUSED_SOAP_CALL(CreateOSD)
MASK_UNUSED_SOAP_CALL(CreateProfile)
MASK_UNUSED_SOAP_CALL(DeleteOSD)
MASK_UNUSED_SOAP_CALL(DeleteProfile)
MASK_UNUSED_SOAP_CALL(GetAudioDecoderConfiguration)
MASK_UNUSED_SOAP_CALL(GetAudioDecoderConfigurationOptions)
MASK_UNUSED_SOAP_CALL(GetAudioDecoderConfigurations)
MASK_UNUSED_SOAP_CALL(GetAudioEncoderConfiguration)
MASK_UNUSED_SOAP_CALL(GetAudioEncoderConfigurationOptions)
MASK_UNUSED_SOAP_CALL(GetAudioEncoderConfigurations)
MASK_UNUSED_SOAP_CALL(GetAudioOutputConfiguration)
MASK_UNUSED_SOAP_CALL(GetAudioOutputConfigurationOptions)
MASK_UNUSED_SOAP_CALL(GetAudioOutputConfigurations)
MASK_UNUSED_SOAP_CALL(GetAudioOutputs)
MASK_UNUSED_SOAP_CALL(GetAudioSourceConfiguration)
MASK_UNUSED_SOAP_CALL(GetAudioSourceConfigurationOptions)
MASK_UNUSED_SOAP_CALL(GetAudioSourceConfigurations)
MASK_UNUSED_SOAP_CALL(GetAudioSources)
MASK_UNUSED_SOAP_CALL(GetCompatibleAudioDecoderConfigurations)
MASK_UNUSED_SOAP_CALL(GetCompatibleAudioEncoderConfigurations)
MASK_UNUSED_SOAP_CALL(GetCompatibleAudioOutputConfigurations)
MASK_UNUSED_SOAP_CALL(GetCompatibleAudioSourceConfigurations)
MASK_UNUSED_SOAP_CALL(GetCompatibleMetadataConfigurations)
MASK_UNUSED_SOAP_CALL(GetCompatibleVideoAnalyticsConfigurations)
MASK_UNUSED_SOAP_CALL(GetCompatibleVideoEncoderConfigurations)
MASK_UNUSED_SOAP_CALL(GetCompatibleVideoSourceConfigurations)
MASK_UNUSED_SOAP_CALL(GetGuaranteedNumberOfVideoEncoderInstances)
MASK_UNUSED_SOAP_CALL(GetMetadataConfiguration)
MASK_UNUSED_SOAP_CALL(GetMetadataConfigurationOptions)
MASK_UNUSED_SOAP_CALL(GetMetadataConfigurations)
MASK_UNUSED_SOAP_CALL(GetOSD)
MASK_UNUSED_SOAP_CALL(GetOSDOptions)
MASK_UNUSED_SOAP_CALL(GetOSDs)
MASK_UNUSED_SOAP_CALL(GetVideoAnalyticsConfiguration)
MASK_UNUSED_SOAP_CALL(GetVideoAnalyticsConfigurations)
MASK_UNUSED_SOAP_CALL(GetVideoEncoderConfiguration)
MASK_UNUSED_SOAP_CALL(GetVideoEncoderConfigurations)
MASK_UNUSED_SOAP_CALL(GetVideoSourceConfiguration)
MASK_UNUSED_SOAP_CALL(GetVideoSourceConfigurationOptions)
MASK_UNUSED_SOAP_CALL(GetVideoSourceConfigurations)
MASK_UNUSED_SOAP_CALL(GetVideoSourceModes)
MASK_UNUSED_SOAP_CALL(RemoveAudioDecoderConfiguration)
MASK_UNUSED_SOAP_CALL(RemoveAudioEncoderConfiguration)
MASK_UNUSED_SOAP_CALL(RemoveAudioOutputConfiguration)
MASK_UNUSED_SOAP_CALL(RemoveAudioSourceConfiguration)
MASK_UNUSED_SOAP_CALL(RemoveMetadataConfiguration)
MASK_UNUSED_SOAP_CALL(RemovePTZConfiguration)
MASK_UNUSED_SOAP_CALL(RemoveVideoAnalyticsConfiguration)
MASK_UNUSED_SOAP_CALL(RemoveVideoEncoderConfiguration)
MASK_UNUSED_SOAP_CALL(RemoveVideoSourceConfiguration)
MASK_UNUSED_SOAP_CALL(SetAudioDecoderConfiguration)
MASK_UNUSED_SOAP_CALL(SetAudioEncoderConfiguration)
MASK_UNUSED_SOAP_CALL(SetAudioOutputConfiguration)
MASK_UNUSED_SOAP_CALL(SetAudioSourceConfiguration)
MASK_UNUSED_SOAP_CALL(SetMetadataConfiguration)
MASK_UNUSED_SOAP_CALL(SetOSD)
MASK_UNUSED_SOAP_CALL(SetVideoAnalyticsConfiguration)
MASK_UNUSED_SOAP_CALL(SetVideoEncoderConfiguration)
MASK_UNUSED_SOAP_CALL(SetVideoSourceConfiguration)
MASK_UNUSED_SOAP_CALL(SetVideoSourceMode)
MASK_UNUSED_SOAP_CALL(StartMulticastStreaming)
MASK_UNUSED_SOAP_CALL(StopMulticastStreaming)
