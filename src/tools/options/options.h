#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include <json/value.h>

namespace osrvd
{
namespace tools
{
namespace options
{

class Options
{
  public:
    Options();
    ~Options();
    void load(const std::string &filename);
    bool check();
    Json::Value& operator[](const std::string name) { return m_root[name]; }

  private:
    Json::Value m_root;
};

}
}
}
#endif // OPTIONS_H
