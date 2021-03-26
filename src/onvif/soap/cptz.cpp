#include "cptz.h"
//#include "smacros.h"
#include "soap_defines.h"
#include "st.h"
namespace osrvd
{
namespace soap
{

CPtz::CPtz() : PTZBindingService(OSRVD_SOAP)
{}

CPtz::~CPtz()
{}

tptz__Capabilities *CPtz::serviceCapabilities()
{
    tptz__Capabilities *capabilities = soap_new_tptz__Capabilities(OSRVD_SOAP);
    capabilities->EFlip                       = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->Reverse                     = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    capabilities->GetCompatibleConfigurations = osrvd::soap::valuePointer(OSRVD_SOAP, false);
    return capabilities;
}

}
}


#define REQUEST_VARIABLE(_method)  OSRVD_SOAP_REQUEST_VARIABLE(_method, tptz)
#define RESPONSE_VARIABLE(_method) OSRVD_SOAP_RESPONSE_VARIABLE(_method, tptz)
#define SOAP_METHOD_BEGIN(_method) OSRVD_SOAP_METHOD_BEGIN(_method, tptz, PTZBindingService)
#define SOAP_METHOD_END OSRVD_SOAP_METHOD_END
#define MASK_UNUSED_SOAP_CALL(_method) OSRVD_MASK_UNUSED_SOAP_CALL_DEFINE(_method, tptz, PTZBindingService)

//int GetPTZPreset(struct soap *soap, tt__PTZPreset* ptzp, int number)
//{
//    ptzp->token  = soap_new_std__string(soap);
//    *ptzp->token = std::to_string(number);
//    ptzp->Name   = soap_new_std__string(soap);
//    *ptzp->Name  = std::to_string(number);

//    ptzp->PTZPosition             = soap_new_tt__PTZVector(soap);
//    ptzp->PTZPosition->PanTilt    = soap_new_req_tt__Vector2D(soap, 0.0f, 0.0f);
//    ptzp->PTZPosition->Zoom       = soap_new_req_tt__Vector1D(soap, 1.0f);

//    return SOAP_OK;
//}


/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(GetPresets)
{
//    soap_default_std__vectorTemplateOfPointerTott__PTZPreset(soap, &tptz__GetPresetsResponse._tptz__GetPresetsResponse::Preset);
//    for (int i = 0; i < 8; i++) {
//        tt__PTZPreset* ptzp;
//        ptzp = soap_new_tt__PTZPreset(soap);
//        tptz__GetPresetsResponse.Preset.push_back(ptzp);
//        GetPTZPreset(this->soap, ptzp, i);
//    }
}
SOAP_METHOD_END
/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(GotoPreset)
{
//    std::string preset_cmd;

//    ServiceContext* ctx = (ServiceContext*)this->soap->user;

//    if (tptz__GotoPreset == NULL) {
//        return SOAP_OK;
//    }
//    if (tptz__GotoPreset->ProfileToken.c_str() == NULL) {
//        return SOAP_OK;
//    }
//    if (tptz__GotoPreset->PresetToken.c_str() == NULL) {
//        return SOAP_OK;
//    }

//    std::string move_preset = OSRVD_ST.options()["ptz"]["move_preset"].asString();
//    if (!move_preset.empty()) {
//        preset_cmd = move_preset.c_str();
//    } else {
//        return SOAP_OK;
//    }

//    std::string template_str_t("%t");

//    auto it_t = preset_cmd.find(template_str_t, 0);

//    if( it_t != std::string::npos ) {
//        preset_cmd.replace(it_t, template_str_t.size(), tptz__GotoPreset->PresetToken.c_str());
//    }

//    system(preset_cmd.c_str());
}
SOAP_METHOD_END

//int GetPTZNode(struct soap *soap, tt__PTZNode* ptzn)
//{
//    ptzn->token = "PTZNodeToken";
//    ptzn->Name  = soap_new_std__string(soap);
//    *ptzn->Name = "PTZ";

//    ptzn->SupportedPTZSpaces = soap_new_tt__PTZSpaces(soap);;
//    soap_default_std__vectorTemplateOfPointerTott__Space2DDescription(soap, &ptzn->SupportedPTZSpaces->tt__PTZSpaces::RelativePanTiltTranslationSpace);
//    soap_default_std__vectorTemplateOfPointerTott__Space1DDescription(soap, &ptzn->SupportedPTZSpaces->tt__PTZSpaces::RelativeZoomTranslationSpace);
//    soap_default_std__vectorTemplateOfPointerTott__Space2DDescription(soap, &ptzn->SupportedPTZSpaces->tt__PTZSpaces::ContinuousPanTiltVelocitySpace);
//    soap_default_std__vectorTemplateOfPointerTott__Space1DDescription(soap, &ptzn->SupportedPTZSpaces->tt__PTZSpaces::ContinuousZoomVelocitySpace);
//    soap_default_std__vectorTemplateOfPointerTott__Space1DDescription(soap, &ptzn->SupportedPTZSpaces->tt__PTZSpaces::PanTiltSpeedSpace);
//    soap_default_std__vectorTemplateOfPointerTott__Space1DDescription(soap, &ptzn->SupportedPTZSpaces->tt__PTZSpaces::ZoomSpeedSpace);


