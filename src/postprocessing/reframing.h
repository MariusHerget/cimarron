
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
  std::vector<int> cols, rows;

public:
public:
  reframing() = default;
  reframing(framevector _frames, globalDeltaData _gdd)
      : frames(_frames), gdd(_gdd) {
    cols = std::vector<int>(_frames[0].domain().ncols(), 0);
    rows = std::vector<int>(_frames[0].domain().nrows(), 0);
  }

  framevector reframe() {
    auto resize = calcMaxTransformation();
    auto dimensions = findDimensions();
    framevector tframes;
    std::cout << "applyTransformation(): " << (int)std::abs(resize.x) << "/"
              << (int)std::abs(resize.y) << std::endl;
    for (auto gdi : gdd) {
      auto fr = clone(frames[gdi.frameindex], _border = 0);
      // auto frame_graylevel = rgb_to_graylevel<unsigned char>(fr);
      // auto image = to_opencv(fr);
      // cv::Mat resized;
      // cv::copyMakeBorder(image, resized, -(int)std::abs(resize.y),
      //                    -(int)std::abs(resize.y), -(int)std::abs(resize.x),
      //                    -(int)std::abs(resize.x), cv::BORDER_CONSTANT,
      //                    cv::Scalar(0, 0, 0));
      // auto fret = vpp::from_opencv<unsigned char>(resized);
      // cv::imshow("reframed frames", resized);
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
  deltaVector findDimensions() {
    for (auto f : frames)
      findDimension(to_opencv(clone(f, _border = 0)));
  }
  void findDimension(cv::Mat image) {
    for (int r = 0; r < rows.size(); r++) {
      // if (rows[r] == 0)
      for (int c = 0; c < cols.size(); c++) {
        // if (cols[c] == 0) {
        auto pixel = image.at<cv::Vec3b>(cv::Point(r, c));
        // std::cout << "Pixel at (" << r << "/" << c << "): " << pixel[0] << ",
        // "
        //           << pixel[1] << ", " << pixel[2] << std::endl;
        if (pixel[0] == -1 || pixel[1] == -1 || pixel[2] == -1) {
          cols[c]++;
          rows[r]++;
        }
      }
    }
  }
}; // namespace post
} // namespace post
} // namespace cimarron
