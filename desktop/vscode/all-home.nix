{ ... }:
{
  imports = [
    ./settings-home.nix
    ./language/cpp-home.nix
    ./language/javascript-home.nix # ESlint
    ./language/lua-home.nix
    ./language/markdown-home.nix
    ./language/nix-home.nix # Nix LSP and FMT support
    ./language/python-home.nix
  ];
}
