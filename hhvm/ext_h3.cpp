#include "hphp/runtime/ext/extension.h"

using namespace HPHP;

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


// StaticStrings hold references to constant strings
// in a way the engine can use
const StaticString
  s_H3_INTY("H3_INTY"),
  s_H3_GREETING("H3_GREETING"),
  s_Hello_World("Hello World");

class H3Extension : public Extension {
 public:
  H3Extension(): Extension("h3", "0.0.1") {}

  void moduleInit() override {
    Native::registerConstant<KindOfBoolean>(makeStaticString("H3_FALSY"), false);
    Native::registerConstant<KindOfInt64>(s_H3_INTY.get(), 123);
    Native::registerConstant<KindOfString>(s_H3_GREETING.get(), s_Hello_World.get());

    HHVM_FE(h3_add);
    HHVM_FE(h3_enum_greetings);

    loadSystemlib();
  }

} s_h3_extension;

HHVM_GET_MODULE(h3);
