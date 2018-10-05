// #include "extractFrames.h"

using namespace iod;
using namespace vpp;
using namespace s;

#include "../helper/printer.h"
#include "../helper/types.h"

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
    bool first = true;
    frame prev;
    correctionData cd;
    for (frame f : frames) {
      if (!first) {
        auto l = imageMotionEstimationBlock(prev, f, boxSize);
        cd.push_back(l);
      }
      first = false;
      prev = f;
      auto imagec = clone(f, _border = 0);
      fill_border_mirror(imagec);
      auto image = to_opencv(imagec);
      for (auto rec : trackingAreas) {
        // Draw tracking areas.
        cv::rectangle(image, rec, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
      }
      cv::imshow("Tracking Areas", image);
      cv::waitKey(1);
    }
    // return std::vector<int, int, int>(0, 0, 0);
    return cd;
  }

private:
  coordinates imageMotionEstimationBlock(frame fprev, frame f, int boxSize) {
    // Given a grid with cell of size 10x10 pixels.

    float hranges[] = {0, 20};
    const float *phranges = hranges;
    auto image = to_opencv(clone(f, _border = 0));
    const int _vmin = 132, _vmax = 150, smin = 10;
    int n = 0;
    for (cv::Rect &rec : trackingAreas) {
      cv::cvtColor(image, hsv[n], cv::COLOR_BGR2HSV);
      cv::inRange(hsv[n], cv::Scalar(0, smin, MIN(_vmin, _vmax)),
                  cv::Scalar(180, 255, MAX(_vmin, _vmax)), mask[n]);
      int ch[] = {0, 0};
      hue[n].create(hsv[n].size(), hsv[n].depth());
      cv::mixChannels(&hsv[n], 1, &hue[n], 1, ch, 1);

      cv::Mat roi(hue[n], rec), maskroi(mask[n], rec);
      cv::calcHist(&roi, 1, 0, maskroi, hist[n], 1, &hsize, &phranges);
      cv::normalize(hist[n], hist[n], 0, 255, cv::NORM_MINMAX);

      cv::calcBackProject(&hue[n], 1, 0, hist[n], backproj[n], &phranges);
      backproj[n] &= mask[n];
      cv::RotatedRect trackBox = cv::CamShift(
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
      n++;
    }

    // block_wise(vint2{boxSize, boxSize}, fprev, f, f.domain()) |
    //     [&](auto fps, auto fs, box2d box) {
    //       auto fsc = to_opencv(fs);
    //       cv::cvtColor(fsc, hsv, CV_BGR2HSV);
    //       hue.create(hsv.size(), hsv.depth());
    //       int ch[] = {0, 0};
    //       cv::mixChannels(&hsv, 1, &hue, 1, ch, 1);
    //       cv::Rect selection =
    //           cv::Rect(box.p1()[0], box.p1()[1], boxSize, boxSize);
    //
    //       cv::Mat roi(hue, selection), maskroi(mask, selection);
    //       cv::calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
    //       cv::normalize(hist, hist, 0, 255, cv::NORM_MINMAX);
    //
    //       // Perform CAMShift
    //       cv::calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
    //       // backproj &= mask;
    //       // cv::RotatedRect trackBox = cv::CamShift(
    //       //     backproj, selection,
    //       //     cv::TermCriteria(cv::TermCriteria::EPS |
    //       //     cv::TermCriteria::COUNT,
    //       // 10, 1));
    //       // if (selection.area() <= 1) {
    //       //   int cols = backproj.cols, rows = backproj.rows,
    //       //       r = (MIN(cols, rows) + 5) / 6;
    //       //   selection = cv::Rect(selection.x - r, selection.y - r,
    //       //                        selection.x + r, selection.y + r) &
    //       //               cv::Rect(0, 0, cols, rows);
    //       // }
    //       cv::cvtColor(backproj, fsc, cv::COLOR_GRAY2BGR);
    //       cv::imshow("Analysis backproj", fsc);
    //       cv::waitKey(1);
    //       // a, b and c are sub images representing A, B
    //       // and C at the current cell.
    //       // All the algorithms of the library work on sub images.
    //       // std::cout << "DEBUG " << fb << " " << box << std::endl;
    //       // The box argument is the cell representation of A.domain() and
    //       // holds
    //       // the coordinates of the current cell. box.p1() and box.p2() are
    //       // respectively the first and the last pixel of the cell.
    //     };
    return coordinates(0, 0);
  }
};
} // namespace analysis
} // namespace cimarron
