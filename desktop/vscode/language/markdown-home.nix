{ pkgs, ... }:

{
  programs.vscode = {
    profiles.default = {
      extensions = with pkgs; [
        vscode-extensions.prettier.prettier-vscode
      ];
      userSettings = {
        "[markdown]" = {
          editor.defaultFormatter = "esbenp.prettier-vscode";
        };
      };
    };
  };
}
