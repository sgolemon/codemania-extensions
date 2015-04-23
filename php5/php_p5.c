#include "php_p5.h"

static PHP_FUNCTION(p5_hello_world) {
	php_printf("Hello World\n");
}

static zend_function_entry p5_functions[] = {
	PHP_FE(p5_hello_world, NULL)
	PHP_FE_END
};

/* REGISTER_NULL_CONSTANT and REGISTER_BOOL_CONSTANT
 * don't exist prior to PHP-5.6
 * Use these defines if you need them anyway.
 */
#ifndef REGISTER_NULL_CONSTANT
#define REGISTER_NULL_CONSTANT(cnsname, cnsflags) { \
	zend_constant c; \
	ZVAL_NULL(&c.value); \
	c.flags = cnsflags; \
	c.name = pestrdup(#cnsname, (cnsflags & CONST_PERSISTENT)); \
	c.name_len = strlen(#cnsname); \
	c.module_number = module_number; \
	zend_register_constant(&c TSRMLS_CC); \
}
#endif

#ifndef REGISTER_BOOL_CONSTANT
#define REGISTER_BOOL_CONSTANT(cnsname, cnsval, cnsflags) { \
	zend_constant c; \
	ZVAL_BOOL(&c.value, cnsval); \
	c.flags = cnsflags; \
	c.name = pestrdup(#cnsnamei, (cnsflags & CONST_PERSISTENT)); \
	c.name_len = strlen(#cnsname); \
	c.module_number = module_number; \
	zend_register_constant(&c TSRMLS_CC); \
}
#endif

static PHP_MINIT_FUNCTION(p5) {
	REGISTER_NULL_CONSTANT("P5_NOTHING",                  CONST_CS | CONST_PERSISTENT);
	REGISTER_BOOL_CONSTANT("P5_TRUTH",    1,              CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("P5_VERSIION", 0x010000,       CONST_CS | CONST_PERSISTENT);
	REGISTER_DOUBLE_CONSTANT("P5_PI",     3.1415926535,   CONST_CS | CONST_PERSISTENT);
	REGISTER_STRING_CONSTANT("P5_NAME",   "p5 extension",            CONST_PERSISTENT);

	return SUCCESS;
}

zend_module_entry p5_module_entry = {
	STANDARD_MODULE_HEADER,
	"p5", /* name */
	p5_functions,
	PHP_MINIT(p5),
	NULL, /* MSHUTDOWN */
	NULL, /* RINIT */
	NULL, /* RSHUTDOWN */
	NULL, /* MINFO */
	"0.0.1", /* version */
	STANDARD_MODULE_PROPERTIES,
};

#ifdef COMPILE_DL_P5
ZEND_GET_MODULE(p5)
#endif

