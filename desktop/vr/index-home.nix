{ pkgs, ... }:
{
  home.file = {
    ".config/openxr/1/active_runtime.json".source = "${pkgs.monado}/share/openxr/1/openxr_monado.json";

    ".local/share/Steam/steamapps/common/SteamVR/steamxr_linux64.json".source =
      "${pkgs.monado}/share/openxr/1/openxr_monado.json";

    ".local/share/monado/hand-tracking-models".source = pkgs.fetchgit {
      url = "https://gitlab.freedesktop.org/monado/utilities/hand-tracking-models";
      sha256 = "x/X4HyyHdQUxn3CdMbWj5cfLvV7UyQe1D01H93UCk+M=";
      fetchLFS = true;
    };
  };
}
