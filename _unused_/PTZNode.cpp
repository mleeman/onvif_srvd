#include "PTZNode.h"

void PTZNode::clear()
{
    enable = false;

    move_left.clear();
    move_right.clear();
    move_up.clear();
    move_down.clear();
    move_stop.clear();
    move_preset.clear();
}



bool PTZNode::set_str_value(const char* new_val, std::string& value)
{
    if(!new_val)
    {
        str_err = "Process is empty";
        return false;
    }


    value = new_val;
    return true;
}
