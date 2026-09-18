{
  system,
  lib,
  ...
}:
let
  # Finds strings that match ${NIX.type.thing} (eg: ${NIX.user.screenshotFolder}) and replaces it with the value
  overlaySystemNixValuesOnFile =
    { fileName, filePath }:
    let
      vars = {
        #host = system.hosts.${system.host};
        #network = system.networks.${system.network};
        #theme = system.themes.${system.theme};
        user = system.users.${system.user};
      };
    in
    builtins.toFile fileName "${builtins.replaceStrings
      # From
      (lib.mapAttrsToListRecursive (path: value: "\${NIX.${builtins.concatStringsSep "." path}}") vars)
      # To
      (lib.mapAttrsToListRecursive (path: value: toString value) vars)
      # string
      "${builtins.readFile filePath}"
    }";
in
{
  wayland.windowManager.hyprland = {
    enable = true;
    configType = "lua";
    extraLuaFiles = {
      "bind.lua" = {
        autoLoad = true;
        content = overlaySystemNixValuesOnFile {
          fileName = "bind.lua";
          filePath = ./lua/bind.lua;
        };
      };
      "settings.lua" = {
        autoLoad = true;
        content = overlaySystemNixValuesOnFile {
          fileName = "settings.lua";
          filePath = ./lua/settings.lua;
        };
      };
    };
  };
}
