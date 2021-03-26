#ifndef FS_UTILS_H
#define FS_UTILS_H
#include <string>

namespace osrvd
{
namespace tools
{
namespace fs
{

std::string temp_path();
void create_path(const std::string &path);
void delete_path(const std::string &path);
std::string extract_filename(const std::string &path);
bool file_exists(const std::string &path);

}
}
}
#endif // FS_UTILS_H
