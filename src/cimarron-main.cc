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
// #include <type_traits>
// #include <typeinfo>
// #ifndef _MSC_VER
// #include <cxxabi.h>
// #endif
// #include <cstdlib>
// #include <memory>
// #include <string>
//
// template <class T> std::string type_name() {
//   typedef typename std::remove_reference<T>::type TR;
//   std::unique_ptr<char, void (*)(void *)> own(
// #ifndef _MSC_VER
//       abi::__cxa_demangle(typeid(TR).name(), nullptr, nullptr, nullptr),
// #else
//       nullptr,
// #endif
//       std::free);
//   std::string r = own != nullptr ? own.get() : typeid(TR).name();
//   if (std::is_const<TR>::value)
//     r += " const";
//   if (std::is_volatile<TR>::value)
//     r += " volatile";
//   if (std::is_lvalue_reference<T>::value)
//     r += "&";
//   else if (std::is_rvalue_reference<T>::value)
//     r += "&&";
//   return r;
// }
// using namespace cimarron;

int main(int argc, const char *argv[]) {
  auto opts = parse_command_line(
      argc, argv, cl::positionals(_video, _detector_th, _keypoint_spacing),
      cl::required(_video), _video = std::string(), _detector_th = int(10),
      _keypoint_spacing = int(5), _record_video = std::string());

  bool first = true;

  cimarron::preprocessing pre(opts.video.c_str());
}
