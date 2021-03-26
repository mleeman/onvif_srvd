#ifndef CCAMERASTREAM_H
#define CCAMERASTREAM_H

#include <string>
#include "onvifMediaBindingService.h"

namespace osrvd
{
namespace entity
{

class CCameraProfile;

class CCameraStream
{
public:
    CCameraStream(const int &cameraIndex, const int &streamIndex);
    virtual ~CCameraStream();

    std::string name() const;
    std::string profileName() const;
    std::string encoderName() const;
    std::string token() const;
    std::string sourceToken() const;
    std::string url() const;
    CCameraProfile *parentCamera() const;
    int width() const;
    int height() const;
    int framerate() const;
    bool check();

    tt__VideoSource *videoSource() const;
    tt__VideoSourceConfiguration *videoSourceConfiguration() const;
    tt__VideoEncoderConfiguration *videoEncoderConfiguration() const;
    tt__Profile *profile() const;
private:
    int m_cameraIndex;
    int m_streamIndex;

};

}
}
#endif // CCAMERASTREAM_H
