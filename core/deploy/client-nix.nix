{ pkgs, ... }:
let
  version = "dev";
in
{
  nixpkgs.overlays = [
    (final: prev: {
      internal = (prev.internal or { }) // {

        deployClient = prev.stdenv.mkDerivation {
          pname = "deployClient";
          version = version;

          src = ./src;

          buildInputs = with prev; [
            nlohmann_json
            libtar
            openssl_4_0
            libssh2
          ];

          nativeBuildInputs = with prev; [
            gcc
            pandoc
          ];

          buildPhase = ''
            g++ \
                client/main.cpp \
                utils/ttyHelper.cpp utils/args.cpp \
              -o deployClient \
              -std=c++23 \
              -I${prev.nlohmann_json}/include \
              -I${prev.libtar}/include \
              -L${prev.libtar}/lib -ltar \
              -I${prev.openssl_4_0.dev}/include \
              -L${prev.openssl_4_0.out}/lib -lssl \
              -L${prev.openssl_4_0.out}/lib -lcrypto \
              -I${prev.libssh.dev}/include \
              -L${prev.libssh}/lib -lssh \

            sed -i 's/version/\"${version}\"/' client/man.md
            pandoc client/man.md -s -t man -o deployClient.1
          '';

          installPhase = ''
            mkdir -p $out/bin
            cp deployClient $out/bin/

            mkdir -p $out/share/man/man1
            cp deployClient.1 $out/share/man/man1
          '';
        };
      };
    })
  ];

  environment.systemPackages = [ pkgs.internal.deployClient ]; # install it.
}
