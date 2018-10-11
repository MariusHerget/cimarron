// #include "extractFrames.h"

using namespace iod;
using namespace vpp;
using namespace s;

#include "../helper/filesystem.h"
#include "../helper/printer.h"
#include "../helper/types.h"
#include "../postprocessing/video_output.h"
#include "./camShiftTracker.h"

namespace cimarron {
namespace analysis {
class localMotionEstimation {
private:
  const framevector &frames;
  std::vector<cv::Rect> trackingAreas;
  std::vector<camShiftTracker> camShiftTrackers;
  int _vmin, _vmax, _smin;

  double sizeTrackingAreas = 0.25;

public:
  localMotionEstimation(framevector const &_frames, int vmin = 10,
                        int vmax = 160, int smin = 25)
      : frames(_frames) {
    auto f = frames[0];
    _vmin = vmin;
    _vmax = vmax;
    _smin = smin;
    // Use 9 squares to follow them
    trackingAreas = std::vector<cv::Rect>{
        cv::Rect((int)f.domain().ncols() * 0.05, (int)f.domain().nrows() * 0.05,
                 (int)f.domain().ncols() * sizeTrackingAreas,
                 (int)f.domain().nrows() * sizeTrackingAreas),

        cv::Rect((int)f.domain().ncols() * (0.5 - sizeTrackingAreas / 2),
                 (int)f.domain().nrows() * 0.05,
                 (int)f.domain().ncols() * sizeTrackingAreas,
                 (int)f.domain().nrows() * sizeTrackingAreas),

        cv::Rect((int)f.domain().ncols() * (0.95 - sizeTrackingAreas),
                 (int)f.domain().nrows() * 0.05,
                 (int)f.domain().ncols() * sizeTrackingAreas,
                 (int)f.domain().nrows() * sizeTrackingAreas),

        cv::Rect((int)f.domain().ncols() * 0.05,
                 (int)f.domain().nrows() * (0.5 - sizeTrackingAreas / 2),
                 (int)f.domain().ncols() * sizeTrackingAreas,
                 (int)f.domain().nrows() * sizeTrackingAreas),

        cv::Rect((int)f.domain().ncols() * (0.5 - sizeTrackingAreas / 2),
                 (int)f.domain().nrows() * (0.5 - sizeTrackingAreas / 2),
                 (int)f.domain().ncols() * sizeTrackingAreas,
                 (int)f.domain().nrows() * sizeTrackingAreas),

        cv::Rect((int)f.domain().ncols() * (0.95 - sizeTrackingAreas),
                 (int)f.domain().nrows() * (0.5 - sizeTrackingAreas / 2),
                 (int)f.domain().ncols() * sizeTrackingAreas,
                 (int)f.domain().nrows() * sizeTrackingAreas),

        cv::Rect((int)f.domain().ncols() * 0.05,
                 (int)f.domain().nrows() * (0.95 - sizeTrackingAreas),
                 (int)f.domain().ncols() * sizeTrackingAreas,
                 (int)f.domain().nrows() * sizeTrackingAreas),

        cv::Rect((int)f.domain().ncols() * (0.5 - sizeTrackingAreas / 2),
                 (int)f.domain().nrows() * (0.95 - sizeTrackingAreas),
                 (int)f.domain().ncols() * sizeTrackingAreas,
                 (int)f.domain().nrows() * sizeTrackingAreas),

        cv::Rect((int)f.domain().ncols() * (0.95 - sizeTrackingAreas),
                 (int)f.domain().nrows() * (0.95 - sizeTrackingAreas),
                 (int)f.domain().ncols() * sizeTrackingAreas,
                 (int)f.domain().nrows() * sizeTrackingAreas)

    };
    std::cout << "trackingAreas: " << trackingAreas.size() << std::endl;
  };

