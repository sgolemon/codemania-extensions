#include "php_p5.h"

zend_class_entry *php_p5_myclass_ce;

static PHP_METHOD(MyClass, foo) {
  RETURN_STRING("bar", 1);
}

static PHP_METHOD(MyClass, getName) {
  zval *obj = getThis();
  zval *name = zend_read_property(php_p5_myclass_ce, obj,
                                  "name", strlen("name"), 0 TSRMLS_CC);

  ZVAL_ZVAL(return_value, name, 1, 0);
}

static PHP_METHOD(MyClass, setName) {
  zval *obj = getThis();
  char *name;
  int name_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
    return;
  }

  zend_update_property_stringl(php_p5_myclass_ce, obj, "name", strlen("name"),
                               name, name_len TSRMLS_CC);
}

static zend_function_entry p5_myclass_methods[] = {
  PHP_ME(MyClass, foo, NULL /* arginfo */, ZEND_ACC_PUBLIC)
  PHP_ME(MyClass, getName, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(MyClass, setName, NULL, ZEND_ACC_PUBLIC)
  PHP_FE_END
};

int p5_myclass_init(INIT_FUNC_ARGS) {
  zend_class_entry ce;

  INIT_CLASS_ENTRY(ce, ZEND_NS_NAME("P5", "MyClass"), p5_myclass_methods);
  php_p5_myclass_ce = zend_register_internal_class(&ce TSRMLS_CC);

  zend_declare_property_null(php_p5_myclass_ce, "nulp", strlen("nulp"),
                             ZEND_ACC_PUBLIC TSRMLS_CC);
  zend_declare_property_long(php_p5_myclass_ce, "life", strlen("life"), 42,
                             ZEND_ACC_PRIVATE | ZEND_ACC_STATIC TSRMLS_CC);
  zend_declare_property_string(php_p5_myclass_ce, "name", strlen("name"), "bob",
                             ZEND_ACC_PROTECTED TSRMLS_CC);

  zend_declare_class_constant_double(php_p5_myclass_ce,
                                     "PI", strlen("PI"), 3.1415926535 TSRMLS_CC);

  return SUCCESS;
}
