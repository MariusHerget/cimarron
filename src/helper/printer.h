#ifndef PRINTER_H
#define PRINTER_H
using namespace iod;
using namespace vpp;
using namespace s;

#include "types.h"

namespace cimarron {
namespace print {
void Framevector(framevector frames) {
  for (auto i : frames)
    std::cout << i.has_data() << ' ';
  std::cout << std::endl;
}
// void Coordinates(coordinates c) {
//   std::cout << "(" << c.x << "/" << c.y << ")";
// }
// void CorrectionData(correctionData cd) {
//   std::cout << "CorrectionData (" << cd.size() << "): ";
//   for (auto c : cd) {
//     // cimarron::print::Coordinates(c);
//     std::cout << " ";
//   }
//   std::cout << std::endl;
// }
} // namespace print
} // namespace cimarron
#endif