  motionData estimateBlockWise(int boxSize) {
    motionData md;

    int n = 0;
    for (auto rec : trackingAreas) {
      camShiftTracker tracker(_vmin, _vmax, _smin, n);
      tracker.setTrackingRect(rec);
      camShiftTrackers.push_back(tracker);
      n++;
    }

    int fnumber = 0;
    for (frame f : frames) {
      auto frameMD = imageMotionEstimationBlock(f, boxSize, fnumber);
      md.push_back(frameMD);
      auto imagec = clone(f, _border = 0);
      fill_border_mirror(imagec);
      auto image = to_opencv(imagec);
      for (auto blockTV : frameMD.trackingVectors) {
        // Draw tracking areas.
        cv::Point2f vertices2f[4];
        blockTV.trackingVector.points(vertices2f);

        for (int i = 0; i < 4; i++)
          cv::line(image, vertices2f[i], vertices2f[(i + 1) % 4],
                   cv::Scalar(0, 255, 0), 2);

        cv::rectangle(image, blockTV.trackingVector.boundingRect(),
                      cv::Scalar(255, 0, 0), 2);

        // Print trackingvector fnumber
        cv::putText(
            image, std::to_string(blockTV.index), blockTV.trackingVector.center,
            cv::FONT_HERSHEY_COMPLEX_SMALL, 2, (255, 255, 255), 2, cv::LINE_AA);

        // Print intial tracking areas
        for (auto rec : trackingAreas) {
          cv::rectangle(image, rec, cv::Scalar(0, 0, 255), 2);
        }
      }
      if (md.size() > 1)
        for (int i = 1; i < md.size(); i++)
          for (int o = 0; o < md[i].trackingVectors.size(); o++) {
            cv::Scalar color;
            // Print history of tracking vectors --> does not work correct
            if (o == 0)
              color = cv::Scalar((i * 2) % 255, 0, 0);
            else if (o == 1)
              color = cv::Scalar(0, (i * 2) % 255, 0);
            else
              color = cv::Scalar(0, 0, (i * 2) % 255);
            try {
              if (md[i].trackingVectors.size() > 0)
                if (md[i - 1].trackingVectors.size() > 0) {
                  auto olastframe = o;
                  if (md[i].trackingVectors[o].index !=
                      md[i - 1].trackingVectors[olastframe].index) {
                    for (int k = 0; k < md[i - 1].trackingVectors.size(); k++)
                      if (md[i].trackingVectors[o].index ==
                          md[i - 1].trackingVectors[k].index) {
                        olastframe = k;
                      } else {
                        olastframe = -1;
                      }
                  }
                  if (md[i].trackingVectors[o].trackingVector.center.x != 0 &&
                      md[i].trackingVectors[o].trackingVector.center.y != 0 &&
                      olastframe != -1)
                    cv::line(
                        image,
                        cv::Point(
                            md[i].trackingVectors[o].trackingVector.center.x,
                            md[i].trackingVectors[o].trackingVector.center.y),
                        cv::Point(md[i - 1]
                                      .trackingVectors[olastframe]
                                      .trackingVector.center.x,
                                  md[i - 1]
                                      .trackingVectors[olastframe]
                                      .trackingVector.center.y),
                        color, 1);
                }
            } catch (...) {
              // Catch all exceptions – dangerous!!!
              // Respond (perhaps only partially) to the exception, then
              // re-throw to pass the exception to some other handler
              // ...
              throw;
            }
          }
      // cv::Mat imagebj;
      // cv::cvtColor(camShiftTrackers[0].backproj, image, cv::COLOR_GRAY2BGR);
      // cv::imshow("Tracking Areas Backproj", imagebj);
      // cv::waitKey(1);
      cv::imshow("Tracking Areas", image);
      cv::waitKey(1);
      fnumber++;
    }
    return md;
  }

private:
  motionVector imageMotionEstimationBlock(frame f, int boxSize, int fnumber) {
    // copy and set current Frame
    auto image = clone(f, _border = 0);
    fill_border_mirror(image);
    auto imagecv = to_opencv(image);

    // Preparation for saving all Results
    std::vector<TV> retTV;

    for (auto tracker : camShiftTrackers) {
      tracker.setImage(imagecv);
      if (tracker.track().boundingRect().area() <= 1)
        continue;
      retTV.push_back(
          TV{tracker.track(), tracker.getTrackingRect(), tracker.getIndex()});
    }

    return motionVector(retTV, fnumber);
  }
}; // namespace analysis
} // namespace analysis
} // namespace cimarron
