{ ... }:
{
  imports = [
    # hyprland
    ./land/lua-home.nix
    ./land/monitor-home.nix
    ./land/mouse-home.nix
    ./land/style-home.nix

    # hyprlock
    ./lock/bind-home.nix
    ./lock/style-home.nix

    # hyprpanel
    ./panel/startup-home.nix
    ./panel/style-home.nix
  ];
}
