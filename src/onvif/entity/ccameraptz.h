#ifndef CCAMERAPTZ_H
#define CCAMERAPTZ_H

#include <string>
#include "onvifMediaBindingService.h"

namespace osrvd
{
namespace entity
{

class CCameraPTZ
{
public:
    CCameraPTZ(const int &cameraIndex);
    virtual ~CCameraPTZ();

    bool enabled();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void moveStop();
    void movePreset();

private:
    int m_cameraIndex;
    void executePtz(const std::string &name);
};

}
}
#endif // CCAMERAPTZ_H
