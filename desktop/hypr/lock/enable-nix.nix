{ config, pkgs, ... }:

{
  environment.systemPackages = with pkgs; [
    hyprlock
  ];

  security.pam.services.hyprlock = {
    text = ''
      auth      include     login
      account   include     login
      password  include     login
      session   include     login
    '';
  };
}
