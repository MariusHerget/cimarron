
using namespace vpp;
using namespace s;

#include "../helper/filesystem.h"
#include "../helper/typeIdentifier.h"
#include "../helper/types.h"

namespace cimarron {
namespace post {
class reframing {
private:
  framevector frames;
  globalDeltaData gdd;

public:
public:
  reframing() = default;
  reframing(framevector _frames, globalDeltaData _gdd)
      : frames(_frames), gdd(_gdd) {}

  framevector reframe() {
    auto resize = calcMaxTransformation();
    framevector tframes;
    std::cout << "applyTransformation(): " << (int)std::abs(resize.x) << "/"
              << (int)std::abs(resize.y) << std::endl;
    for (auto gdi : gdd) {
      auto fr = clone(frames[gdi.frameindex], _border = 0);
      // auto frame_graylevel = rgb_to_graylevel<unsigned char>(fr);
      auto image = to_opencv(fr);
      cv::Mat resized;
      cv::copyMakeBorder(image, resized, (int)std::abs(resize.y),
                         (int)std::abs(resize.y), (int)std::abs(resize.x),
                         (int)std::abs(resize.x), cv::BORDER_CONSTANT,
                         cv::Scalar(0, 0, 0));
      performPositionShift(resized, gdi.deltaVector);
      // gdi.deltaVector.deltaAngle = 45;
      performRotation(resized, gdi.deltaVector);
      auto fret = vpp::from_opencv<unsigned char>(resized);
      cv::imshow("transformFrames", resized);
      cv::waitKey(1);
      tframes.push_back(fr);
    }
    return tframes;
  }

private:
  deltaVector calcMaxTransformation() {
    float xmax = 0., ymax = 0.;
    for (auto gdi : gdd) {
      xmax = std::max(xmax, std::abs(gdi.deltaVector.deltaPosition.x));
      ymax = std::max(ymax, std::abs(gdi.deltaVector.deltaPosition.y));
    }
    return deltaVector(xmax, ymax);
  }
};
} // namespace post
} // namespace cimarron
