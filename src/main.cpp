#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "st.h"
#include "cmdline.h"

// ---- gsoap ----
#include "DeviceBinding.nsmap"

#include "cethernetdevice.h"

void check()
{
    if(!OSRVD_ST.options().check())
    {
        OSRVD_ST.quit("Wrong options in config file");
    }
    if(!OSRVD_ST.camears().check())
    {
        OSRVD_ST.quit("Wrong cameras configuration");
    }
    if(!OSRVD_ST.eth().check())
    {
        OSRVD_ST.quit("Wrong network configuration");
    }
}


int main(int argc, char *argv[])
{
    OSRVD_LOG_DBG("Starting");
    processing_cmd(argc, argv);
    check();
    if(!OSRVD_ST.soap().run()) { return EXIT_FAILURE; }
    return 0;
}
