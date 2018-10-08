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
                         cv::Scalar(0, 0, 0));
      performPositionShift(resized, gdi.deltaVector);
      performPositionShift(resized, gdi.deltaVector);
      auto fret = vpp::from_opencv<unsigned char>(resized);
      cv::imshow("transformFrames", resized);
      cv::waitKey(1);
      tframes.push_back(fr);
    }
    return tframes;
  };

private:
  deltaVector calcMaxTransformation() {
    float xmax = 0., ymax = 0.;
    for (auto gdi : gdd) {
      xmax = std::max(xmax, std::abs(gdi.deltaVector.deltaPosition.x));
      ymax = std::max(ymax, std::abs(gdi.deltaVector.deltaPosition.y));
    }
    return deltaVector(xmax, ymax);
  }
  cv::Mat performPositionShift(cv::Mat &image, frameDeltaVector dv) {
    cv::Mat trans_mat = (cv::Mat_<double>(2, 3) << 1, 0, dv.deltaPosition.x, 0,
                         1, dv.deltaPosition.y);
    cv::warpAffine(image, image, trans_mat, image.size());
    return trans_mat;
  }
  cv::Mat perfornRotation(cv::Mat &image, frameDeltaVector dv) {
    // get rotation matrix for rotating the image around its center in pixel
    // coordinates
    cv::Point2f center((image.cols - 1) / 2.0, (image.rows - 1) / 2.0);
    cv::Mat rot = cv::getRotationMatrix2D(center, dv.deltaAngle, 1.0);
    // determine bounding rectangle, center not relevant
    cv::Rect2f bbox =
        cv::RotatedRect(cv::Point2f(), image.size(), dv.deltaAngle)
            .boundingRect2f();
    // adjust transformation matrix
    rot.at<double>(0, 2) += bbox.width / 2.0 - image.cols / 2.0;
    rot.at<double>(1, 2) += bbox.height / 2.0 - image.rows / 2.0;

    cv::warpAffine(image, image, rot, bbox.size());
  }
};
} // namespace stabilziation
} // namespace cimarron
