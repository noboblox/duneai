#ifndef DELETE_DEFAULTS_H_
#define DELETE_DEFAULTS_H_

#define MACRO_DELETE_ALL_DEFAULTS(CLASS_TYPE__)                  \
	              CLASS_TYPE__   (      CLASS_TYPE__&&) = delete;\
                  CLASS_TYPE__   (const CLASS_TYPE__&)  = delete;\
    CLASS_TYPE__& operator=      (      CLASS_TYPE__&&) = delete;\
    CLASS_TYPE__& operator=      (const CLASS_TYPE__&)  = delete;

#endif
