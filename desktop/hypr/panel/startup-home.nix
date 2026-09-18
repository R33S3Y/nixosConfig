{ lib, ... }:
{
  wayland.windowManager.hyprland = {
    enable = true;
    settings = {
      on = {
        _args = [
          "hyprland.start"
          (lib.generators.mkLuaInline "hl.dsp.exec_cmd(\"hyprpanel\")")
        ];
      };
    };
  };
}
