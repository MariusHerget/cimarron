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
  std::vector<cv::Mat> hsv{cv::Mat(), cv::Mat(), cv::Mat(), cv::Mat(),
                           cv::Mat()};
  std::vector<cv::Mat> hist{cv::Mat(), cv::Mat(), cv::Mat(), cv::Mat(),
                            cv::Mat()};
  std::vector<cv::Mat> backproj{cv::Mat(), cv::Mat(), cv::Mat(), cv::Mat(),
                                cv::Mat()};
  std::vector<cv::Mat> hue{cv::Mat(), cv::Mat(), cv::Mat(), cv::Mat(),
                           cv::Mat()};
  std::vector<cv::Mat> mask{cv::Mat(), cv::Mat(), cv::Mat(), cv::Mat(),
                            cv::Mat()};
  cv::Mat histimg;
  int hsize = 16;
  std::vector<cv::Rect> trackingAreas;

public:
  localMotionEstimation(framevector &_frames) : frames(_frames) {
    histimg = cv::Mat::zeros(_frames[0].domain().ncols(),
                             _frames[0].domain().nrows(), CV_8UC3);
    auto f = frames[0];
    // Use 5 squares to follow them
    trackingAreas = std::vector<cv::Rect>{
        // cv::Rect((int)f.domain().ncols() * 0.1, (int)f.domain().nrows() *
        // 0.1,
        //          (int)f.domain().ncols() * 0.1, (int)f.domain().nrows() *
        //          0.1),
        // cv::Rect((int)f.domain().ncols() * 0.7, (int)f.domain().nrows() *
        // 0.1,
        //          (int)f.domain().ncols() * 0.1, (int)f.domain().nrows() *
        //          0.1),
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
    int fnumber = 0;
    for (frame f : frames) {
      auto l = imageMotionEstimationBlock(f, boxSize, fnumber);
      cd.push_back(l);
      auto imagec = clone(f, _border = 0);
      fill_border_mirror(imagec);
      auto image = to_opencv(imagec);
      auto imagebj = to_opencv(clone(f, _border = 0));
      cv::cvtColor(backproj[0], imagebj, cv::COLOR_GRAY2BGR);
      for (auto rec : trackingAreas) {
        // Draw tracking areas.
        cv::rectangle(image, rec, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
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
      cv::imshow("Backproj", imagebj);
      cv::waitKey(1);
      fnumber++;
    }
    // return std::vector<int, int, int>(0, 0, 0);
    return cd;
  }

private:
  correctionVector imageMotionEstimationBlock(frame f, int boxSize,
                                              int fnumber) {
    // Given a grid with cell of size 10x10 pixels.

    float hranges[] = {0, 20};
    std::vector<const float *> phranges{hranges, hranges, hranges, hranges,
                                        hranges};
    auto image = to_opencv(clone(f, _border = 0));
    const int _vmin = 30, _vmax = 50, smin = 50;
    int n = 0;
    std::vector<cv::RotatedRect> retCV;

    for (cv::Rect &rec : trackingAreas) {
      rec &= cv::Rect(0, 0, image.cols, image.rows);
      cv::cvtColor(image, hsv[n], cv::COLOR_BGR2HSV);
      cv::inRange(hsv[n], cv::Scalar(0, smin, MIN(_vmin, _vmax)),
                  cv::Scalar(180, 255, MAX(_vmin, _vmax)), mask[n]);
      int ch[] = {0, 0};
      hue[n].create(hsv[n].size(), hsv[n].depth());
      cv::mixChannels(&hsv[n], 1, &hue[n], 1, ch, 1);

      cv::Mat roi(hue[n], rec), maskroi(mask[n], rec);
      cv::calcHist(&roi, 1, 0, maskroi, hist[n], 1, &hsize, &phranges[n]);
      cv::normalize(hist[n], hist[n], 0, 255, cv::NORM_MINMAX);

      cv::calcBackProject(&hue[n], 1, 0, hist[n], backproj[n], &phranges[n]);
      backproj[n] &= mask[n];
      cv::RotatedRect trackBox = cv::meanShift(
          backproj[n], rec,
          cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 10,
                           1));
      if (rec.area() <= 1) {
        int cols = backproj[n].cols, rows = backproj[n].rows,
            r = (MIN(cols, rows) + 5) / 6;
        rec = cv::Rect(rec.x - r, rec.y - r, rec.x + r, rec.y + r) &
              cv::Rect(0, 0, cols, rows);
      }
      cv::cvtColor(backproj[n], image, cv::COLOR_GRAY2BGR);
      retCV.push_back(trackBox);
      n++;
    }

    return correctionVector(retCV, fnumber);
  }
}; // namespace analysis
} // namespace analysis
} // namespace cimarron
