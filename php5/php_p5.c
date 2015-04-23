#include "php_p5.h"

static PHP_FUNCTION(p5_hello_world) {
	php_printf("Hello World\n");
}

static PHP_FUNCTION(p5_pi) {
	/* Equivalent to:
	 * RETVAL_DOUBLE(3.141592635); return;
	 * or:
	 * ZVAL_DOUBLE(return_value, 3.1415926535); return;
	 */
	RETURN_DOUBLE(3.1415926535);
}


static PHP_FUNCTION(p5_get_greeting) {
  // Equivalent to first copying the string, then passing 0
  // The variable must "own" the string its given.
  // RETURN_STRING(estrdup("Hello"), 0);
  RETURN_STRING("Hello", 1);
}

ZEND_BEGIN_ARG_INFO_EX(p5_add_arginfo, 0, ZEND_RETURN_VALUE, 2)
  ZEND_ARG_INFO(0, a)
  ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO();

static PHP_FUNCTION(p5_add) {
  long a, b;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
                            "ll", &a, &b) == FAILURE) {
    return;
  }
  RETURN_LONG(a + b);
}

ZEND_BEGIN_ARG_INFO_EX(p5_greet_arginfo, 0, ZEND_RETURN_VALUE, 1)
  ZEND_ARG_INFO(0, name)
  ZEND_ARG_INFO(0, greet)
  ZEND_ARG_INFO(0, count)
ZEND_END_ARG_INFO();
static PHP_FUNCTION(p5_greet) {
  char *name;
  int name_len;
  zend_bool greet = 0;
  long count = 1;
  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
                            "s|bl", &name, &name_len,
                                    &greet, &count) == FAILURE) {
    return;
  }
  while (count-- > 0) {
    php_printf("%s ", greet ? "Hello" : "Goodbye");
    PHPWRITE(name, name_len);
    php_printf("\n");
  }
}

ZEND_BEGIN_ARG_INFO_EX(p5_greeting_by_ref_arginfo, 0, ZEND_RETURN_VALUE, 1)
  ZEND_ARG_INFO(1, greeting)
ZEND_END_ARG_INFO();
static PHP_FUNCTION(p5_greeting_by_ref) {
  zval *name;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
                            "z", &name) == FAILURE) {
    return;
  }

  zval_dtor(name);
  ZVAL_STRING(name, "Rumor has it, hi", 1);
}

PHP_FUNCTION(p5_enum_greetings) {
  zval *es_greetings;
  array_init(return_value);    // Initialize rv as empty array

  add_index_string(return_value, 0,   "Hello", 1);     // $ret[0]   = "Hello";
  add_next_index_string(return_value, "Hola", 1);      // $ret[]    = "Hola";
  add_assoc_string(return_value, "2", "Bonjour", 1);   // $ret["2"] = "Bonjour";

  add_next_index_null(return_value);                  // $ret[] = null;
  add_next_index_bool(return_value, 1);               // $ret[] = true;
  add_next_index_long(return_value, 42);              // $ret[] = 42;
  add_next_index_double(return_value, 3.1415926535);  // $ret[] = 3.1415926535;
  add_next_index_string(return_value, "Konichiwa", 1); // $ret[] = "Konichiwa";

  MAKE_STD_ZVAL(es_greetings);
  array_init(es_greetings);                               // $sg = array();
  add_next_index_string(es_greetings, "Hola", 1);         // $sg[] = "Hola";
  add_next_index_string(es_greetings, "Saludos", 1);      // $sg[] = "Saludos";
  add_next_index_string(es_greetings, "¿Qué Tal?", 1);    // $sg[] = "¿Qué Tal?";
  add_assoc_zval(return_value, "spanish", es_greetings);  // $ret["spanish"] = $sg;
}

static zend_function_entry p5_functions[] = {
	PHP_FE(p5_hello_world, NULL)
	PHP_FE(p5_pi, NULL)
	PHP_FE(p5_get_greeting, NULL)
	PHP_FE(p5_add, p5_add_arginfo)
	PHP_FE(p5_greet, p5_greet_arginfo)
	PHP_FE(p5_greeting_by_ref, p5_greeting_by_ref_arginfo)
	PHP_FE(p5_enum_greetings, NULL)
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

