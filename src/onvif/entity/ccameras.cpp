#include "ccameras.h"
#include "st.h"

namespace osrvd
{
namespace entity
{


CCameras::CCameras()
{
    for ( int index = 0; index < OSRVD_ST.options()["cameras"].size(); ++index )
    {
       m_cameras.push_back(new CCameraProfile(index));
    }
}

CCameras::~CCameras()
{
    for(CCameraProfile *i_camera : m_cameras)
    {
        delete i_camera;
    }
    m_cameras.clear();
}

CCameraProfile *CCameras::camera(const int &inex)
{
    return m_cameras[inex];
}

CCameraStream *CCameras::findStream(const std::string &token)
{
    for(CCameraProfile *i_camera : m_cameras)
    {
        for (int i_stream = 0; i_stream < i_camera->streams(); i_stream++)
        {
            if(i_camera->stream(i_stream)->token() == token)
            {
                return i_camera->stream(i_stream);
            }
        }
    }
    OSRVD_LOG_DBG("Can not find stream token: " << token);
    return nullptr;
}

int CCameras::count()
{
    return m_cameras.size();
}

int CCameras::totalStreamsCount()
{
    int count = 0;
    for(CCameraProfile *i_camera : m_cameras)
    {
        count += i_camera->streams();
    }
    return count;
}

bool CCameras::check()
{
    bool result = true;
    if(!m_cameras.size())
    {
        OSRVD_LOG_ERR("No one camera for listing");
        result = false;
    }
    for(CCameraProfile *i_camera : m_cameras)
    {
        result = result && i_camera->check();
    }
    return result;
}

}
}
