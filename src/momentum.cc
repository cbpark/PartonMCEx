#include "momentum.h"
#include <ostream>

namespace pmc {
std::ostream &operator<<(std::ostream &os, const FourMomentum &p) {
    os << "e = " << p.e_ << ", px = " << p.px_ << ", py = " << p.py_
       << ", pz = " << p.pz_;
    return os;
}
}  // namespace pmc
