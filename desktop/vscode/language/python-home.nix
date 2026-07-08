{
  pkgs,
  system,
  ...
}:

{

  programs.vscode = {
    profiles.default = {
      extensions = with pkgs; [
        vscode-extensions.charliermarsh.ruff
      ];
      userSettings = {

      };
    };
  };
}
