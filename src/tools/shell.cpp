#include "shell.h"
#include <iostream>
#include <stdio.h>
#include "st.h"

namespace osrvd
{
namespace tools
{
namespace shell
{

void exec(const std::string &command)
{
    char buffer[128];
    std::string result = "";
    OSRVD_LOG_ERR("Executing command: " << command);
    FILE* pipe = popen(command.c_str(), "r");
    if (pipe)
    {
        try
        {
            while (fgets(buffer, sizeof buffer, pipe) != NULL)
            {
                result += buffer;
            }
        }
        catch (...)
        {
            pclose(pipe);
            OSRVD_LOG_ERR("Executing failed: " << command);
            if(!result.empty())
            {
                OSRVD_LOG_ERR("Command output: " << result);
            }
            throw;
        }
        pclose(pipe);
        OSRVD_LOG_NFO("Executing done: " << command);
        OSRVD_LOG_NFO("Command output: " << result);
        return;
    }
    OSRVD_LOG_ERR("Executing failed: " << command);
}


}
}
}
