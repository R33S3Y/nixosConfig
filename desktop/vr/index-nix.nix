{
  pkgs,
  system,
  options,
  lib,
  ...
}:

{

  # hardware stuff
  hardware.steam-hardware.enable = true;
  services.udev.packages = [ pkgs.monado ];

  users.users.${system.user}.extraGroups = [
    "plugdev"
    "input"
  ];

  environment.sessionVariables = {
    # Here so you don't need to add this to every game you launch
    PRESSURE_VESSEL_IMPORT_OPENXR_1_RUNTIMES = "1";
    # technically, not need but why take chances
    XR_RUNTIME_JSON = "/home/${system.user}/.config/openxr/1/active_runtime.json";
    VR_OVERRIDE = "${pkgs.opencomposite}/lib/opencomposite";
  };

  services.monado = {
    enable = true;
    defaultRuntime = true;
    highPriority = true;
    forceDefaultRuntime = true;
  };

  environment.systemPackages = with pkgs; [
    monado
    libsurvive # backend for lighthouse tracking
    opencomposite
  ];

  # Monado service overrides:
  systemd.user.services.monado = {
    environment = {
      STEAMVR_LH_ENABLE = "0";
      # LH_HANDTRACKING = "0";
      SURVIVE_GLOBALSCENESOLVER = "0";
      PRESSURE_VESSEL_IMPORT_OPENXR_1_RUNTIMES = "1";
    };
    serviceConfig = {
      ExecStart = lib.getExe (
        pkgs.writeShellScriptBin "start-monado-server" ''
          # The server needs Steam in PATH to open Steam games from the application launcher
          export PATH="${lib.makeBinPath [ options.programs.steam.package ]}:$PATH"
          ${
            if options.services.monado.highPriority then
              "${options.config.security.wrapperDir}/monado-service"
            else
              lib.getExe' options.services.monado.package "monado-service"
          }
        ''
      );
    };
  };

}
