using namespace iod;
using namespace vpp;
using namespace s;

#include "../helper/printer.h"
#include "../helper/types.h"

namespace cimarron {
namespace analysis {
class smoothGDD {
private:
  globalDeltaData &gdd;

public:
  smoothGDD() = default;
  smoothGDD(globalDeltaData &_gdd) : gdd(_gdd) {}

  globalDeltaData smooth(globalDeltaData _gdd) { gdd = _gdd; }

private:
};
} // namespace analysis
} // namespace cimarron
