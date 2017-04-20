# partonMCex

Solution codes for [How-to: Write a parton level Monte Carlo event generator](https://arxiv.org/abs/1412.4677) written by [A. Papaefstathiou](http://www.physik.uzh.ch/~andreasp/). It has been tested in Linux and macOS.

## Requirements

* C++ compiler supporting C++14 features ([Clang](http://clang.llvm.org/cxx_status.html) > 3.4, [GCC](https://gcc.gnu.org/projects/cxx-status.html) > 4.9).
* [LHAPDF 6](http://lhapdf.hepforge.org/) and the `cteq6l1` PDF data. The version of LHAPDF can be checked by `lhapdf-config --version`. The `cteq6l1` data can be installed by

``` shell
lhapdf-config install cteq6l1
```

## Usage and example outputs

* `./bin/eeZGmumu 1`

```
** e+ e- --> Z/gamma --> mu+ mu- (ECM = 90 GeV)
-- Integrating for cross section and getting maximum ...
---- Done integrating.
---- Maximum value of dsigma = 4.47285e-06, found at costh = -0.999993
---- Total cross section = 1061.05 +- 0.252718 pb
-- Generating events ...
---- event (1)
e-: e = 45, px = 0, py = 0, pz = 45
e+: e = 45, px = 0, py = 0, pz = -45
mu-: e = 45, px = -1.71321, py = 33.4487, pz = -30.0541
mu+: e = 45, px = 1.71321, py = -33.4487, pz = 30.0541
```
