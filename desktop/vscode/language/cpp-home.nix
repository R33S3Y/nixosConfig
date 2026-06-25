{
  pkgs,
  ...
}:

{
  home.packages = with pkgs; [
    llvmPackages.clang-tools # C / CPP 23
  ];
  programs.vscode = {
    profiles.default = {
      extensions = with pkgs; [
        vscode-extensions.llvm-vs-code-extensions.vscode-clangd
      ];
      userSettings = {
        "[cpp]" = {
          editor.defaultFormatter = "llvm-vs-code-extensions.vscode-clangd";
        };
        "[c]" = {
          editor.defaultFormatter = "llvm-vs-code-extensions.vscode-clangd";
        };
        #clangd = {
        #  arguments = [ "-std=c++23" ];
        #  fallbackFlags = [ "-std=c++23" ];
        #};
      };
    };
  };
}
