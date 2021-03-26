#ifndef STREAMPROFILE_H
#define STREAMPROFILE_H

#include <string>
#include "soapH.h"

class StreamProfile
{
    public:

        StreamProfile() { clear(); }

        std::string  get_name   (void) const { return name;   }
        int          get_width  (void) const { return width;  }
        int          get_height (void) const { return height; }
        std::string  get_url    (void) const { return url;    }
        std::string  get_snapurl(void) const { return snapurl;}
        int          get_type   (void) const { return type;   }


        tt__Profile*     get_profile(struct soap *soap) const;
        tt__VideoSource* get_video_src(struct soap *soap) const;


        //methods for parsing opt from cmd
        bool set_name   (const char *new_val);
        bool set_width  (const char *new_val);
        bool set_height (const char *new_val);
        bool set_url    (const char *new_val);
        bool set_snapurl(const char *new_val);
        bool set_type   (const char *new_val);


        std::string get_str_err()  const { return str_err;         }
        const char* get_cstr_err() const { return str_err.c_str(); }

        void clear(void);
        bool is_valid(void) const;


    private:

        std::string  name;
        int          width;
        int          height;
        std::string  url;
        std::string  snapurl;
        int          type;


        std::string  str_err;


        tt__VideoSourceConfiguration*  get_video_src_cnf(struct soap *soap) const;
        tt__VideoEncoderConfiguration* get_video_enc_cfg(struct soap *soap) const;
        tt__PTZConfiguration*          get_ptz_cfg(struct soap *soap) const;
};


#endif // STREAMPROFILE_H
