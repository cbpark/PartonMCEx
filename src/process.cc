/*
 *  Copyright (C) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of partonMCex, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#include "process.h"
#include <cmath>
#include "constants.h"
#include "utils.h"

namespace pmc {
void Process::init() {
    const double sth = std::sqrt(1.0 - cth_ * cth_);
    const double phi = 2.0 * PI * getRandom();
    const double sinphi = std::sin(phi);
    const double cosphi = std::cos(phi);

    mothers_ = {{FourMomentum(1, 0, 0, 1), FourMomentum(1, 0, 0, -1)}};
    daughters_ = {{FourMomentum(1, sth * cosphi, sth * sinphi, cth_),
                   FourMomentum(1, -sth * cosphi, -sth * sinphi, -cth_)}};
}
}  // namespace pmc
