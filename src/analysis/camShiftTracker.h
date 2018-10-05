// #include "extractFrames.h"

using namespace iod;
using namespace vpp;
using namespace s;

#include "../helper/printer.h"
#include "../helper/types.h"

namespace cimarron {
namespace analysis {
class camShiftTracker {
private:
  bool _start = true;
  cv::Mat _image;
  cv::Rect _currentRect;
  int _vmin, _vmax, _smin;
  cv::Rect _trackWindow;
  int _hsize = 16;
  cv::Rect _selection;
  const int _index;

public:
  cv::Mat hsv, hue, mask, hist, backproj;

public:
  camShiftTracker(int vmin, int vmax, int smin, const int index)
      : _vmin{vmin}, _vmax(vmax), _smin(smin), _index(index){};

  void setImage(const cv::Mat image) { image.copyTo(_image); };
  cv::Mat getImage() { return _image; };

  void setTrackingRect(const cv::Rect currentRect) {
    _currentRect = currentRect;
    _selection = _currentRect;
  };
  const int getIndex() const { return _index; }
  cv::Rect getTrackingRect() { return _currentRect; };
  cv::RotatedRect track() {
    cv::RotatedRect trackBox;

    float hranges[] = {0, 180};
    const float *phranges = hranges;

    cv::Mat image;
    _image.copyTo(image);

    cv::cvtColor(image, hsv, CV_BGR2HSV);

    cv::inRange(hsv, cv::Scalar(0, _smin, MIN(_vmin, _vmax)),
                cv::Scalar(180, 256, MAX(_vmin, _vmax)), mask);
    int ch[] = {0, 0};
    hue.create(hsv.size(), hsv.depth());
    cv::mixChannels(&hsv, 1, &hue, 1, ch, 1);

    if (_start) {
      cv::Mat roi(hue, _selection), maskroi(mask, _selection);
      cv::calcHist(&roi, 1, 0, maskroi, hist, 1, &_hsize, &phranges);
      cv::normalize(hist, hist, 0, 255, CV_MINMAX);

      _trackWindow = _selection;
      _start = false;
    }

    cv::calcBackProject(&hue, 1, 0, hist, backproj, &phranges);

    backproj &= mask;

    trackBox = cv::CamShift(
        backproj, _trackWindow,
        cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1));

    if (_trackWindow.area() <= 1) {
      int cols = backproj.cols, rows = backproj.rows,
          r = (MIN(cols, rows) + 5) / 6;
      _trackWindow = cv::Rect(_trackWindow.x - r, _trackWindow.y - r,
                              _trackWindow.x + r, _trackWindow.y + r) &
                     cv::Rect(0, 0, cols, rows);
    }

    return trackBox;
  };
};
} // namespace analysis
} // namespace cimarron
