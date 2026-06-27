{ system, ... }:
{

  imports = system.hosts.${system.host}.homeImports;
  # Enable home-manager
  config.programs.home-manager.enable = true;

  # https://nixos.wiki/wiki/FAQ/When_do_I_update_stateVersion
  config.home = {
    stateVersion = "24.11";
    username = system.user;
    homeDirectory = "/home/${system.user}";
  };

}
