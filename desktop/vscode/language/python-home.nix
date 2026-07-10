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
        "[python]".editor = {
          formatOnSave = true;
          codeActionsOnSave.source = {
            fixAll = "explicit";
            organizeImports = "explicit";
          };
          "editor.defaultFormatter" = "charliermarsh.ruff";
        };
      };
    };
  };
}
