#include "hphp/runtime/ext/extension.h"
#include "hphp/runtime/base/execution-context.h"
#include "hphp/runtime/vm/native-data.h"
#include <bzlib.h>

using namespace HPHP;

class MyClassData {
 public:
  // Default constructor is invoked whenever
  // an instance of this class is created
  // just prior to the constructor being called
  MyClassData(): counter(0) {}

  // Assignment operator is invoked to create
  // a new instance from another one via cloning.
  MyClassData& operator=(const MyClassData& src) {
    counter = src.counter;
    return *this;
  }

  // If a destructor is defined, it is invoked
  // when the object falls out of scope
  ~MyClassData() {}

  int64_t counter;
};

static String HHVM_FUNCTION(h3_bzlib_version) {
#ifdef HAVE_BZ2_VERSION
  return BZ2_bzlibVersion();
#else
  return "Unknown";
#endif
}

static int64_t HHVM_FUNCTION(h3_add, int64_t a, int64_t b) {
  return a + b;
}

const StaticString
  s_Hello("Hello"),
  s_Hola("Hola"),
  s_Bonjour("Bonjour"),
  s_Konichiwa("Konichiwa"),
  s_Saludos("Saludos"),
  s_QueTal("¿Qué Tal?"),
  s_spanish("spanish");

static Array HHVM_FUNCTION(h3_enum_greetings) {
  auto ret = Array::Create();

  ret.set(0, s_Hello);
  ret.append(s_Hola);
  ret.set(2, s_Bonjour);

  ret.append(init_null());
  ret.append(true);
  ret.append(42);
  ret.append(3.1415926535);
  ret.append(s_Konichiwa);

  auto es = Array::Create();
  es.append(s_Hola);
  es.append(s_Saludos);
  es.append(s_QueTal);
  ret.set(s_spanish, es);

  return ret;
}

static void HHVM_FUNCTION(h3_iterate, const Array& arr) {
  for (ArrayIter iter(arr); iter; ++iter) {
    g_context->write(iter.first().toString());
    g_context->write(" => ");
    g_context->write(iter.second().toString());
    g_context->write("\n");
  }
}

static Variant HHVM_METHOD(MyClass, foo, int64_t bar) {
  if (bar > 0) {
    Variant v = bar;
    return v.toString();
  } else {
    raise_warning("MyClass::foo() expects a positive value");
    return init_null();
  }
}

static Variant HHVM_STATIC_METHOD(MyClass, numeric, const String& val) {
  int64_t ival;
  double dval;
  auto dt = val.get()->toNumeric(ival, dval);
  if (dt == KindOfInt64) {
    return ival;
  } else if (dt == KindOfDouble) {
    return dval;
  } else {
    return false;
  }
}

static int64_t HHVM_METHOD(MyClass, get) {
  return Native::data<MyClassData>(this_)->counter;
}

static int64_t HHVM_METHOD(MyClass, inc) {
  return ++Native::data<MyClassData>(this_)->counter;
}

static int64_t HHVM_METHOD(MyClass, dec) {
  return --Native::data<MyClassData>(this_)->counter;
}

// StaticStrings hold references to constant strings
// in a way the engine can use
const StaticString
  s_H3_INTY("H3_INTY"),
  s_H3_GREETING("H3_GREETING"),
  s_Hello_World("Hello World"),
  s_MyClass("MyClass");

class H3Extension : public Extension {
 public:
  H3Extension(): Extension("h3", "0.0.1") {}

  void moduleInit() override {
    Native::registerConstant<KindOfBoolean>(makeStaticString("H3_FALSY"), false);
    Native::registerConstant<KindOfInt64>(s_H3_INTY.get(), 123);
    Native::registerConstant<KindOfString>(s_H3_GREETING.get(), s_Hello_World.get());

    HHVM_FE(h3_bzlib_version);
    HHVM_FE(h3_add);
    HHVM_FE(h3_enum_greetings);
    HHVM_FE(h3_iterate);

    HHVM_ME(MyClass, foo);
    HHVM_STATIC_ME(MyClass, numeric);

    HHVM_ME(MyClass, get);
    HHVM_ME(MyClass, inc);
    HHVM_ME(MyClass, dec);

    Native::registerNativeDataInfo<MyClassData>(s_MyClass.get());

    loadSystemlib();
  }

} s_h3_extension;

HHVM_GET_MODULE(h3);
