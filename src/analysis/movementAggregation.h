// #include "extractFrames.h"

using namespace iod;
using namespace vpp;
using namespace s;

#include "../helper/printer.h"
#include "../helper/types.h"

namespace cimarron {
namespace analysis {
class movementAggregation {
private:
  correctionData _lme; // localMotionEstimation

  framevector &frames;

public:
  movementAggregation(framevector &_frames, correctionData _lme)
      : frames(_frames), lme(_lme){};

private:
  compareLocalCD(){};
};
} // namespace analysis
} // namespace cimarron
