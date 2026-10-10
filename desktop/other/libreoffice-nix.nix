{ pkgs, ... }:
{
  environment.systemPackages = with pkgs; [
    libreoffice
    hunspell
    hunspellDicts.en-au-large
  ];

  environment.variables.DICPATH = "${pkgs.hunspellDicts.en-au-large}/share/hunspell";
}
