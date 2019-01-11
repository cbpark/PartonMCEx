{ pkgs ? (import <nixpkgs> {}) }:

with pkgs;

stdenv.mkDerivation {
  name = "PartonMCEx";
  src = ./.;
  enableParallelBuilding = true;
  buildInputs = [
    lhapdf
    lhapdf.pdf_sets.cteq6l1
  ];

  installPhase = ''
    mkdir -p $out/{bin,lib}
    for destdir in bin lib; do
      cp "$destdir"/* $out/$destdir
    done
  '';
}