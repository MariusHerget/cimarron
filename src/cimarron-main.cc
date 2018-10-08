#include <opencv2/highgui.hpp>

#include <iod/parse_command_line.hh>
#include <iod/timer.hh>

#include <vpp/algorithms/symbols.hh>

#include <vpp/algorithms/video_extruder.hh>
#include <vpp/utils/opencv_bridge.hh>
#include <vpp/utils/opencv_utils.hh>
#include <vpp/vpp.hh>

#include "analysis/analysis.h"
#include "helper/types.h"
#include "postprocessing/reframing.h"
#include "postprocessing/video_output.h"
#include "preprocessing/preprocessing.h"
#include "stabilziation/stabilziation.h"
#include "test/symbols.hh"

using namespace iod;
using namespace vpp;
using namespace s;

int main(int argc, const char *argv[]) {
  std::string _outputLocation;
  std::cout << "name of program: " << argv[0] << '\n';
  std::cout << "Input: " << argv[1] << '\n';
  std::cout << "Output: " << argv[2] << '\n';
  // auto opts = parse_command_line(
  //     argc, argv,
  //     cl::positionals(_video, _outputLocation, _detector_th,
  //     _keypoint_spacing), cl::required(_video),
  //     cl::required(_outputLocation), _video = std::string(), _outputLocation
  //     = std::string(), _detector_th = int(10), _keypoint_spacing = int(5),
  //     _record_video = std::string());

  auto opts = parse_command_line(
      argc, argv,
      cl::positionals(_video, _record_video, _detector_th, _keypoint_spacing),
      cl::required(_video, _record_video), _video = std::string(),
      _detector_th = int(10), _keypoint_spacing = int(5),
      _record_video = std::string());

  // std::cout << "Input: " << _video << "\nOutput: " << _outputLocation;
  cimarron::pre::preprocessing pre(opts.video.c_str());
  auto frames = pre.getFrameVector();
  cimarron::post::reframing ref();
  cimarron::analysis::analysis ana(frames);
  cimarron::stabilziation::stabilziation stab(frames, ana.gdd);
  auto finFrames = stab.getTransformedFramevector();
  // DEBUG POINT
  cimarron::post::video_output vo;
  // Currently this:
  vo.safe(finFrames, argv[2]);
  // how can I do this:
  // vo << ref << frames;
}
