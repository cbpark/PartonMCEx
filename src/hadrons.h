#ifndef SRC_HADRONS_H_
#define SRC_HADRONS_H_

#include <cmath>
#include "utils.h"

namespace pmc {
enum class Qtype { UP, DOWN };

class InitQuark {
private:
    double x1_, x2_;
    double ymax_;

public:
    InitQuark() = delete;
    explicit InitQuark(const double s, const double hats) {
        ymax_ = -0.5 * std::log(hats / s);
        const double y = (2 * getRandom() - 1.0) * ymax_;
        const double tau = hats / s;
        x1_ = std::sqrt(tau) * std::exp(y);
        x2_ = std::sqrt(tau) * std::exp(-y);
    }

    double x1() const { return x1_; }
    double x2() const { return x2_; }
    double delta_y() const { return 2 * ymax_; }
};

class Rho {
private:
    double rho1_, rho2_;

    double mtr_, gtr_, s_;

public:
    Rho() = delete;
    Rho(const double qmin, const double mtr, const double gtr, const double s)
        : mtr_(mtr), gtr_(gtr), s_(s) {
        const double gm = gtr_ * mtr_;
        rho1_ = std::atan((qmin * qmin - mtr_ * mtr_) / gm);
        rho2_ = std::atan((s_ - mtr_ * mtr_) / gm);
    }

    double s() const { return s_; }

    double delta() const { return rho2_ - rho1_; }

    double jacobian(const double val) const {
        const double cosrho = std::cos(val);
        return mtr_ * gtr_ / (cosrho * cosrho * s_);
    }

    double qscale(const double val) const {
        return std::sqrt(hats(val));
    }

    double hats(const double val) const {
        return mtr_ * gtr_ * std::tan(val) + mtr_ * mtr_;
    }

    friend double rhoValue(const Rho &rho);
};

inline double rhoValue(const Rho &rho) {
    return rho.rho1_ + getRandom() * (rho.rho2_ - rho.rho1_);
}
}  // namespace pmc

#endif  // SRC_HADRONS_H_
