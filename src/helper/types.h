#ifndef TYPES_H
#define TYPES_H
using namespace iod;
using namespace vpp;
using namespace s;

using clc_str = char const *;
using frame = image2d<vuchar3>;
using framevector = std::vector<frame>;

struct coordinates {
  int x, y; // Pixelwise

  coordinates(int _x, int _y) : x(_y), y(_y) {}
};

// currently not necessacry
// class correctionData {
// private:
//   std::vector<coordinates> v;
//
// public:
//   correctionData() = default;
//   correctionData(const correctionData &other) = default;
//   correctionData(correctionData &&other) = default;
//   ~correctionData() = default;
//   correctionData &operator=(const correctionData &other) = default;
//   correctionData &operator=(correctionData &&other) = default;
// };

struct TV {
  const cv::RotatedRect trackingVector;
  const cv::Rect initalPosition;
  const int index;
};

struct correctionVector {
  std::vector<TV> trackingVectors;
  int frameindex;

  correctionVector(std::vector<TV> _trackingVectors, int _frameindex)
      : trackingVectors(_trackingVectors), frameindex(_frameindex) {}
};

using correctionData = std::vector<correctionVector>;

#endif
