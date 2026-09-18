{ lib, ... }:
{
  wayland.windowManager.hyprland = {
    enable = true;
    settings = {
      bind = [
        {
          _args = [
            "SUPER + L"
            lib.generators.mkLuaInline
            "hl.dsp.exec_cmd(\"hyprlock\")"
          ];
        }
      ];
    };
  };
}
