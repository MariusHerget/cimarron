#ifndef OWN_MATH_H
#define OWN_MATH_H
using namespace iod;
using namespace vpp;
using namespace s;
#include <cmath> // std::abs
float cosine_similarity(frameDeltaVector *A, frameDeltaVector *B) {
  float dot = 0.0, denom_a = 0.0, denom_b = 0.0;

  dot += A->deltaPosition.x * B->deltaPosition.x +
         A->deltaPosition.y * B->deltaPosition.y;
  denom_a +=
      std::pow(A->deltaPosition.x, 2.0) * std::pow(A->deltaPosition.y, 2.0);
  denom_b +=
      std::pow(B->deltaPosition.x, 2.0) * std::pow(B->deltaPosition.y, 2.0);
  auto ret = dot / (sqrt(denom_a) * sqrt(denom_b));
  if (std::isnan(ret))
    return 0.0;
  if (std::isinf(ret)) // cosine_similarity is [-1, 1] therfore 1 and -1 means
                       // similarity
    return 1.;
  return ret;
}
float euclidean_distance(frameDeltaVector *A, frameDeltaVector *B) {
  float x = A->deltaPosition.x - B->deltaPosition.x;
  float y = A->deltaPosition.y - B->deltaPosition.y;
  float dist;

  auto ret = std::sqrt(std::pow(x, 2) + std::pow(y, 2));
  if (std::isnan(ret))
    return 0.0;
  return ret;
}
#endif
