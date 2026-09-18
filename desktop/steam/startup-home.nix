{ lib, ... }:
{
  wayland.windowManager.hyprland = {
    enable = true;
    settings = {
      on = {
        _args = [
          "hyprland.start"
          (lib.generators.mkLuaInline "function() hl.exec_cmd(\"steam -silent -no-browser\") end")
        ];
      };
    };
  };
}
