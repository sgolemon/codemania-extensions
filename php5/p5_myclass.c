#include "php_p5.h"

zend_class_entry *php_p5_myclass_ce;

static PHP_METHOD(MyClass, foo) {
  RETURN_STRING("bar", 1);
}

static zend_function_entry p5_myclass_methods[] = {
  PHP_ME(MyClass, foo, NULL /* arginfo */, ZEND_ACC_PUBLIC)
  PHP_FE_END
};

int p5_myclass_init(INIT_FUNC_ARGS) {
  zend_class_entry ce;

  INIT_CLASS_ENTRY(ce, ZEND_NS_NAME("P5", "MyClass"), p5_myclass_methods);
  php_p5_myclass_ce = zend_register_internal_class(&ce TSRMLS_CC);

  return SUCCESS;
}
