{ system, ... }:
{

  imports = system.hosts.${system.host}.homeImports;
  # Enable home-manager
  config.programs.home-manager.enable = true;
}