//    auto ptzs1 = soap_new_tt__Space2DDescription(soap);
//    ptzn->SupportedPTZSpaces->RelativePanTiltTranslationSpace.push_back(ptzs1);

//    auto ptzs2 = soap_new_tt__Space1DDescription(soap);
//    ptzn->SupportedPTZSpaces->RelativeZoomTranslationSpace.push_back(ptzs2);

//    auto ptzs3 = soap_new_tt__Space2DDescription(soap);
//    ptzn->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace.push_back(ptzs3);

//    auto ptzs4 = soap_new_tt__Space1DDescription(soap);
//    ptzn->SupportedPTZSpaces->ContinuousZoomVelocitySpace.push_back(ptzs4);

//    auto ptzs5 = soap_new_tt__Space1DDescription(soap);
//    ptzn->SupportedPTZSpaces->PanTiltSpeedSpace.push_back(ptzs5);

//    auto ptzs6 = soap_new_tt__Space1DDescription(soap);
//    ptzn->SupportedPTZSpaces->ZoomSpeedSpace.push_back(ptzs6);

//    ptzs1->URI         = "http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationGenericSpace";
//    ptzs1->XRange      = soap_new_req_tt__FloatRange(soap, -1.0f, 1.0f);
//    ptzs1->YRange      = soap_new_req_tt__FloatRange(soap, -1.0f, 1.0f);

//    ptzs2->URI         = "http://www.onvif.org/ver10/tptz/ZoomSpaces/TranslationGenericSpace";
//    ptzs2->XRange      = soap_new_req_tt__FloatRange(soap, -1.0f, 1.0f);

//    ptzs3->URI         = "http://www.onvif.org/ver10/tptz/PanTiltSpaces/VelocityGenericSpace";
//    ptzs3->XRange      = soap_new_req_tt__FloatRange(soap, -1.0f, 1.0f);
//    ptzs3->YRange      = soap_new_req_tt__FloatRange(soap, -1.0f, 1.0f);

//    ptzs4->URI         = "http://www.onvif.org/ver10/tptz/ZoomSpaces/VelocityGenericSpace";
//    ptzs4->XRange      = soap_new_req_tt__FloatRange(soap, -1.0f, 1.0f);

//    ptzs5->URI         = "http://www.onvif.org/ver10/tptz/PanTiltSpaces/GenericSpeedSpace";
//    ptzs5->XRange      = soap_new_req_tt__FloatRange(soap, 0.0f, 1.0f);

//    ptzs6->URI         = "http://www.onvif.org/ver10/tptz/ZoomSpaces/ZoomGenericSpeedSpace";
//    ptzs6->XRange      = soap_new_req_tt__FloatRange(soap, 0.0f, 1.0f);


//    ptzn->MaximumNumberOfPresets = 8;
//    ptzn->HomeSupported          = true;
//    ptzn->FixedHomePosition      = (bool *)soap_malloc(soap, sizeof(bool));
//    soap_s2bool(soap, "true", ptzn->FixedHomePosition);

//    return SOAP_OK;
//}

