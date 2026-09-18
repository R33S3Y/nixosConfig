{
  system,
  ...
}:
{
  wayland.windowManager.hyprland = {
    enable = true;
    settings = {
      monitor = system.hosts.${system.host}.monitors;
    };
  };
}
