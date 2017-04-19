#include <iostream>
// #include "LHAPDF/LHAPDF.h"
#include "LHAPDF/PDFSet.h"

int main() {
    // const LHAPDF::PDF *pdf = LHAPDF::mkPDF("cteq6l1", 0);
    const LHAPDF::PDFSet set("cteq6l1");
    std::cout << "SetDesc: " << set.get_entry("SetDesc") << '\n';
    std::cout << "Verbosity from set: " << set.get_entry("Verbosity") << '\n';
}
