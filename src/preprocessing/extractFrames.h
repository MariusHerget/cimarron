using namespace iod;
using namespace vpp;
using namespace s;

#include "../helper/types.h"

namespace cimarron {
class extractFrames {
private:
  clc_str videocstr;

public:
  extractFrames(clc_str _videocstr) : videocstr(_videocstr){};

  void exportToPPM(std::string baseDir) {
    int nframes = 0;
    int us_cpt = 0;
    foreach_videoframe(videocstr) | [&](const image2d<vuchar3> &frame_cv) {
      auto frame = clone(frame_cv, _border = 3);
      fill_border_mirror(frame);
      auto frame_graylevel = rgb_to_graylevel<unsigned char>(frame);
      timer t;
      t.start();
      // Export Frame to tmp
      std::string name(baseDir);
      name.append(std::to_string(nframes));
      name.append(".ppm");
      cv::imwrite(name, to_opencv(frame));
      t.end();

      us_cpt += t.us();
      if (!(nframes % 5)) {
        std::cout
            << "(Frame: " << nframes << " / " //<< totalFrame
            << ") Tracker time: " << (us_cpt / 1000000.f)
            << " ms/frame. " // << ctx.trajectories.size() << " particles."
            << std::endl;
        us_cpt = 0;
      }
      nframes++;
    };
  };
};
} // namespace cimarron
