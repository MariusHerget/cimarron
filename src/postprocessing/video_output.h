#ifndef VO_H
#define VO_H
using namespace iod;
using namespace vpp;
using namespace s;

#include "../helper/filesystem.h"
#include "../helper/typeIdentifier.h"
#include "../helper/types.h"

#include <stdexcept>
#include <string>

namespace cimarron {
namespace post {
class video_output {
private:
  framevector videocstr;
  framevector lazy;
  cv::VideoWriter output_video;
  std::string filename;

public:
  video_output() = default;
  video_output(framevector _videocstr) : videocstr(_videocstr){};
  video_output(std::string _file) : filename(_file){};

  // video_output &operator<<(String file) { safeToVideoFile(videocstr, file); }

  video_output &operator<<(framevector _videocstr) {
    safeToVideoFile(_videocstr, filename);
    return *this;
  }

  // video_output &operator<<(cimarron::post::reframing _ref) {
  //   safeToVideoFile(_ref.getFrameVector(), filename);
  //   return *this;
  // }
  // video_output &operator<<(framevector _videocstr, String file) {
  //   safeToVideoFile(_videocstr, file);
  //   return *this;
  // }
  void safeImageToFrameVector(cv::Mat _frame) {
    safeImageToFrameVector(from_opencv<vuchar3>(_frame));
  }
  void safeImageToFrameVector(frame _frame) { lazy.push_back(_frame); }
  void safeLazy(std::string file) {
    if (lazy.size() > 0)
      safeToVideoFile(lazy, file);
  }

private:
  void safeToVideoFile(framevector _frames, std::string file) {
    // TODO SAFE FRAMES TO VIDEO
    // Videooutput does not work yet
    cv::VideoWriter outputVideo;
    // cv::Size S = cv::Size((int)_frames[0].domain().nrows(),
    //                       (int)_frames[0].domain().ncols());
    // Weird bug - changed order of cols and rows
    std::cout << "SIZE: " << (int)_frames[0].domain().nrows() << " / "
              << (int)_frames[0].domain().ncols() << std::endl;
    outputVideo.open(
        file, cv::VideoWriter::fourcc('I', '4', '2', '0'), 24.f,
        cv::Size(_frames[0].domain().ncols(), _frames[0].domain().nrows()),
        true);
    if (!outputVideo.isOpened()) {
      std::cout << "Could not open the output video for write: " << file
                << std::endl;
      throw std::invalid_argument("Videooutput error!");
    }
    outputVideo << to_opencv(_frames[0]);
    std::cout << "Framevector safe: " << _frames.size() << std::endl;
    for (frame f : _frames) {
      auto fr = clone(f, _border = 0);
      fill_border_mirror(fr);
      cv::imshow("safeToVideoFile", to_opencv(fr));
      cv::waitKey(1);
      if (outputVideo.isOpened())
        outputVideo << to_opencv(fr);
      else
        throw std::invalid_argument("Videooutput error!");
    }
    outputVideo.release();
  }
};
} // namespace post
} // namespace cimarron
#endif
