#include <opencv2/highgui.hpp>

#include <iod/parse_command_line.hh>
#include <iod/timer.hh>

#include <vpp/algorithms/symbols.hh>

#include <vpp/algorithms/video_extruder.hh>
#include <vpp/utils/opencv_bridge.hh>
#include <vpp/utils/opencv_utils.hh>
#include <vpp/vpp.hh>

#include "helper/types.h"
#include "preprocessing/preprocessing.h"
#include "test/symbols.hh"

using namespace iod;
using namespace vpp;
using namespace s;

int main(int argc, const char *argv[]) {
  auto opts = parse_command_line(
      argc, argv, cl::positionals(_video, _detector_th, _keypoint_spacing),
      cl::required(_video), _video = std::string(), _detector_th = int(10),
      _keypoint_spacing = int(5), _record_video = std::string());

  bool first = true;

  cimarron::pre::preprocessing pre(opts.video.c_str());
}
