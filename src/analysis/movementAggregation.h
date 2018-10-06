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
  motionData lme; // localMotionEstimation

  framevector &frames;

public:
  movementAggregation(framevector &_frames, motionData _lme)
      : frames(_frames), lme(_lme) {
    auto aggregatedLocalMF = compareLocalMD();
    std::cout << "movementAggregation finished" << std::endl;
  };

private:
  frameDeltaData compareLocalMD() {
    frameDeltaData fdd;
    for (int i = 1; i < lme.size(); i++) {
      fdd.push_back(compareFramesMD(lme[i - 1], lme[i]));
    }
    return fdd;
  };
  frameDeltaImage compareFramesMD(motionVector fnow, motionVector fnext) {
    std::vector<frameDeltaVector> fdv;
    // Maybe sorting beforehand? Better performance
    if (fnow.frameindex == fnext.frameindex - 1) {
      for (auto tvnow : fnow.trackingVectors) {
        for (auto tvnext : fnext.trackingVectors) {
          if (tvnow.index == tvnext.index) {
            fdv.push_back(compareTV(tvnow, tvnext));
            continue;
          }
        }
      }
      return frameDeltaImage(fdv, fnow.frameindex, fnext.frameindex);
    } else {
      return frameDeltaImage();
    }
  };
  frameDeltaVector compareTV(TV tvnow, TV tvnext) {
    if (tvnow.index == tvnext.index) {
      float diffCenterX = (float)tvnext.trackingVector.center.x -
                          (float)tvnow.trackingVector.center.x;
      float diffCenterY = (float)tvnext.trackingVector.center.y -
                          (float)tvnow.trackingVector.center.y;
      float diffAngle = ((float)tvnext.trackingVector.angle -
                         (float)tvnow.trackingVector.angle);
      // - -> rotating left // + -> rotating right
      float diffArea = ((float)tvnext.trackingVector.size.width *
                        (float)tvnext.trackingVector.size.height) -
                       ((float)tvnow.trackingVector.size.width *
                        (float)tvnow.trackingVector.size.height);
      return frameDeltaVector(deltaVector(diffCenterX, diffCenterY), diffAngle,
                              diffArea);
    } else {
      return frameDeltaVector();
    }
  };
};
} // namespace analysis
} // namespace cimarron
