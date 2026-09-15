{ pkgs, ... }:
{

  services.wivrn = {
    enable = true;
    openFirewall = true;
    autoStart = false;
  };

  environment.systemPackages = with pkgs; [ xrizer ];
}
