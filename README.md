# partonMCex

Solution codes for [How-to: Write a parton level Monte Carlo event generator](https://arxiv.org/abs/1412.4677) written by [A. Papaefstathiou](http://www.physik.uzh.ch/~andreasp/). It has been tested in Linux and macOS.

## Requirements

* C++ compiler supporting C++14 features ([Clang](http://clang.llvm.org/cxx_status.html) > 3.4, [GCC](https://gcc.gnu.org/projects/cxx-status.html) > 4.9).
* [LHAPDF 6](http://lhapdf.hepforge.org/) and the `cteq6l1` PDF data. The version of LHAPDF can be checked by `lhapdf-config --version`. The `cteq6l1` data can be installed by

``` shell
lhapdf-config install cteq6l1
```
