// #include "extractFrames.h"
using namespace iod;
using namespace vpp;
using namespace s;

#include "../helper/printer.h"
#include "../helper/types.h"

#include <algorithm> // std::max
#include <cmath>     // std::abs

namespace cimarron {
namespace stabilziation {
class transformFrames {
private:
  framevector frames;
  globalDeltaData gdd;

public:
public:
  transformFrames() = default;
  transformFrames(framevector _frames, globalDeltaData _gdd)
      : frames(_frames), gdd(_gdd) {}

  framevector applyTransformation() {
    auto resize = calcMaxTransformation();
    return applyTransformation(resize);
  }

  framevector applyTransformation(deltaVector resize) {
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
                         cv::Scalar(-1, -1, -1));
      performPositionShift(resized, gdi.deltaVector);
      // gdi.deltaVector.deltaAngle = 45;
      performRotation(resized, gdi.deltaVector);
      auto fret = vpp::from_opencv<vuchar3>(resized);
      cv::imshow("transformFrames", to_opencv(fret));
      cv::waitKey(1);
      tframes.push_back(fret);
    }
    std::cout << "applyTransformation vo: " << (int)tframes[0].domain().ncols()
              << std::endl;
    return tframes;
  };

private:
  deltaVector calcMaxTransformation() {
    // Calculate the maximum change in position for resizing
    float xmax = 0., ymax = 0.;
    for (auto gdi : gdd) {
      xmax = std::max(xmax, std::abs(gdi.deltaVector.deltaPosition.x));
      ymax = std::max(ymax, std::abs(gdi.deltaVector.deltaPosition.y));
    }
    return deltaVector(xmax, ymax);
  }
  void performPositionShift(cv::Mat &image, frameDeltaVector dv) {
    // Perform position shift
    cv::Mat trans_mat = (cv::Mat_<double>(2, 3) << 1, 0, dv.deltaPosition.x, 0,
                         1, dv.deltaPosition.y);
    cv::warpAffine(image, image, trans_mat, image.size());
  }
  void performRotation(cv::Mat &image, frameDeltaVector dv) {
    // Perform rotation
    cv::Point2f center((image.cols - 1) / 2.0, (image.rows - 1) / 2.0);
    cv::Mat rot = cv::getRotationMatrix2D(center, dv.deltaAngle, 1.0);

    cv::warpAffine(image, image, rot, image.size());
  }
};
} // namespace stabilziation
} // namespace cimarron
