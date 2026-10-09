{ lib, pkgs, ... }:
{
  environment.systemPackages = with pkgs; [
    libreoffice
    hunspell
    hunspellDicts.en-au-large
    hunspellDicts.en-us-large
  ];

  environment.variables.DICPATH = lib.makeSearchPath "share/hunspell" [
    pkgs.hunspellDicts.en-us-large
    pkgs.hunspellDicts.en-au-large
  ];
}
