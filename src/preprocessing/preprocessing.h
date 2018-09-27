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
  preprocessing(clc_str _videocstr) : videocstr(_videocstr) {
    extractFrames ef(videocstr);
    frames = ef.getFrames();
    std::cout << "Framevector: " << countFrames() << std::endl;
    // ef.exportToPPM("./tmp/");
  };

  decltype(auto) getFrameVector() { return frames; }

private:
  int countFrames(int totalFrame = 0) { return totalFrame + frames.size(); };
};
} // namespace pre
} // namespace cimarron
