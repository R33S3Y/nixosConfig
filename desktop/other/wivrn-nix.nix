{ pkgs, ... }:
{

  services.wivrn = {
    enable = true;
    openFirewall = true;
    autoStart = true;
  };

  environment.systemPackages = with pkgs; [ xrizer ];
}
