#include "php_p5.h"

zend_module_entry p5_module_entry = {
	STANDARD_MODULE_HEADER,
	"p5", /* name */
	NULL, /* functions */
	NULL, /* MINIT */
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

