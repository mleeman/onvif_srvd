#include <getopt.h>

#include "cmdline.h"
#include "version.h"
#include "st.h"

static const char *help_str =
        " ===============  Help  ===============\n"
        " Version:  " OSRVD_VERSION_STR   "\n"
#ifdef  DEBUG
        " Build  mode:  debug\n"
#else
        " Build  mode:  release\n"
#endif
        " Build  date:  " __DATE__ "\n"
        " Build  time:  " __TIME__ "\n\n"
        "Options:                      description:\n\n"
        "       --tz_format    [value] Set Time Zone Format           (default = 0)\n"
        "  -v,  --version              Display daemon version\n"
        "  -h,  --help                 Display this help\n\n";

// indexes for long_opt function
namespace LongOpts
{
    enum
    {
        version = 'v',
        help    = 'h',
        pid_file,
        log_file,
        config = 'c'

        //ONVIF Service options (context)
        // tz_format,

    };
}

static const char *short_opts = "hv";


static const struct option long_opts[] =
{
    { "version",      no_argument,       NULL, LongOpts::version       },
    { "help",         no_argument,       NULL, LongOpts::help          },
    { "config",       required_argument, NULL, LongOpts::config        },

    //ONVIF Service options (context)
    // { "tz_format",    required_argument, NULL, LongOpts::tz_format     },

    { NULL,           no_argument,       NULL,  0                      }
};


void processing_cmd(int argc, char *argv[])
{
    int opt;
    while( (opt = getopt_long(argc, argv, short_opts, long_opts, NULL)) != -1 )
    {
        switch( opt )
        {

            case LongOpts::help:
            {
                puts(help_str);
                OSRVD_ST.quit();
            }
            break;

            case LongOpts::version:
            {
                OSRVD_LOG_NFO("Version: " OSRVD_VERSION_STR);
                OSRVD_ST.quit();
            }
            break;

            case LongOpts::config:
            {
                OSRVD_ST.options().load(optarg);
            }
            break;


                // case LongOpts::ifs:
                //             service_ctx.eth_ifs.push_back(Eth_Dev_Param());

                //             if( service_ctx.eth_ifs.back().open(optarg) != 0 )
                //                 daemon_error_exit("Can't open ethernet interface: %s - %m\n", optarg);

                //             break;

                // case LongOpts::tz_format:
                //             if( !service_ctx.set_tz_format(optarg) )
                //                 daemon_error_exit("Can't set tz_format: %s\n", service_ctx.get_cstr_err());

                //             break;

            default:
            {
                OSRVD_ST.quit("for more detail see help");
            }
            break;
        }
    }
}
