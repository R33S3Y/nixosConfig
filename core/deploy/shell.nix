# file is called shell.nix instead of shell-nix.nix because it is not imported anywhere and
# is instead intended to be used by running:
# 1. cd-ing into the project dir
# 1. nix develop -f ./core/deploy/shell.nix
# 2. code .
#    (launches vscode at dir . )
# 3. closing the terminal
{
  pkgs ? import <nixpkgs> { },
}:

pkgs.mkShell {
  packages = with pkgs; [
    libtar
    openssl_4_1
    libssh
  ];
  shellHook = ''
    export CPATH="${pkgs.nlohmann_json}/include:${pkgs.libtar}/include:${pkgs.openssl_4_1.dev}/include:${pkgs.libssh.dev}/include:$CPATH"
  '';
}
