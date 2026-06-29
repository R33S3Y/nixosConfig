{ config, pkgs, ... }:
let
  findPackage = pkgs: name: builtins.head (builtins.filter (p: p.pname or p.name or "" == name) pkgs);
in
{

  systemd = {
    services."lazyRebuild" = {
      enable = true;
      description = "Rebuilds the system on startup";
      wants = [ "default.target" ];

      after = [ "default.target" ];

      path = with pkgs; [
        libnotify
        git
      ];

      serviceConfig = {
        Type = "oneshot";
      };

      script = ''
        set -e

        notify-send -i ${findPackage config.environment.systemPackages "nixSnowflake"}/nixSnowflake.svg "NixOS Auto Rebuild" "Starting rebuild for $HOSTNAME..."
        rm -rf /tmp/config_current
        mkdir -p /tmp/config_current
        chown root /tmp/config_current
        chmod 700 /tmp/config_current
        rm -rf /tmp/config_current/{*,.*}
        git clone https://github.com/R33S3Y/nixosConfig /tmp/config_current
        sudo nixos-rebuild switch --flake /tmp/config_current/#$HOSTNAME && \
          notify-send -i ${findPackage config.environment.systemPackages "nixSnowflake"}/nixSnowflake.svg "NixOS Auto Rebuild" "Rebuild complete!" \
          rm -rf /tmp/config_current || \
          notify-send -i ${findPackage config.environment.systemPackages "nixSnowflake"}/nixSnowflake.svg "NixOS Auto Rebuild" "Rebuild failed!" \
          rm -rf /tmp/config_current
      '';
    };
  };
}
