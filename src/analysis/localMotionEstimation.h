// // #include "extractFrames.h"
//
// using namespace iod;
// using namespace vpp;
// using namespace s;
//
// #include "../helper/types.h"
//
// namespace cimarron {
// namespace analysis {
// class localMotionEstimation {
// private:
//   framevector &frames;
//
// public:
//   localMotionEstimation(framevector &_frames) : frames(_frames){};
//
//   decltype(auto) estimateBlockWise(int boxSize) {
//     bool first = true;
//     frame prev;
//     for (frame f : frames) {
//       if (!first) {
//         imageMotionEstimationBlock(prev, f, boxSize);
//       }
//       prev = f;
//     }
//     return std::vector<int, int, int>(0, 0, 0);
//   }
//
// private:
//   decltype(auto) imageMotionEstimationBlock(frame fprev, frame f, int
//   boxSize) {
//     // Given a grid with cell of size 10x10 pixels.
//     block_wise(vint2{boxSize, boxSize}, f, f.domain()) |
//         [](auto fb, box2d box) {
//           // a, b and c are sub images representing A, B
//           // and C at the current cell.
//           // All the algorithms of the library work on sub images.
//           // std::cout << "DEBUG " << fb << " " << box << std::endl;
//           // The box argument is the cell representation of A.domain() and
//           holds
//           // the coordinates of the current cell. box.p1() and box.p2() are
//           // respectively the first and the last pixel of the cell.
//         };
//     return "test";
//   }
// };
// } // namespace analysis
// } // namespace cimarron
