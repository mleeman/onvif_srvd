#ifndef CCAMERAPROFILE_H
#define CCAMERAPROFILE_H

#include <string>
#include <vector>
#include "ccamerastream.h"
#include "ccameraptz.h"
#include "onvifMediaBindingService.h"

namespace osrvd
{
namespace entity
{

class CCameraProfile
{
public:
    CCameraProfile(const int &cameraIndex);
    virtual ~CCameraProfile();

    std::string name() const;
    tt__VideoEncoding type();
    std::string snapshotUrl();
    CCameraStream *stream(const int &index);
    CCameraPTZ *ptz();
    int streams();
    bool check();

private:
    std::string typeString();

    int m_cameraIndex;
    std::vector<CCameraStream *> m_streams;
    CCameraPTZ *m_ptz;
};

}
}
#endif // CCAMERAPROFILE_H
