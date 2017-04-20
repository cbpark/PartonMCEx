#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include "LHAPDF/Info.h"  // for LHAPDF::getConfig
#include "LHAPDF/LHAPDF.h"
#include "constants.h"
#include "hadrons.h"
#include "momentum.h"
#include "utils.h"

const int N = 1000000;
const double QMIN = 60.0;
const double MTR = QMIN;
const double GTR = QMIN;

/** scale for the pdfs */
const double MU = MZ;

double weight(std::shared_ptr<LHAPDF::PDF> pdf, const double hats,
              const double mu, const double x1, const double x2,
              const double costh);

int main(int argc, char *argv[]) {
    std::string appname("ppZGmumu");
    if (argc != 3) {
        std::cerr << "Usage: " << appname << " <ECM in GeV> <nevent>\n";
        return 1;
    }

    const double eCM = std::atof(argv[1]);
    std::cout << "** p p --> Z/gamma --> mu+ mu- (ECM = " << eCM << " GeV)\n";

    /*
     * First step: we calculate:
     * - the cross section
     * - and the maximum point of the phase space
     */
    double sum_w = 0, sum_w_sq = 0;  // for the variance
    double w_max = 0;
    double costh_max = -2;

    const double s = eCM * eCM;
    const pmc::Rho rho = pmc::Rho(QMIN, MTR, GTR, s);

    LHAPDF::Info &cfg = LHAPDF::getConfig();
    cfg.set_entry("Verbosity", 0);  // make lhapdf quiet
    std::shared_ptr<LHAPDF::PDF> pdf(LHAPDF::mkPDF("cteq6l1"));

    std::cout << "-- Integrating for cross section and getting maximum ...\n";
    for (int i = 0; i != N; ++i) {
        pmc::printProgress(i, N);

        // random costh and rho
        double costh = pmc::costh(DELTATH);
        double rho_val = pmc::rhoValue(rho);
        double hats = rho.hats(rho_val);

        pmc::InitQuark qin = pmc::InitQuark(s, hats);
        double x1 = qin.x1(), x2 = qin.x2();

        // calculate the phase space point
        double w = weight(pdf, hats, MU, x1, x2, costh) * DELTATH *
                   rho.delta() * qin.delta_y() * rho.jacobian(rho_val);
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
     * We must boost from the CM frame to LAB frame.
     */
    std::cout << "-- Generating events ...\n";
    const int nev = std::atoi(argv[2]);

    int iev = 0;  // counter for event generation
    while (iev < nev) {
        pmc::printProgress(iev, nev);

        double costh = pmc::costh(DELTATH);   // random costh
        double rho_val = pmc::rhoValue(rho);  // random rho
        double hats = rho.hats(rho_val);
        double sqrt_hats = std::sqrt(hats);

        pmc::InitQuark qin = pmc::InitQuark(s, hats);
        double x1 = qin.x1(), x2 = qin.x2();
        double w = weight(pdf, hats, MU, x1, x2, costh) * DELTATH *
                   rho.delta() * qin.delta_y() * rho.jacobian(rho_val) /
                   (x1 * x2);
        double prob = w / w_max;

        // accept the event if the random number is less than the probability of
        // the phase space point
        double r = pmc::getRandom();
        if (r < prob) {
            ++iev;
            std::cout << "---- event (" << iev << ")    \n";

            double phi = pmc::phi();  // generate random phi
            double sinphi = std::sin(phi);
            double cosphi = std::cos(phi);
            double sinth = std::sqrt(1.0 - costh * costh);

            pmc::Particles ps;

            // quark momenta at the LAB frame.
            pmc::FourMomentum pq1(1, 0, 0, 1);
            pq1.scale(0.5 * x1 * eCM);
            ps.push_back(std::make_pair("q1", pq1));
            pmc::FourMomentum pq2(1, 0, 0, -1);
            pq2.scale(0.5 * x2 * eCM);
            ps.push_back(std::make_pair("q2", pq2));

            // muon momenta at the CM frame.
            pmc::FourMomentum pmm(1, sinth * cosphi, sinth * sinphi, costh);
            pmm.scale(0.5 * sqrt_hats);
            pmc::FourMomentum pmp(1, -sinth * cosphi, -sinth * sinphi, -costh);
            pmp.scale(0.5 * sqrt_hats);
            // boost to the LAB frame.
            double beta = (x2 - x1) / (x2 + x1);
            pmm = boostz(pmm, beta);
            pmp = boostz(pmp, beta);
            ps.push_back(std::make_pair("mu-", pmm));
            ps.push_back(std::make_pair("mu+", pmp));

            pmc::printMomenta(ps);
        }
    }
}

double dsigma(const double costh, const double hats, const pmc::Qtype typ) {
    const double kappa = std::sqrt(2) * GF * MZ2 / (4 * PI * ALPHA);
    const double fBW = pmc::fBreitWignerZ(hats);
    const double chi1 = kappa * hats * (hats - MZ2) * fBW;
    const double chi2 = kappa * kappa * hats * hats * fBW;

    // CL and CR for leptons
    const double cVl = -0.5 + 2 * SW2;
    const double cAl = -0.5;

    // CL and CR for quarks
    double cVf, cAf, qf;
    if (typ == pmc::Qtype::UP) {  // up-type quarks
        cVf = 0.5 - 4.0 * SW2 / 3;
        cAf = 0.5;
        qf = 2.0 / 3;
    } else {  // down-type quarks
        cVf = -0.5 + 2.0 * SW2 / 3;
        cAf = -0.5;
        qf = -1.0 / 3;
    }

    const double a0 = qf * qf - 2 * qf * cVl * cVf * chi1 +
                      (cVl * cVl + cAl * cAl) * (cVf * cVf + cAf * cAf) * chi2;
    const double a1 =
        -4 * qf * cAl * cAf * chi1 + 8 * cAl * cVl * cAf * cVf * chi2;
    //  1/3 from initial color averaging
    const double prefac = 2 * PI * ALPHA * ALPHA / (4 * hats) / 3;

    return prefac * (a0 * (1 + costh * costh) + a1 * costh);
}

double weight(std::shared_ptr<LHAPDF::PDF> pdf, const double hats,
              const double mu, const double x1, const double x2,
              const double costh) {
    // up-type quarks
    pmc::Qtype typ = pmc::Qtype::UP;
    double w = dsigma(costh, hats, typ) *
               (pdf->xfxQ(2, x1, mu) * pdf->xfxQ(-2, x2, mu) +
                pdf->xfxQ(4, x1, mu) * pdf->xfxQ(-4, x2, mu));
    w += dsigma(-costh, hats, typ) *
         (pdf->xfxQ(-2, x1, mu) * pdf->xfxQ(2, x2, mu) +
          pdf->xfxQ(-4, x1, mu) * pdf->xfxQ(4, x2, mu));

    // down-type quarks
    typ = pmc::Qtype::DOWN;
    w += dsigma(costh, hats, typ) *
         (pdf->xfxQ(1, x1, mu) * pdf->xfxQ(-1, x2, mu) +
          pdf->xfxQ(3, x1, mu) * pdf->xfxQ(-3, x2, mu));
    w += dsigma(-costh, hats, typ) *
         (pdf->xfxQ(-1, x1, mu) * pdf->xfxQ(1, x2, mu) +
          pdf->xfxQ(-3, x1, mu) * pdf->xfxQ(3, x2, mu));

    return w / (x1 * x2);  // xfxQ is x * f(x, Q^2)
}
