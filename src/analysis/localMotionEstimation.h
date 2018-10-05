// #include "extractFrames.h"

using namespace iod;
using namespace vpp;
using namespace s;

#include "../helper/printer.h"
#include "../helper/types.h"
#include "./camShiftTracker.h"

namespace cimarron {
namespace analysis {
class localMotionEstimation {
private:
  framevector &frames;
  std::vector<cv::Rect> trackingAreas;
  std::vector<camShiftTracker> camShiftTrackers;

public:
  localMotionEstimation(framevector &_frames) : frames(_frames) {
    auto f = frames[0];
    // Use 5 squares to follow them
    trackingAreas = std::vector<cv::Rect>{
        cv::Rect((int)f.domain().ncols() * 0.1, (int)f.domain().nrows() * 0.1,
                 (int)f.domain().ncols() * 0.1, (int)f.domain().nrows() * 0.1),
        cv::Rect((int)f.domain().ncols() * 0.7, (int)f.domain().nrows() * 0.1,
                 (int)f.domain().ncols() * 0.1, (int)f.domain().nrows() * 0.1),
        cv::Rect((int)f.domain().ncols() * 0.1, (int)f.domain().nrows() * 0.7,
                 (int)f.domain().ncols() * 0.1, (int)f.domain().nrows() * 0.1),
        cv::Rect((int)f.domain().ncols() * 0.7, (int)f.domain().nrows() * 0.7,
                 (int)f.domain().ncols() * 0.1, (int)f.domain().nrows() * 0.1),
        cv::Rect((int)f.domain().ncols() * 0.4, (int)f.domain().nrows() * 0.4,
                 (int)f.domain().ncols() * 0.1, (int)f.domain().nrows() * 0.1)};
    std::cout << "trackingAreas: " << trackingAreas.size() << std::endl;
  };

  correctionData estimateBlockWise(int boxSize) {
    correctionData cd;

    const int _vmin = 10, _vmax = 256, _smin = 30;
    for (auto rec : trackingAreas) {
      camShiftTracker tracker(_vmin, _vmax, _smin);
      tracker.setTrackingRect(rec);
      camShiftTrackers.push_back(tracker);
    }

    int fnumber = 0;
    for (frame f : frames) {
      auto frameCD = imageMotionEstimationBlock(f, boxSize, fnumber);
      cd.push_back(frameCD);
      auto imagec = clone(f, _border = 0);
      fill_border_mirror(imagec);
      auto image = to_opencv(imagec);
      for (auto blockTV : frameCD.trackingVectors) {
        // Draw tracking areas.
        cv::Point2f vertices2f[4];
        blockTV.points(vertices2f);

        for (int i = 0; i < 4; i++)
          cv::line(image, vertices2f[i], vertices2f[(i + 1) % 4],
                   cv::Scalar(0, 255, 0), 2);

        cv::rectangle(image, blockTV.boundingRect(), cv::Scalar(255, 0, 0), 2);
      }
      if (cd.size() > 1)
        for (int i = 1; i < cd.size(); i++)
          for (int o = 0; o < cd[i].trackingVectors.size(); o++)
            cv::line(image,
                     cv::Point(cd[i].trackingVectors[o].center.x,
                               cd[i].trackingVectors[o].center.y),
                     cv::Point(cd[i - 1].trackingVectors[o].center.x,
                               cd[i - 1].trackingVectors[o].center.y),
                     cv::Scalar(255, 255, 255), 1);

      cv::imshow("Tracking Areas", image);
      cv::waitKey(1);
      fnumber++;
    }
    // return std::vector<int, int, int>(0, 0, 0);
    return cd;
  }

private:
  correctionVector imageMotionEstimationBlock(frame f, int boxSize,
                                              int fnumber) {
    // copy and set current Frame
    auto image = clone(f, _border = 0);
    fill_border_mirror(image);
    auto imagecv = to_opencv(image);

    // Preparation for saving all Results
    std::vector<cv::RotatedRect> retCV;

    for (auto tracker : camShiftTrackers) {
      tracker.setImage(imagecv);
      if (tracker.track().boundingRect().area() <= 1)
        continue;
      retCV.push_back(tracker.track());
    }

    return correctionVector(retCV, fnumber);
  }
}; // namespace analysis
} // namespace analysis
} // namespace cimarron
