#ifndef PTZNODE_H
#define PTZNODE_H

#include <string>

class PTZNode
{
    public:

        PTZNode() { clear(); }

        bool         enable;

        std::string  get_move_left   (void) const { return move_left;   }
        std::string  get_move_right  (void) const { return move_right;  }
        std::string  get_move_up     (void) const { return move_up;     }
        std::string  get_move_down   (void) const { return move_down;   }
        std::string  get_move_stop   (void) const { return move_stop;   }
        std::string  get_move_preset (void) const { return move_preset;   }



        //methods for parsing opt from cmd
        bool set_move_left   (const char *new_val) { return set_str_value(new_val, move_left  ); }
        bool set_move_right  (const char *new_val) { return set_str_value(new_val, move_right ); }
        bool set_move_up     (const char *new_val) { return set_str_value(new_val, move_up    ); }
        bool set_move_down   (const char *new_val) { return set_str_value(new_val, move_down  ); }
        bool set_move_stop   (const char *new_val) { return set_str_value(new_val, move_stop  ); }
        bool set_move_preset (const char *new_val) { return set_str_value(new_val, move_preset); }


        std::string get_str_err()  const { return str_err;         }
        const char* get_cstr_err() const { return str_err.c_str(); }

        void clear(void);


    private:

        std::string  move_left;
        std::string  move_right;
        std::string  move_up;
        std::string  move_down;
        std::string  move_stop;
        std::string  move_preset;


        std::string  str_err;

        bool set_str_value(const char *new_val, std::string& value);
};


#endif // PTZNODE_H
