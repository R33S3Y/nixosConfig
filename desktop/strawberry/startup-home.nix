{ lib, ... }:
{
  wayland.windowManager.hyprland = {
    enable = true;
    settings = {
      on = {
        _args = [
          "hyprland.start"
          (lib.generators.mkLuaInline "function() hl.exec_cmd(\"strawberry --play\") end")
        ];
      };
    };
  };
}
