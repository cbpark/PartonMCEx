/*
 *  Copyright (C) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of partonMCex, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include <iostream>
#include "constants.h"

namespace pmc {
inline double fBreitWignerZ(const double hats) {
    const double hats2 = hats - MZ2;
    return 1.0 / (hats2 * hats2 + GAMMAZ2 * MZ2);
}

inline void printProgress(const int i, const int N) {
    std::cout << "progress: " << i * 100.0 / N << "%\r" << std::flush;
}

double getRandom();

inline double costh(const double delta) {
    return -1.0 + getRandom() * delta;
}

inline double phi() {
    return 2.0 * PI * getRandom();
}
}  // namespace pmc

#endif  // SRC_UTILS_H_
