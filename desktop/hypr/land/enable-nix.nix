{ pkgs, ... }:

{
  programs.hyprland.enable = true;

  environment.systemPackages = with pkgs; [
    hyprpicker
    hyprshot
    hyprcursor
    rose-pine-cursor
    kdePackages.breeze-icons
  ];
}
