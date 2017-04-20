#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "constants.h"
#include "momentum.h"
#include "utils.h"

/** CM energy in GeV */
const double ECM = 90.0;
const double HATS = ECM * ECM;

/** number of integration points */
const int N = 1000000;

/** The differential cross section for e+e- --> Z/gamma --> mu+mu- */
double dsigma(const double costh);

int main(int argc, char *argv[]) {
    const std::string appname("eeZGmumu");
    if (argc != 2) {
        std::cerr << "Usage: " << appname << " nevent\n";
        return 1;
    }

    std::cout << "** e+ e- --> Z/gamma --> mu+ mu- (ECM = " << ECM << " GeV)\n";

    /*
     * First step: we calculate:
     * - the cross section
     * - and the maximum point of the phase space
     */
    double sum_w = 0, sum_w_sq = 0;  // for the variance
    double w_max = 0;
    double costh_max = -2;

    std::cout << "-- Integrating for cross section and getting maximum ...\n";
    for (int i = 0; i != N; ++i) {
        pmc::printProgress(i, N);

        // random costh
        double costh = pmc::costh(DELTATH);
        // calculate the phase space point
        double w = dsigma(costh) * DELTATH;
        sum_w += w;  // add to the sums
        sum_w_sq += w * w;
        if (w > w_max) {  // check if higher than maximum
            w_max = w;
            costh_max = costh;
        }
    }

    // cross section
    const double sigma = sum_w / N;

    // calculate the error
    const double variance = sum_w_sq / N - std::pow(sum_w / N, 2);
    const double error = std::sqrt(variance / N);

    std::cout << "---- Done integrating.\n";
    std::cout << "---- Maximum value of dsigma = " << w_max
              << ", found at costh = " << costh_max << '\n';
    std::cout << "---- Total cross section = " << sigma * PBCONV << " +- "
              << error * PBCONV << " pb\n";

    /*
     * Second step: we generate events
     *
     * Since we have only one parameter, costh, we can translate the parameter
     * into outgoing momenta.
     */
    std::cout << "-- Generating events ...\n";
    const int nev = std::atoi(argv[1]);
    std::vector<double> ps_costh;
    const double ecm_half = ECM / 2;

    int iev = 0;  // counter for event generation
    while (iev < nev) {
        pmc::printProgress(iev, nev);

        double costh = pmc::costh(DELTATH);  // random costh
        double w = dsigma(costh) * DELTATH;     // phase space point
        double prob = w / w_max;

        // accept the event if the random number is less than the probability of
        // the phase space point
        double r = pmc::getRandom();
        if (r < prob) {
            ++iev;
            std::cout << "---- event (" << iev << ")    \n";
            ps_costh.push_back(costh);

            double phi = pmc::phi();  // generate random phi
            double sinphi = std::sin(phi);
            double cosphi = std::cos(phi);
            double sinth = std::sqrt(1.0 - costh * costh);

            auto ps = std::make_unique<pmc::Particles>();
            pmc::FourMomentum pem(1, 0, 0, 1);
            ps->push_back(std::make_pair("e-", pem));
            pmc::FourMomentum pep(1, 0, 0, -1);
            ps->push_back(std::make_pair("e+", pep));
            pmc::FourMomentum pmm(1, sinth * cosphi, sinth * sinphi, costh);
            ps->push_back(std::make_pair("mu-", pmm));
            pmc::FourMomentum pmp(1, -sinth * cosphi, -sinth * sinphi, -costh);
            ps->push_back(std::make_pair("mu+", pmp));
            auto ps_scaled = pmc::scaleMomenta(std::move(ps), ecm_half);
            pmc::printMomenta(ps_scaled);
        }
    }
}

double dsigma(const double costh) {
    const double cV = -0.5 + 2 * SW2;
    const double cV2 = cV * cV;
    const double cA = -0.5;
    const double cA2 = cA * cA;

    const double kappa = std::sqrt(2) * GF * MZ2 / (4 * PI * ALPHA);

    const double fBW = pmc::fBreitWignerZ(HATS);
    const double chi1 = kappa * HATS * (HATS - MZ2) * fBW;
    const double chi2 = kappa * kappa * HATS * HATS * fBW;

    const double a0 = 1.0 + 2 * cV2 * chi1 + std::pow(cA2 + cV2, 2) * chi2;
    const double a1 = 4 * cA2 * chi1 + 8 * cA2 * cV2 * chi2;

    // 2 * pi comes from d\phi integral
    const double prefac = 2 * PI * ALPHA * ALPHA / (4 * HATS);

    return prefac * (a0 * (1 + costh * costh) + a1 * costh);
}
