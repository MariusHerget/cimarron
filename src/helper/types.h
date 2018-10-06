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

struct deltaVector {
  float x;
  float y;
  deltaVector(float _x, float _y) : x(_x), y(_y){};
  deltaVector(){};
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

// Motion within a frame
struct TV {
  const cv::RotatedRect trackingVector;
  const cv::Rect initalPosition;
  const int index;
};

struct motionVector {
  std::vector<TV> trackingVectors;
  int frameindex;

  motionVector(std::vector<TV> _trackingVectors, int _frameindex)
      : trackingVectors(_trackingVectors), frameindex(_frameindex){};
};

using motionData = std::vector<motionVector>;

// Motion Delta between two frames
struct frameDeltaVector {
  deltaVector deltaPosition;
  float deltaAngle; // - -> rotating left // + -> rotating right
  float deltaArea;
  frameDeltaVector(deltaVector _deltaPosition, float _deltaAngle,
                   float _deltaArea)
      : deltaPosition(_deltaPosition), deltaAngle(_deltaAngle),
        deltaArea(_deltaArea){};
  frameDeltaVector(){};
};

struct frameDeltaImage {
  std::vector<frameDeltaVector> deltaVectors;
  int frameindex;
  int framenext;
  frameDeltaImage(std::vector<frameDeltaVector> _deltaVectors, int _frameindex,
                  int _framenext)
      : deltaVectors(_deltaVectors), frameindex(_frameindex),
        framenext(_framenext){};
  frameDeltaImage(){};
};
using frameDeltaData = std::vector<frameDeltaImage>;

#endif
