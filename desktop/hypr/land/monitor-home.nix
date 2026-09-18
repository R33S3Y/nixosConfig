{
  system,
  ...
}:
{
  wayland.windowManager.hyprland = {
    settings = {
      monitor = system.hosts.${system.host}.monitors;
    };
  };
}
