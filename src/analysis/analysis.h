// #include "extractFrames.h"
#include "./localMotionEstimation.h"
#include "./movementAggregation.h"
#include "./movementAnalysis.h"

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
  globalDeltaData gdd;

public:
  analysis() = default;
  analysis(framevector _frames) : frames(_frames) {
    localMotionEstimation lme(frames);
    auto localmotion = lme.estimateBlockWise(boxSize);
    movementAggregation mAggro(localmotion);
    movementAnalysis mAnal(frames, mAggro.getDelta());
    gdd = mAnal.calcGlobalMotion();
  };

  decltype(auto) analyze(framevector const &_f) {
    localMotionEstimation lme(_f);
    auto localmotion = lme.estimateBlockWise(boxSize);
    movementAggregation mAggro(localmotion);
    movementAnalysis mAnal(_f, mAggro.getDelta());
    return mAnal.calcGlobalMotion();
  }

private:
};
} // namespace analysis
} // namespace cimarron
