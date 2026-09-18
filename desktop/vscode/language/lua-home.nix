{
  pkgs,
  ...
}:

{
  programs.vscode = {
    profiles.default = {
      extensions = with pkgs; [
        vscode-extensions.sumneko.lua
      ];
      userSettings = {
        "[lua]".editor = {
          formatOnSave = true;
          defaultFormatter = "sumneko.lua";
        };
      };
    };
  };
}
