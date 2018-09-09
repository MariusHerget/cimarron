using namespace iod;
using namespace vpp;
using namespace s;

#include "../helper/filesystem.h"
#include "../helper/typeIdentifier.h"
#include "../helper/types.h"

namespace cimarron {
namespace pre {
class extractFrames {
private:
  clc_str videocstr;

public:
  extractFrames(clc_str _videocstr) : videocstr(_videocstr){};

  decltype(auto) getFrames() {
    framevector frames;
    foreach_videoframe(videocstr) |
        [&](const image2d<vuchar3> &frame_cv) { frames.push_back(frame_cv); };
    return frames;
  }

  void exportToPPM(std::string baseDir) {
    cimarron::helper::createDir(baseDir);
    cimarron::helper::createDir(baseDir + "raw/");
    cimarron::helper::createDir(baseDir + "graylevel/");
    int nframes = 0;
    int us_cpt = 0;
    foreach_videoframe(videocstr) | [&](const image2d<vuchar3> &frame_cv) {
      auto frame = clone(frame_cv, _border = 3);
      // std::cout << "frame_cv is " << type_name<decltype(frame_cv)>() << '\n';
      // std::cout << "frame is " << type_name<decltype(frame)>() << '\n';
      fill_border_mirror(frame);
      auto frame_graylevel = rgb_to_graylevel<unsigned char>(frame);
      timer t;
      t.start();
      // Export Frame to tmp raw
      std::string name(baseDir);
      name.append("raw/");
      name.append(std::to_string(nframes));
      name.append(".ppm");
      cv::imwrite(name, to_opencv(frame));

      // Export Frame to tmp gray
      std::string namegray(baseDir);
      namegray.append("graylevel/");
      namegray.append(std::to_string(nframes));
      namegray.append(".jpg");
      cv::imwrite(namegray, to_opencv(frame_graylevel));
      t.end();

      us_cpt += t.us();
      if (!(nframes % 5)) {
        std::cout
            << "(Frame: " << nframes //<< " / " //<< totalFrame
            << ") Tracker time: " << (us_cpt / 1000000.f)
            << " ms/frame. " // << ctx.trajectories.size() << " particles."
            << std::endl;
        us_cpt = 0;
      }
      nframes++;
    };
  };
};

} // namespace pre
} // namespace cimarron
