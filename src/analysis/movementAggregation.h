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
  frameDeltaData aggregatedLocalMF;

public:
  movementAggregation(motionData _lme) : lme(_lme) {
    aggregatedLocalMF = compareLocalMD();
    std::cout << "movementAggregation finished" << std::endl;
  };

  decltype(auto) getDelta() { return aggregatedLocalMF; }

private:
  frameDeltaData compareLocalMD() {
    frameDeltaData fdd;
    for (int i = 1; i < lme.size(); i++) {
      frameDeltaImage fdi = compareFramesMD(lme[i - 1], lme[i]);
      fdd.push_back(fdi);
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
            frameDeltaVector fdvl = compareTV(tvnow, tvnext);
            fdv.emplace_back(fdvl);
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
                              diffArea, tvnow.index);
    } else {
      return frameDeltaVector();
    }
  };
};
} // namespace analysis
} // namespace cimarron
