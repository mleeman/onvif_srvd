#include "options.h"
#include "st.h"
#include <json/json.h>
#include <fstream>
#include <istream>

#define OSRVD_CONFIG_HELP_LINK " See http://example.com/example_config.json for example"

namespace osrvd
{
namespace tools
{
namespace options
{

Options::Options()
{}

Options::~Options()
{}

void Options::load(const std::string &filename)
{
  OSRVD_LOG_DBG("Loading config")
  std::ifstream config_doc(filename, std::ifstream::binary);
  config_doc >> m_root;
}

bool Options::check()
{
  OSRVD_LOG_DBG("Checking config");
  bool result = true;
  if(!OSRVD_ST.options()["scopes"].size())
  {
      OSRVD_LOG_ERR("Error: not set 'scopes' configuration in config file." OSRVD_CONFIG_HELP_LINK);
      result = false;
  }
  return result;
}



}
}
}
