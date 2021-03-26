#ifndef STRING_UTILS_H
#define STRING_UTILS_H
#include <string>
#include <vector>

namespace osrvd
{
namespace tools
{
namespace string
{

std::string rand_alnum_str (std::string::size_type sz);
std::string replace_all (const std::string &str, const std::string& from, const std::string& to);
bool contains (const std::string &str, const std::string &needle);
std::string join (const std::vector<std::string> &strings, const std::string &delimiter = ", ");
std::string formatted_current_datetime(const std::string &format);

}
}
}
#endif // STRING_UTILS_H
