#ifndef SOAP_DEFINES_HEAD_H
#define SOAP_DEFINES_HEAD_H

#define OSRVD_MASK_UNUSED_SOAP_CALL_DECLARE(_method, _parametr_prefix) \
    int _method \
        ( \
        _##_parametr_prefix##__##_method *_parametr_prefix##__##_method, \
        _##_parametr_prefix##__##_method##Response &_parametr_prefix##__##_method##Response \
        ) final;


#endif // SOAP_DEFINES_HEAD_H
