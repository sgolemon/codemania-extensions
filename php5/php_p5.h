#ifndef incl_PHP_P5_H
#define incl_PHP_P5_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"

extern zend_module_entry p5_module_entry;
#define phpext_p5_ptr &p5_module_entry

#endif	/* incl_PHP_P5_H */
