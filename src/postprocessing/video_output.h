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
class video_output : public std::istream {
private:
  framevector videocstr;
  cv::VideoWriter output_video;

public:
  video_output() = default;
  video_output(framevector _videocstr) : videocstr(_videocstr){};

  // video_output &operator<<(String file) { safeToVideoFile(videocstr, file); }

  video_output &operator<<(framevector _videocstr) {
    safeToVideoFile(_videocstr, "./tmp/output.avi");
    return *this;
  }

  // video_output &operator<<(framevector _videocstr, String file) {
  //   safeToVideoFile(_videocstr, file);
  //   return *this;
  // }

private:
  void safeToVideoFile(framevector _frames, std::string file) {
    // TODO SAFE FRAMES TO VIDEO
    cv::VideoWriter outputVideo;
    cv::Size S = cv::Size((int)_frames[0].domain_with_border().ncols(),
                          (int)_frames[0].domain_with_border().nrows());
    std::cout << "SIZE: " << (int)_frames[0].domain_with_border().ncols()
              << " / " << (int)_frames[0].domain_with_border().nrows()
              << std::endl;
    outputVideo.open(file, cv::VideoWriter::fourcc('P', 'I', 'M', '1'), 24.f, S,
                     true);
    if (!outputVideo.isOpened()) {
      std::cout << "Could not open the output video for write: " << file
                << std::endl;
      throw std::invalid_argument("Videooutput error!");
    }

    std::cout << "Framevector safe: " << _frames.size() << std::endl;
    for (frame f : _frames) {
      outputVideo << to_opencv(f);
    }
    outputVideo.release();
  }
};
} // namespace post
} // namespace cimarron
