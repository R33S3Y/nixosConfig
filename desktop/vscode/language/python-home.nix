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
        vscode-extensions.ms-python.python
      ];
      userSettings = {
        "[python]" = {
          editor.defaultFormatter = "charliermarsh.ruff";
        };
      };
    };
  };
}
