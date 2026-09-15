{ pkgs, system, ... }:

{
  services.monado = {
    enable = true;
    defaultRuntime = true;
  };

  services.udev.packages = [ pkgs.monado ];

  users.users.${system.user}.extraGroups = [
    "plugdev"
    "input"
  ];

  environment.systemPackages = with pkgs; [
    monado
    libsurvive # backend for lighthouse tracking
  ];
}
