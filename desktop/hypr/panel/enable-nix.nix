{ pkgs, ... }:

{
  environment.systemPackages = with pkgs; [
    stable.hyprpanel
  ];

  fonts.packages = with pkgs; [
    nerd-fonts.jetbrains-mono
  ];
}
