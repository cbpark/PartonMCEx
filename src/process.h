/*
 *  Copyright (C) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of partonMCex, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_PROCESS_H_
#define SRC_PROCESS_H_

#include <array>
#include "momentum.h"

namespace pmc {
/**
 * generates a random 2 -> 2 process with unit momentum at CM frame
 * for a given cos(theta).
 */
class Process {
private:
    double cth_;
    std::array<FourMomentum, 2> mothers_;
    std::array<FourMomentum, 2> daughters_;

public:
    Process() = delete;
    explicit Process(const double cth) : cth_(cth) { init(); }

    std::array<FourMomentum, 2> mothers() const { return mothers_; }
    std::array<FourMomentum, 2> daughters() const { return daughters_; }

private:
    void init();
};
}  // namespace pmc

#endif  // SRC_PROCESS_H_
