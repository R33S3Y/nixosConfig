{ pkgs, ... }:
let
  version = "dev";
in
{
  nixpkgs.overlays = [
    (final: prev: {
      internal = (prev.internal or { }) // {

        deploy = prev.stdenv.mkDerivation {
          pname = "deploy";
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
                server/main.cpp \
                utils/systemHelper.cpp utils/split.cpp utils/strings.cpp utils/ttyHelper.cpp \
                utils/args.cpp utils/nixGet.cpp utils/tarHelper.cpp utils/sslHelper.cpp utils/base64.cpp \
              -o deploy \
              -std=c++23 \
              -debug \
              -I${prev.nlohmann_json}/include \
              -I${prev.libtar}/include \
              -L${prev.libtar}/lib -ltar \
              -I${prev.openssl_4_0.dev}/include \
              -L${prev.openssl_4_0.out}/lib -lssl \
              -L${prev.openssl_4_0.out}/lib -lcrypto \
              -I${prev.libssh.dev}/include \
              -L${prev.libssh}/lib -lssh \

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
