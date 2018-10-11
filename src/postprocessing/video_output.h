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
  framevector frames;
  cv::VideoWriter output_video;
  std::string filename;

public:
  video_output() = default;
  video_output(framevector const &_f, std::string _file)
      : frames(_f), filename(_file) {
    std::cout << "Output file: " << _file << "\n Dimensions"
              << (int)frames[0].domain().nrows() << " / "
              << (int)frames[0].domain().ncols() << std::endl;
  };
  video_output(std::string _file) : filename(_file){};
  //
  // void safeImageToFrameVector(cv::Mat _frame) {
  //   safeImageToFrameVector(from_opencv<vuchar3>(_frame));
  // }
  // void safeImageToFrameVector(frame _frame) { lazy.push_back(_frame); }
  // void safeLazy(std::string file) {
  //   if (lazy.size() > 0)
  //     safeToVideoFile(lazy, file);
  // }

  void set(framevector const &_f, std::string _file) {
    frames = _f;
    filename = _file;
  }
  void set(framevector const &_f) { frames = _f; }
  void flush() { safeToVideoFile(frames, filename); }

private:
  void safeToVideoFile(framevector _frames, std::string file) {
    cv::VideoWriter outputVideo;
    outputVideo.open(
        file, cv::VideoWriter::fourcc('I', '4', '2', '0'), 24.f,
        cv::Size(_frames[0].domain().ncols(), _frames[0].domain().nrows()),
        true);
    if (!outputVideo.isOpened()) {
      std::cout << "Could not open the output video for write: " << file
                << std::endl;
      throw std::invalid_argument("Videooutput error!");
    }
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
