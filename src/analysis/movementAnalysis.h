// #include "extractFrames.h"

using namespace iod;
using namespace vpp;
using namespace s;
#include <algorithm> // std::max
#include <cmath>     // std::abs

#include "../helper/ownMath.h"
#include "../helper/printer.h"
#include "../helper/types.h"

namespace cimarron {
namespace analysis {
class movementAnalysis {
private:
  frameDeltaData fdd;
  const framevector &frames;

  deltaVector threshHoldCameraErrors;
  float threshHoldSimiliartyTV = 0.7;
  float threshHoldSimilar = 0.75;

public:
  movementAnalysis(framevector const &_frames, frameDeltaData _fdd)
      : frames(_frames), fdd(_fdd) {
    threshHoldCameraErrors = deltaVector(frames[0].domain().ncols() * 0.10,
                                         frames[0].domain().nrows() * 0.10);
    filterTrackingErrors();
  };
  void filterTrackingErrors() {
    std::cout << "threshHoldCameraErrors (" << threshHoldCameraErrors.x << "/"
              << threshHoldCameraErrors.y << ")" << std::endl;
    for (auto fd : fdd) {
      for (int i = 0; i < fd.deltaVectors.size(); i++) {
        if (std::abs(fd.deltaVectors[i].deltaPosition.x) >
                threshHoldCameraErrors.x ||
            std::abs(fd.deltaVectors[i].deltaPosition.y) >
                threshHoldCameraErrors.y) {
          fd.deltaVectors.erase(fd.deltaVectors.begin() + i);
          std::cout << "!!!!! Deleted TV(" << fd.deltaVectors[i].TVindex
                    << ") from frame " << fd.frameindex << std::endl;
        }
      }
    }
  }
  globalDeltaData calcGlobalMotion() {
    globalDeltaData gdd;
    std::cout << "Calculation GlobalMotionVectors" << std::endl;
    for (auto fd : fdd) {
      globalDeltaImage gdi(fd.frameindex, fd.framenext);
      // Sonderfall nur ein TV
      if (fd.deltaVectors.size() == 0) {
        gdi =
            globalDeltaImage(frameDeltaVector(deltaVector(0., 0.), 0., 0., -1),
                             fd.frameindex, fd.framenext);

      } else if (fd.deltaVectors.size() == 1) {
        // Gute Lösung? TODO
        // return the single DeltaVector
        gdi = globalDeltaImage(fd.deltaVectors[0], fd.frameindex, fd.framenext);
        gdi.deltaVector.TVindex = -1;
      } else {
        // Calc simularity and decide whether theres is camera shake
        std::vector<std::vector<float>> vectorsims;
        std::vector<std::vector<float>> anglesims;
        for (int i = 0; i < fd.deltaVectors.size(); i++) {
          for (int o = i + 1; o < fd.deltaVectors.size(); o++) {
            vectorsims.push_back(
                calcVectorsSimilarty(&fd.deltaVectors[i], &fd.deltaVectors[o]));
            anglesims.push_back(
                calcAngleSimilarty(&fd.deltaVectors[i], &fd.deltaVectors[o]));
          }
        }

        // Check whether sim and offset holds the global threshold
        //
        int similarVectors = 0;
        float simVectorsx = 0.0;
        float simVectorsy = 0.0;
        for (auto sim : vectorsims) {
          // Sim is higher than 0.7
          if (sim[0] > threshHoldSimiliartyTV) {
            similarVectors++;
            simVectorsx += sim[1];
            simVectorsy += sim[2];
          }
        }
        int similarAngles = 0;
        float similarAnglesSum = 0;
        for (auto ang : anglesims) {
          // Angle difference is higher than 10%
          if (ang[0] > 0.1) {
            similarAngles++;
            similarAnglesSum += ang[1];
          }
        }

        // Calculate Global motion
        // Vectors:
        float gdiangle;
        deltaVector gdidv;
        // If there are enough vectors with high enough similarity calculate the
        // mean
        if (similarVectors > (threshHoldSimilar * vectorsims.size())) {
          // Enough similar
          gdidv = deltaVector(simVectorsx / similarVectors,
                              simVectorsy / similarVectors);
        } else {
          gdidv = deltaVector(0., 0.);
        }
        // Angles
        if (similarAngles > (threshHoldSimilar * anglesims.size())) {
          // Enough similar
          gdiangle = similarAnglesSum / similarAngles;
        } else {
          gdiangle = 0.;
        }
        gdi.deltaVector = frameDeltaVector(gdidv, gdiangle, 0., -1);
      }
      gdd.push_back(gdi);
    }
    return gdd;
  }

private:
  std::vector<float> calcVectorsSimilarty(frameDeltaVector *DVfirst,
                                          frameDeltaVector *DVsecond) {
    // Use only cosine_similarity
    // Maybe euclidean_distance but how we rate it?
    auto cossim = cosine_similarity(DVfirst, DVsecond);
    auto eucdist = euclidean_distance(DVfirst, DVsecond);
    // return (0.5 * cossim + 0.5 * eucdist);
    return std::vector<float>{cossim, DVfirst->deltaPosition.x,
                              DVfirst->deltaPosition.y};
  }
  std::vector<float> calcAngleSimilarty(frameDeltaVector *DVfirst,
                                        frameDeltaVector *DVsecond) {
    auto crease = DVfirst->deltaAngle - DVsecond->deltaAngle;
    auto percfirst = crease / DVfirst->deltaAngle * 100;

    auto screase = DVsecond->deltaAngle - DVfirst->deltaAngle;
    auto spercfirst = screase / DVsecond->deltaAngle * 100;

    float retAngle = 0.0;
    if (std::abs(percfirst) > std::abs(spercfirst)) {
      retAngle = DVfirst->deltaAngle;
    } else {
      retAngle = DVsecond->deltaAngle;
    }
    if (std::isinf(percfirst))
      percfirst = 0.;
    if (std::isinf(spercfirst))
      spercfirst = 0.;

    return std::vector<float>{
        std::max(std::abs(percfirst), std::abs(spercfirst)), retAngle};
  }
};
} // namespace analysis
} // namespace cimarron
