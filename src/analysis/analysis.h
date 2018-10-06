// #include "extractFrames.h"
#include "./localMotionEstimation.h"
#include "./movementAggregation.h"

using namespace iod;
using namespace vpp;
using namespace s;

#include "../helper/printer.h"
#include "../helper/types.h"

namespace cimarron {
namespace analysis {
class analysis {
private:
  framevector frames;
  int boxSize = 10;

public:
  analysis(framevector _frames) : frames(_frames) {
    // cimarron::print::Framevector(frames);
    localMotionEstimation lme(frames);
    auto localmotion = lme.estimateBlockWise(boxSize);
    movementAggregation mA(frames, localmotion);
    // cimarron::print::CorrectionData(localcorrection);
  };

private:
};
} // namespace analysis
} // namespace cimarron
