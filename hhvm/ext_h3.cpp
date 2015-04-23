#include "hphp/runtime/ext/extension.h"

using namespace HPHP;

class H3Extension : public Extension {
 public:
  H3Extension(): Extension("h3", "0.0.1") {}

  void moduleInit() override {
    loadSystemlib();
  }

} s_h3_extension;

HHVM_GET_MODULE(h3);
