/*
 *  Copyright (C) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of partonMCex, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#include "momentum.h"
#include <cmath>
#include <ostream>

namespace pmc {
std::ostream &operator<<(std::ostream &os, const FourMomentum &p) {
    os << "e = " << p.e_ << ", px = " << p.px_ << ", py = " << p.py_
       << ", pz = " << p.pz_;
    return os;
}

FourMomentum boostz(const FourMomentum &p, const double beta) {
    const double gamma = 1.0 / std::sqrt(1 - beta * beta);
    const double gb = gamma * beta;
    FourMomentum boosted(gamma * p.e_ - gb * p.pz_, p.px_, p.py_,
                         -gb * p.e_ + gamma * p.pz_);
    return boosted;
}
}  // namespace pmc
