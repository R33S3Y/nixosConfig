{ pkgs, system, ... }:

{
  services.monado = {
    enable = true;
    defaultRuntime = true;
    highPriority = true;
  };

  services.udev.packages = [ pkgs.monado ];

  users.users.${system.user}.extraGroups = [
    "plugdev"
    "input"
  ];

  environment.systemPackages = with pkgs; [
    monado
    libsurvive # backend for lighthouse tracking
    xrizer
  ];

  systemd.user.services.monado.environment = {
    STEAMVR_LH_ENABLE = "0";
    # LH_HANDTRACKING = "0";
  };

  environment.sessionVariables = {
    XR_RUNTIME_JSON = "~/.config/openxr/1/active_runtime.json";
    VR_OVERRIDE = "${pkgs.xrizer}/lib/xrizer";
  };

  hardware.steam-hardware.enable = true;
}
