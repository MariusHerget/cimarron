
using namespace vpp;
using namespace s;

#include "../helper/filesystem.h"
#include "../helper/typeIdentifier.h"
#include "../helper/types.h"

namespace cimarron {
namespace post {
class reframing {
  framevector videocstr;

public:
  reframing() = default;

  decltype(auto) operator<<(framevector _videocstr) {
    videocstr = _videocstr;
    return _videocstr;
  }

  decltype(auto) getFrameVector() { return videocstr; }

private:
};
} // namespace post
} // namespace cimarron
