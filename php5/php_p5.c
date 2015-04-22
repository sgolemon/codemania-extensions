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
  RETURN_STRING("Hello");
}

ZEND_BEGIN_ARG_INFO_EX(p5_add_arginfo, 0, ZEND_RETURN_VALUE, 2)
  ZEND_ARG_INFO(0, a)
  ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO();

static PHP_FUNCTION(p5_add) {
  long a, b;

  if (zend_parse_parameters(ZEND_NUM_ARGS(),
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
  if (zend_parse_parameters(ZEND_NUM_ARGS(),
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

  if (zend_parse_parameters(ZEND_NUM_ARGS(),
                            "z", &name) == FAILURE) {
    return;
  }

  zval_dtor(name);
  ZVAL_STRING(name, "Rumor has it, hi");
}

PHP_FUNCTION(p5_enum_greetings) {
  zval *es_greetings;
  array_init(return_value);    // Initialize rv as empty array

  add_index_string(return_value, 0,   "Hello");       // $ret[0]   = "Hello";
  add_next_index_string(return_value, "Hola");        // $ret[]    = "Hola";
  add_assoc_string(return_value, "2", "Bonjour");     // $ret["2"] = "Bonjour";

  add_next_index_null(return_value);                  // $ret[] = null;
  add_next_index_bool(return_value, 1);               // $ret[] = true;
  add_next_index_long(return_value, 42);              // $ret[] = 42;
  add_next_index_double(return_value, 3.1415926535);  // $ret[] = 3.1415926535;
  add_next_index_string(return_value, "Konichiwa");   // $ret[] = "Konichiwa";

  MAKE_STD_ZVAL(es_greetings);
  array_init(es_greetings);                               // $sg = array();
  add_next_index_string(es_greetings, "Hola");            // $sg[] = "Hola";
  add_next_index_string(es_greetings, "Saludos");         // $sg[] = "Saludos";
  add_next_index_string(es_greetings, "¿Qué Tal?");       // $sg[] = "¿Qué Tal?";
  add_assoc_zval(return_value, "spanish", es_greetings);  // $ret["spanish"] = $sg;
}

ZEND_BEGIN_ARG_INFO_EX(p5_array_arginfo, 0, ZEND_RETURN_VALUE, 1)
	ZEND_ARG_ARRAY_INFO(0, arr, 0)
ZEND_END_ARG_INFO();
PHP_FUNCTION(p5_array) {
  HashTable *arr;
  zval *pz;
  char *key = "123";

  if (zend_parse_parameters(ZEND_NUM_ARGS(),
                            "h", &arr) == FAILURE) { return; }

  if (zend_hash_str_exists(arr, "key", sizeof("key"))) {
    php_printf("$arr['key'] is set\n");
  }
  if ((pz = zend_hash_str_find(arr, "key", sizeof("key")))) {
    php_printf("$arr['key'] is %s\n", (Z_TYPE_P(pz) == IS_STRING)
                                    ? Z_STRVAL_P(pz) : "not a string");
  }
  if (zend_symtable_str_exists(arr, key, strlen(key) + 1)) {
    php_printf("$arr['%s'] is set\n", key);
  }
}

static PHP_FUNCTION(p5_greet_all) {
  HashTable *arr;
  HashPosition pos;
  zval *pz;

  if (zend_parse_parameters(ZEND_NUM_ARGS(),
                            "h", &arr) == FAILURE) { return; }

  for (zend_hash_internal_pointer_reset_ex(arr, &pos);
       pz = zend_hash_get_current_data_ex(arr, &pos);
       zend_hash_move_forward_ex(arr, &pos)) {
    zend_string *key;
    zend_ulong idx;
    int key_type = zend_hash_get_current_key_ex(arr, &key, &idx, &pos);
    zval copy;

    if (key_type == HASH_KEY_IS_STRING) {
      printf("%s ", key->val);
    } else {
      printf("%ld ", idx);
    }

    ZVAL_ZVAL(&copy, pz, 1, 0);
    convert_to_string(&copy);
    printf("=> %s\n", Z_STRVAL(copy));
    zval_dtor(&copy);
  }
}

static int p5_greeter(zval* pz) {
  if (Z_TYPE_P(pz) == IS_STRING) {
    printf("=> %s\n", Z_STRVAL_P(pz));
  } else {
    zval copy;
    ZVAL_ZVAL(&copy, pz, 1, 0);
    convert_to_string(&copy);
    printf("=> %s\n", Z_STRVAL(copy));
    zval_dtor(&copy);
  }
  return ZEND_HASH_APPLY_KEEP;
}

static PHP_FUNCTION(p5_greet_all2) {
  HashTable *arr;

  if (zend_parse_parameters(ZEND_NUM_ARGS(),
                            "h", &arr) == FAILURE) { return; }

  zend_hash_apply(arr, p5_greeter);
}

static zend_function_entry p5_functions[] = {
	PHP_FE(p5_hello_world, NULL)
	PHP_FE(p5_pi, NULL)
	PHP_FE(p5_get_greeting, NULL)
	PHP_FE(p5_add, p5_add_arginfo)
	PHP_FE(p5_greet, p5_greet_arginfo)
	PHP_FE(p5_greeting_by_ref, p5_greeting_by_ref_arginfo)
	PHP_FE(p5_enum_greetings, NULL)
	PHP_FE(p5_array, p5_array_arginfo)
	PHP_FE(p5_greet_all, NULL)
	PHP_FE(p5_greet_all2, NULL)
	PHP_FE_END
};

static PHP_MINIT_FUNCTION(p5) {
	REGISTER_NULL_CONSTANT("P5_NOTHING",                  CONST_CS | CONST_PERSISTENT);
	REGISTER_BOOL_CONSTANT("P5_TRUTH",    1,              CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("P5_VERSIION", 0x010000,       CONST_CS | CONST_PERSISTENT);
	REGISTER_DOUBLE_CONSTANT("P5_PI",     3.1415926535,   CONST_CS | CONST_PERSISTENT);
	REGISTER_STRING_CONSTANT("P5_NAME",   "p5 extension",            CONST_PERSISTENT);

	return p5_myclass_init(INIT_FUNC_ARGS_PASSTHRU);
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

