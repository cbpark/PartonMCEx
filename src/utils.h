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
}  // namespace pmc

#endif  // SRC_UTILS_H_
