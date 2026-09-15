{ config, pkgs, ... }:

{
  # Enable Monado service
  services.monado = {
    enable = true;
    defaultRuntime = true;  # sets Monado as the active OpenXR runtime
  };

  # Udev rules for Index HMD + controllers + lighthouses
  services.udev.packages = [ pkgs.monado ];

  # Make sure your user is in the right groups for USB/HID access
  users.users.<youruser>.extraGroups = [ "plugdev" "input" ];

  # SteamVR lighthouse driver dependency (for base station tracking via libsurvive or the Valve driver)
  environment.systemPackages = with pkgs; [
    monado
    libsurvive   # if using libsurvive backend for lighthouse tracking instead of SteamVR's driver
  ];
}
