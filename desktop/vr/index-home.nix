{ pkgs, ... }:
{
  home.file."~/.config/openxr/1/active_runtime.json".source =
    "${pkgs.monado}/share/openxr/1/openxr_monado.json";
  home.file."~/.local/share/Steam/steamapps/common/SteamVR/steamzr_linux64.json".source =
    "${pkgs.monado}/share/openxr/1/openxr_monado.json";
}
