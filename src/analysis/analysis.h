// #include "extractFrames.h"
#include "localMotionEstimation.h"

using namespace iod;
using namespace vpp;
using namespace s;

#include "../helper/types.h"

namespace cimarron {
namespace analysis {
class analysis {
private:
  framevector frames;
  int boxSize = 10;

public:
  analysis(framevector _frames)
      : frames(_frames){
            // localMotionEstimation lme(frames);
            // auto localcorrection = lme.estimateBlockWise(boxSize);
        };

private:
};
} // namespace analysis
} // namespace cimarron
