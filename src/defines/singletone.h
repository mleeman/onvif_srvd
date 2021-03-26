#ifndef SINGLETONE_DEFINES_H
#define SINGLETONE_DEFINES_H

#define OSRVD_STN_DEFINITION_TMPL(_type,_name) \
    public: _type &_name()

#define OSRVD_STN_DECLARATION_TMPL(_type) \
    { \
        static _type value = _type(); \
        return value; \
    }

#define OSRVD_STN_DEFINITION(_type,_name) \
    OSRVD_STN_DEFINITION_TMPL(_type,_name);

#define OSRVD_STN_DECLARATION(_target_type,_st_type,_name) \
    _target_type &_st_type::_name() \
    OSRVD_STN_DECLARATION_TMPL(_target_type)

#define OSRVD_SIMPLE_SINGLTON_DECLARATION(_type,_name) \
    OSRVD_STN_DEFINITION_TMPL(_type,_name) \
    OSRVD_STN_DECLARATION_TMPL(_type)

#endif // SINGLETONE_DEFINES_H
