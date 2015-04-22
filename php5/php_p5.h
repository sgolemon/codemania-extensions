#ifndef incl_PHP_P5_H
#define incl_PHP_P5_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"

extern zend_module_entry p5_module_entry;
#define phpext_p5_ptr &p5_module_entry

typedef struct _p5_myclass_object {
  /* “Inherit” normal object structure */
  zend_object obj;

  /* Additional fields local to us */
  char *name;
  char *salutation;
} p5_myclass_object;

int p5_myclass_init(INIT_FUNC_ARGS);

#endif	/* incl_PHP_P5_H */
