/*
 *  Copyright (C) 2017 Chan Beom Park <cbpark@gmail.com>
 *
 *  This file is part of partonMCex, which is released under the GNU General
 *  Public License. See file LICENSE in the top directory of this project or
 *  go to <http://www.gnu.org/licenses/> for full license details.
 */

#ifndef SRC_MOMENTUM_H_
#define SRC_MOMENTUM_H_

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace pmc {
class FourMomentum {
private:
    double e_;
    double px_;
    double py_;
    double pz_;

public:
    FourMomentum() = default;
    FourMomentum(const double e, const double px, const double py,
                 const double pz)
        : e_(e), px_(px), py_(py), pz_(pz) {}

    void scale(const double s) {
        e_ = e_ * s;
        px_ = px_ * s;
        py_ = py_ * s;
        pz_ = pz_ * s;
    }

    friend std::ostream &operator<<(std::ostream &os, const FourMomentum &p);

    friend FourMomentum boostz(const FourMomentum &p, const double beta);
};

using Particle = std::pair<std::string, FourMomentum>;
using Particles = std::vector<Particle>;

inline Particles scaleMomenta(std::unique_ptr<Particles> ps, const double s) {
    Particles ps_scaled;
    for (Particle p : *ps) {
        p.second.scale(s);
        ps_scaled.push_back(p);
    }
    return ps_scaled;
}

inline void printMomenta(const Particles &ps) {
    for (const Particle p : ps) {
        std::cout << p.first << ": ";
        std::cout << p.second << '\n';
    }
}
}  // namespace pmc

#endif  // SRC_MOMENTUM_H_
