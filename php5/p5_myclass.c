#include "php_p5.h"

zend_class_entry *php_p5_myclass_ce;
static zend_object_handlers p5_myclass_handlers;

static PHP_METHOD(MyClass, foo) {
  RETURN_STRING("bar");
}

static PHP_METHOD(MyClass, __construct) {
  p5_myclass_object *objval = (p5_myclass_object*)Z_OBJ_P(getThis());
  char *name, *salutation = "Ms.";
  int name_len, sal_len = strlen(salutation);

  if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|s",
                            &name, &name_len, &salutation, &sal_len) == FAILURE) {
    return;
  }
  objval->name = estrndup(name, name_len);
  objval->salutation = estrndup(salutation, sal_len);
}

static PHP_METHOD(MyClass, setName) {
  p5_myclass_object *objval = (p5_myclass_object*)Z_OBJ_P(getThis());
  char *name;
  int name_len;
  if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &name, &name_len) == FAILURE) {
    return;
  }
  if (objval->name) efree(objval->name);
  objval->name = estrndup(name, name_len);
}

static PHP_METHOD(MyClass, getName) {
  p5_myclass_object *objval = (p5_myclass_object*)Z_OBJ_P(getThis());
  if (objval->name) {
    RETURN_STRING(objval->name);
  } else {
    RETURN_EMPTY_STRING();
  }
}

static zend_function_entry p5_myclass_methods[] = {
  PHP_ME(MyClass, foo, NULL /* arginfo */, ZEND_ACC_PUBLIC)
  PHP_ME(MyClass, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
  PHP_ME(MyClass, getName, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(MyClass, setName, NULL, ZEND_ACC_PUBLIC)
  PHP_FE_END
};

static void p5_myclass_destroy(p5_myclass_object *objval) {
  if (objval->name) efree(objval->name);
  if (objval->salutation) efree(objval->salutation);
  zend_object_std_dtor(&(objval->obj));
  efree(objval);
}

static zend_object* p5_myclass_object_create(zend_class_entry *ce) {
  p5_myclass_object *objval = ecalloc(1, sizeof(p5_myclass_object));
  zend_object_std_init(&objval->obj, ce);
  return (zend_object*)objval;
}

int p5_myclass_init(INIT_FUNC_ARGS) {
  zend_class_entry ce;

  INIT_CLASS_ENTRY(ce, ZEND_NS_NAME("P5", "MyClass"), p5_myclass_methods);
  php_p5_myclass_ce = zend_register_internal_class(&ce);
  php_p5_myclass_ce->create_object = p5_myclass_object_create;

  zend_declare_property_null(php_p5_myclass_ce, "nulp", strlen("nulp"),
                             ZEND_ACC_PUBLIC);
  zend_declare_property_long(php_p5_myclass_ce, "life", strlen("life"), 42,
                             ZEND_ACC_PRIVATE | ZEND_ACC_STATIC);

  zend_declare_class_constant_double(php_p5_myclass_ce,
                                     "PI", strlen("PI"), 3.1415926535);

  return SUCCESS;
}
