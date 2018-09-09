#include "extractFrames.h"

using namespace iod;
using namespace vpp;
using namespace s;

#include "../helper/types.h"

namespace cimarron {
class preprocessing {
private:
  clc_str videocstr;

public:
  preprocessing(clc_str _videocstr) : videocstr(_videocstr) {
    extractFrames ef(videocstr);
    int totalFrame = countFrames();
    // std::cout << totalFrame << " == " << opts.record_video.size() <<
    // std::endl;
    ef.exportToPPM("./tmp/");
  };

private:
  int countFrames(int totalFrame = 0) {
    foreach_videoframe(videocstr) |
        [&](const image2d<vuchar3> &frame_cv) { totalFrame++; };
    return totalFrame;
  };
};
} // namespace cimarron
