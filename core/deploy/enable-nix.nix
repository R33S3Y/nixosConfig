{ pkgs, ... }:

{
  environment.systemPackages = [
    (pkgs.stdenv.mkDerivation {
      pname = "deploy";
      version = "1.0";

      src = ./src;

      buildInputs = [
        pkgs.nlohmann_json
      ];

      nativeBuildInputs = [
        pkgs.gcc
      ];

      buildPhase = ''
        g++ main.cpp utils/systemHelper.cpp utils/split.cpp utils/strings.cpp utils/ttyHelper.cpp resolve.cpp nixEval.cpp nixEvalStatic.cpp -o deploy \
          -std=c++23 \
          -g \
          -I${pkgs.nlohmann_json}/include
      '';

      installPhase = ''
        mkdir -p $out/bin
        cp deploy $out/bin/
      '';
    })
  ];
}
