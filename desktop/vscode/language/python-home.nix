{
  pkgs,
  ...
}:

{
  home.packages = with pkgs; [
    ruff
  ];

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
          defaultFormatter = "charliermarsh.ruff";
        };
      };
    };
  };
}
