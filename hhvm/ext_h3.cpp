#include "hphp/runtime/ext/extension.h"

using namespace HPHP;

static int64_t HHVM_FUNCTION(h3_add, int64_t a, int64_t b) {
  return a + b;
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

    loadSystemlib();
  }

} s_h3_extension;

HHVM_GET_MODULE(h3);
