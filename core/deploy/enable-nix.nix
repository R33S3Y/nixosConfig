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
      ];

      nativeBuildInputs = [
        pkgs.gcc
        pkgs.pandoc
      ];

      buildPhase = ''
        g++ main.cpp utils/systemHelper.cpp utils/split.cpp utils/strings.cpp utils/ttyHelper.cpp resolve.cpp nixEval.cpp nixEvalStatic.cpp dynamic.cpp -o deploy \
          -std=c++23 \
          -g \
          -I${pkgs.nlohmann_json}/include

        sed -i 's/version/\"${version}\"/' man.md
        pandoc man.md -s -t man -o deploy.1
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
