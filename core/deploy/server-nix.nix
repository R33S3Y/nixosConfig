{ pkgs, ... }:
let
  version = "dev";
in
{
  # overlay deploy package. So that it can be package and accessed with pkgs.internal.deploy
  nixpkgs.overlays = [
    (final: prev: {
      internal = (prev.internal or { }) // {
        # prev.internal is their so we don't overwrite pkgs.internal

        deploy = pkgs.stdenv.mkDerivation {
          pname = "deploy";
          version = version;

          src = ./src;

          buildInputs = with pkgs; [
            nlohmann_json
            libtar
            libssh2
          ];

          nativeBuildInputs = with pkgs; [
            gcc
            pandoc
          ];

          buildPhase = ''
            g++ \
                server/main.cpp \
                utils/systemHelper.cpp utils/split.cpp utils/strings.cpp utils/ttyHelper.cpp utils/args.cpp utils/nixGet.cpp \
              -o deploy \
              -std=c++23 \
              -g \
              -I${pkgs.nlohmann_json}/include \
              -I${pkgs.libtar}/include \
              -L${pkgs.libtar}/lib -ltar \
              -I${pkgs.libssh2.dev}/include \
              -L${pkgs.libssh2}/lib -lssh2 \

            sed -i 's/version/\"${version}\"/' server/man.md
            pandoc server/man.md -s -t man -o deploy.1
          '';

          installPhase = ''
            mkdir -p $out/bin
            cp deploy $out/bin/

            mkdir -p $out/share/man/man1
            cp deploy.1 $out/share/man/man1
          '';
        };
      };
    })
  ];

  environment.systemPackages = [ pkgs.internal.deploy ]; # install it.
}
