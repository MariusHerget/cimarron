// #include "extractFrames.h"
#include "./transformFrames.h"

using namespace iod;
using namespace vpp;
using namespace s;

#include "../helper/printer.h"
#include "../helper/types.h"

namespace cimarron {
namespace stabilziation {
class stabilziation {
private:
  framevector frames;
  globalDeltaData gdd;
  cimarron::stabilziation::transformFrames transFrames;
  std::vector<char> performStabi;

public:
  stabilziation() = default;
  stabilziation(std::vector<char> ps) : performStabi(ps){};
  stabilziation(framevector _frames, globalDeltaData _gdd)
      : frames(_frames), gdd(_gdd) {
    transFrames =
        cimarron::stabilziation::transformFrames(frames, gdd, performStabi);
  };

  framevector getTransformedFramevector() {
    return transFrames.applyTransformation();
  }

  decltype(auto) stabilze(framevector _frames, globalDeltaData _gdd) {
    transFrames =
        cimarron::stabilziation::transformFrames(_frames, _gdd, performStabi);
    return transFrames.applyTransformation();
  }

private:
};
} // namespace stabilziation
} // namespace cimarron