/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(GetNodes)
{
//    soap_default_std__vectorTemplateOfPointerTott__PTZNode(soap, &tptz__GetNodesResponse._tptz__GetNodesResponse::PTZNode);
//    tt__PTZNode* ptzn;
//    ptzn = soap_new_tt__PTZNode(soap);
//    tptz__GetNodesResponse.PTZNode.push_back(ptzn);
//    GetPTZNode(this->soap, ptzn);
}
SOAP_METHOD_END
/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(GetNode)
{
//    tptz__GetNodeResponse.PTZNode = soap_new_tt__PTZNode(this->soap);
//    GetPTZNode(this->soap, tptz__GetNodeResponse.PTZNode);
}
SOAP_METHOD_END
/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(GotoHomePosition)
{
//    std::string preset_cmd;

//    ServiceContext* ctx = (ServiceContext*)this->soap->user;

//    if (tptz__GotoHomePosition == NULL) {
//        return SOAP_OK;
//    }
//    if (tptz__GotoHomePosition->ProfileToken.c_str() == NULL) {
//        return SOAP_OK;
//    }

//    std::string move_preset = OSRVD_ST.options()["ptz"]["move_preset"].asString();
//    if (!move_preset.empty()) {
//        preset_cmd = move_preset.c_str();
//    } else {
//        return SOAP_OK;
//    }

//    std::string template_str_t("%t");

//    auto it_t = preset_cmd.find(template_str_t, 0);

//    if( it_t != std::string::npos ) {
//        preset_cmd.replace(it_t, template_str_t.size(), "1");
//    }

//    system(preset_cmd.c_str());
}
SOAP_METHOD_END
/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(ContinuousMove)
{
//    ServiceContext* ctx = (ServiceContext*)this->soap->user;

//    if (tptz__ContinuousMove == NULL) {
//        return SOAP_OK;
//    }
//    if (tptz__ContinuousMove->Velocity == NULL) {
//        return SOAP_OK;
//    }
//    if (tptz__ContinuousMove->Velocity->PanTilt == NULL) {
//        return SOAP_OK;
//    }

//    if (tptz__ContinuousMove->Velocity->PanTilt->x > 0) {
//        system(OSRVD_ST.options()["ptz"]["move_right"].asString().c_str());
//    } else if (tptz__ContinuousMove->Velocity->PanTilt->x < 0) {
//        system(OSRVD_ST.options()["ptz"]["move_left"].asString().c_str());
//    }
//    if (tptz__ContinuousMove->Velocity->PanTilt->y > 0) {
//        system(OSRVD_ST.options()["ptz"]["move_up"].asString().c_str());
//    } else if (tptz__ContinuousMove->Velocity->PanTilt->y < 0) {
//        system(OSRVD_ST.options()["ptz"]["move_down"].asString().c_str());
//    }
}
SOAP_METHOD_END
/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(RelativeMove)
{
//    ServiceContext* ctx = (ServiceContext*)this->soap->user;

//    if (tptz__RelativeMove == NULL) {
//        return SOAP_OK;
//    }
//    if (tptz__RelativeMove->Translation == NULL) {
//        return SOAP_OK;
//    }
//    if (tptz__RelativeMove->Translation->PanTilt == NULL) {
//        return SOAP_OK;
//    }

//    if (tptz__RelativeMove->Translation->PanTilt->x > 0) {
//        system(OSRVD_ST.options()["ptz"]["move_right"].asString().c_str());
//        usleep(300000);
//        system(OSRVD_ST.options()["ptz"]["move_stop"].asString().c_str());
//    } else if (tptz__RelativeMove->Translation->PanTilt->x < 0) {
//        system(OSRVD_ST.options()["ptz"]["move_left"].asString().c_str());
//        usleep(300000);
//        system(OSRVD_ST.options()["ptz"]["move_stop"].asString().c_str());
//    }
//    if (tptz__RelativeMove->Translation->PanTilt->y > 0) {
//        system(OSRVD_ST.options()["ptz"]["move_up"].asString().c_str());
//        usleep(300000);
//        system(OSRVD_ST.options()["ptz"]["move_stop"].asString().c_str());
//    } else if (tptz__RelativeMove->Translation->PanTilt->y < 0) {
//        system(OSRVD_ST.options()["ptz"]["move_down"].asString().c_str());
//        usleep(300000);
//        system(OSRVD_ST.options()["ptz"]["move_stop"].asString().c_str());
//    }
}
SOAP_METHOD_END
/* ================================================================================================================================================================================ */
SOAP_METHOD_BEGIN(Stop)
{
//    ServiceContext* ctx = (ServiceContext*)this->soap->user;
//    system(OSRVD_ST.options()["ptz"]["move_stop"].asString().c_str());
}
SOAP_METHOD_END

MASK_UNUSED_SOAP_CALL(AbsoluteMove)
MASK_UNUSED_SOAP_CALL(CreatePresetTour)
MASK_UNUSED_SOAP_CALL(GetCompatibleConfigurations)
MASK_UNUSED_SOAP_CALL(GetConfiguration)
MASK_UNUSED_SOAP_CALL(GetConfigurationOptions)
MASK_UNUSED_SOAP_CALL(GetConfigurations)
MASK_UNUSED_SOAP_CALL(GetPresetTour)
MASK_UNUSED_SOAP_CALL(GetPresetTourOptions)
MASK_UNUSED_SOAP_CALL(GetPresetTours)
MASK_UNUSED_SOAP_CALL(GetServiceCapabilities)
MASK_UNUSED_SOAP_CALL(GetStatus)
MASK_UNUSED_SOAP_CALL(ModifyPresetTour)
MASK_UNUSED_SOAP_CALL(OperatePresetTour)
MASK_UNUSED_SOAP_CALL(RemovePreset)
MASK_UNUSED_SOAP_CALL(RemovePresetTour)
MASK_UNUSED_SOAP_CALL(SendAuxiliaryCommand)
MASK_UNUSED_SOAP_CALL(SetConfiguration)
MASK_UNUSED_SOAP_CALL(SetHomePosition)
MASK_UNUSED_SOAP_CALL(SetPreset)
