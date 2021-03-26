#ifndef CCAMERAS_H
#define CCAMERAS_H

#include <string>
#include <vector>

#include "ccameraprofile.h"

namespace osrvd
{
namespace entity
{


class CCameras
{
public:
    CCameras();
    virtual ~CCameras();
    CCameraProfile *camera(const int &inex);
    CCameraStream *findStream(const std::string &token);
    int count();
    int totalStreamsCount();
    bool check();
private:
    std::vector<CCameraProfile *> m_cameras;
};

}
}
#endif // CCAMERAS_H
