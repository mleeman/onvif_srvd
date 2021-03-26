#ifndef DEFINES_HEADER
#define DEFINES_HEADER

#define OSRVD_NAME "onvif_srvd"
#define OSRVD_MAJOR_VERSION 2
#define OSRVD_MINOR_VERSION 0
#define OSRVD_PATCH_VERSION 0


#define OSRVD_DEF_TO_STR_(text) #text
#define OSRVD_DEF_TO_STR(arg) OSRVD_DEF_TO_STR_(arg)


#define OSRVD_MAJOR_VERSION_STR  OSRVD_DEF_TO_STR(OSRVD_MAJOR_VERSION)
#define OSRVD_MINOR_VERSION_STR  OSRVD_DEF_TO_STR(OSRVD_MINOR_VERSION)
#define OSRVD_PATCH_VERSION_STR  OSRVD_DEF_TO_STR(OSRVD_PATCH_VERSION)

#define OSRVD_VERSION_STR  OSRVD_MAJOR_VERSION_STR "." \
                            OSRVD_MINOR_VERSION_STR "." \
                            OSRVD_PATCH_VERSION_STR

#endif //DEFINES_HEADER
