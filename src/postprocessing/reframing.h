
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
  reframing(framevector &_frames, globalDeltaData &_gdd)
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

  decltype(auto) mask(framevector const &_f, globalDeltaData const &_gdd) {
    frames = _f;
    gdd = _gdd;
    auto resize = calcMaxTransformation();
    auto ffirst = clone(_f[0], _border = 0);
    cv::Mat mask =
        cv::Mat::zeros(to_opencv(ffirst).size(), to_opencv(ffirst).type());
    for (int c = 0; c < mask.cols; c++)
      for (int r = 0; r < mask.rows; r++)
        if ((r > resize.y * 2 && r < mask.rows - resize.y * 2) &&
            (c > resize.x * 2 && c < mask.cols - resize.x * 2))
          mask.at<cv::Vec3b>(cv::Point(c, r)) = cv::Vec3b{255, 255, 255};

    framevector reframed;
    for (auto frame : _f) {
      auto freframe = clone(frame, _border = 0);
      auto freframec = to_opencv(freframe);
      cv::Mat out;
      freframec.copyTo(out, mask);
      reframed.push_back(vpp::from_opencv<vuchar3>(out));
    }
    return reframed;
    // mask [resize.x:_f[0].domain().ncols() + resize.x,
    // resize.y:_f[0].domain().nrows() + resize.y] = 255; auto masked_img =
    // cv::bitwise_and(f[0], f[0], mask = mask)
  }
  // cv::Size(_f[0].domain().ncols() - resize.x * 2,
  //                         _f[0].domain().nrows() - resize.y * 2)
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
