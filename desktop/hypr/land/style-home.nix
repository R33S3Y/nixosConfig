{
  system,
  ...
}:
{
  wayland.windowManager.hyprland = {
    settings = {
      config = {
        general = {
          gaps_in = system.themes.${system.theme}.borders.padding.inner;
          gaps_out = system.themes.${system.theme}.borders.padding.outer;
          border_size = system.themes.${system.theme}.borders.thickness;

          layout = "dwindle";
        };
        decoration = {
          # See https://wiki.hypr.land/configuring/core/config-options/#decoration for more

          rounding = system.themes.${system.theme}.borders.rounding;

          blur = system.themes.${system.theme}.blur;

          shadow = system.themes.${system.theme}.shadow;

          active_opacity = system.themes.${system.theme}.opacity.active;
          inactive_opacity = system.themes.${system.theme}.opacity.inactive;
        };
      };
    };
  };
}
