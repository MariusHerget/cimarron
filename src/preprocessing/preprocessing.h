#include "extractFrames.h"

using namespace iod;
using namespace vpp;
using namespace s;

#include "../helper/types.h"

namespace cimarron {
namespace pre {
class preprocessing {
private:
  clc_str videocstr;
  framevector frames;

public:
  preprocessing() = default;
  preprocessing(clc_str _videocstr) : videocstr(_videocstr) {
    extractFrames ef(videocstr);
    frames = ef.getFrames(frames);
    std::cout << "Framevector: " << countFrames() << std::endl;
    // ef.exportToPPM("./tmp/");
  };

  decltype(auto) getFrameVector() { return frames; }

  decltype(auto) prepare(clc_str const &path) {
    extractFrames ef(path);
    return ef.getFrames(frames);
  }
  // friend framevector &operator>>(framevector &streamframes, const clc_str
  // &s); framevector &operator>>(const clc_str &videocstr, framevector
  // &streamframes) {
  //   std::cout << "Works!";
  // }
  //
  // framevector &operator<<(const clc_str &videocstr, framevector
  // &streamframes) {
  //   std::cout << "Works!";
  // }

private:
  int countFrames(int totalFrame = 0) { return totalFrame + frames.size(); };
};
} // namespace pre
} // namespace cimarron

// framevector &operator>>(framevector &streamframes, const clc_str &s) {
//   std::cout << "Works";
//   return streamframes;
// }
//
// framevector &cimarron::pre::preprocessing::operator<<(const clc_str &s) {
//   extractFrames ef(s);
//   return ef.getFrames(streamframes);
// }
