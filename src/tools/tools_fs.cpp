#include "tools_fs.h"
#include <experimental/filesystem>

namespace osrvd
{
namespace tools
{
namespace fs
{

void create_path(const std::string &path)
{
    std::experimental::filesystem::create_directories(path);
}

void delete_path(const std::string &path)
{
    std::experimental::filesystem::remove_all(path);
}

std::string extract_filename(const std::string &path)
{
    // https://stackoverflow.com/questions/8520560/get-a-file-name-from-a-path
    return std::experimental::filesystem::path(path).stem();
}

bool file_exists(const std::string &path)
{
    return std::experimental::filesystem::exists(path);
}

std::string temp_path()
{
    return std::experimental::filesystem::temp_directory_path();
}

}
}
}
