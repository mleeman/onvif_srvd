#include "ccameraprofile.h"
#include "soap_defines.h"
#include "st.h"

namespace osrvd
{
namespace entity
{

CCameraProfile::CCameraProfile(const int &cameraIndex) :
    m_cameraIndex(cameraIndex)
{
    for ( int index = 0; index < OSRVD_ST.options()["cameras"][m_cameraIndex]["streams"].size(); ++index )
    {
       m_streams.push_back(new CCameraStream(m_cameraIndex, index));
    }
    m_ptz = new CCameraPTZ(m_cameraIndex);
}

CCameraProfile::~CCameraProfile()
{
    for(CCameraStream *i_stream : m_streams)
    {
        delete i_stream;
    }
    m_streams.clear();
    delete m_ptz;
}

std::string CCameraProfile::name() const
{
    return OSRVD_ST.options()["cameras"][m_cameraIndex]["name"].asString();
}

std::string CCameraProfile::typeString()
{
    return OSRVD_ST.options()["cameras"][m_cameraIndex]["type"].asString();
}

tt__VideoEncoding CCameraProfile::type()
{
    std::string s_type = typeString();
//    OSRVD_LOG_DBG("Stream coding: " << s_type);
    if     (s_type == "JPEG" ) { return tt__VideoEncoding::JPEG;  }
    else if(s_type == "MPEG4") { return tt__VideoEncoding::MPEG4; }
    else if(s_type == "H264" ) { return tt__VideoEncoding::H264;  }
    OSRVD_LOG_ERR("Type " << s_type << " dont support");
    return static_cast<tt__VideoEncoding>(100);
}

std::string CCameraProfile::snapshotUrl()
{
    return OSRVD_ST.options()["cameras"][m_cameraIndex]["snapshot_url"].asString();
}

CCameraStream *CCameraProfile::stream(const int &index)
{
    return m_streams[index];
}

CCameraPTZ *CCameraProfile::ptz()
{
    return m_ptz;
}

int CCameraProfile::streams()
{
    return m_streams.size();
}

bool CCameraProfile::check()
{
    bool result = true;
    if(name().empty())
    {
        OSRVD_LOG_ERR("Camera name is empty");
        result = false;
    }
    if(!m_streams.size())
    {
        OSRVD_LOG_ERR("No one stream for listing at camera " << name());
        result = false;
    }
    if(type() == static_cast<tt__VideoEncoding>(100))
    {
        OSRVD_LOG_ERR("Camera stream type " << typeString() << "  at camera" << name() << " unsupported. Supported types: JPEG, MPEG4, H264")
        result = false;
    }
    for(CCameraStream *i_stream : m_streams)
    {
        result = result && i_stream->check();
    }
    return result;
}




}
}
