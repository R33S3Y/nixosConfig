{ pkgs, ... }:
let
  version = "dev";
in
{
  environment.systemPackages = [
    (pkgs.stdenv.mkDerivation {
      pname = "deploy";
      version = version;

      src = ./src;

      buildInputs = [
        pkgs.nlohmann_json
        pkgs.libtar
      ];

      nativeBuildInputs = [
        pkgs.gcc
        pkgs.pandoc
      ];

      buildPhase = ''
        g++ \
            server/main.cpp \
            utils/systemHelper.cpp utils/split.cpp utils/strings.cpp utils/ttyHelper.cpp utils/args.cpp utils/nixGet.cpp \
          -o deploy \
          -std=c++23 \
          -g \
          -I${pkgs.nlohmann_json}/include \
          -I${pkgs.libtar}/include

        sed -i 's/version/\"${version}\"/' server/man.md
        pandoc server/man.md -s -t man -o deploy.1
      '';

      installPhase = ''
        mkdir -p $out/bin
        cp deploy $out/bin/

        mkdir -p $out/share/man/man1
        cp deploy.1 $out/share/man/man1
      '';
    })
  ];
}
