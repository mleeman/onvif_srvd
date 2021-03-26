#include "ccameraptz.h"
#include "shell.h"
#include "soap_defines.h"
#include "st.h"

namespace osrvd
{
namespace entity
{

CCameraPTZ::CCameraPTZ(const int &cameraIndex) :
    m_cameraIndex(cameraIndex)
{}

CCameraPTZ::~CCameraPTZ()
{}

bool CCameraPTZ::enabled()
{
    return OSRVD_ST.options()["cameras"][m_cameraIndex].isMember("ptz")            &&
           OSRVD_ST.options()["cameras"][m_cameraIndex]["ptz"].isMember("enabled") &&
           OSRVD_ST.options()["cameras"][m_cameraIndex]["ptz"]["enabled"].asBool();
}

void CCameraPTZ::executePtz(const std::string &name)
{
    if(enabled())
    {
        if(OSRVD_ST.options()["cameras"][m_cameraIndex]["ptz"].isMember(name))
        {
            OSRVD_LOG_NFO("PTZ conmmand: " << name)
            std::string command = OSRVD_ST.options()["cameras"][m_cameraIndex]["ptz"][name].asString();
            if(!command.empty())
            {
                osrvd::tools::shell::exec(command);
            }
        }
    }
}

void CCameraPTZ::moveLeft()
{
    executePtz("move_left");
}

void CCameraPTZ::moveRight()
{
    executePtz("move_right");
}

void CCameraPTZ::moveUp()
{
    executePtz("move_up");
}

void CCameraPTZ::moveDown()
{
    executePtz("move_down");
}

void CCameraPTZ::moveStop()
{
    executePtz("move_stop");
}

void CCameraPTZ::movePreset()
{
    executePtz("move_preset");
}

}
}
