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
  int _vmin, _vmax, _smin;

public:
  localMotionEstimation(framevector &_frames, int vmin = 10, int vmax = 255,
                        int smin = 50)
      : frames(_frames) {
    auto f = frames[0];
    _vmin = vmin;
    _vmax = vmax;
    _smin = smin;
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
      }
      if (md.size() > 1)
        for (int i = 1; i < md.size(); i++)
          for (int o = 0; o < md[i].trackingVectors.size(); o++)
            cv::line(
                image,
                cv::Point(md[i].trackingVectors[o].trackingVector.center.x,
                          md[i].trackingVectors[o].trackingVector.center.y),
                cv::Point(md[i - 1].trackingVectors[o].trackingVector.center.x,
                          md[i - 1].trackingVectors[o].trackingVector.center.y),
                cv::Scalar(255, 255, 255), 1);

      cv::imshow("Tracking Areas", image);
      cv::waitKey(1);
      fnumber++;
    }
    // return std::vector<int, int, int>(0, 0, 0);
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
