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
        nixos-rebuild-ng
      ];

      serviceConfig = {
        Type = "oneshot";
        User = "root";
        Group = "root";
      };

      script = ''
        set -e

        #notify-send -i ${findPackage config.environment.systemPackages "nixSnowflake"}/nixSnowflake.svg "NixOS Auto Rebuild" "Starting rebuild for $HOSTNAME..."

        rm -rf /tmp/config_current
        mkdir -p /tmp/config_current
        chown root /tmp/config_current
        chmod 700 /tmp/config_current
        rm -rf /tmp/config_current/{*,.*}
        echo "Set folder"
        git clone https://github.com/R33S3Y/nixosConfig /tmp/config_current
        echo "Cloned Repo"
        nixos-rebuild switch --flake /tmp/config_current/#$HOSTNAME && \
          #notify-send -i ${findPackage config.environment.systemPackages "nixSnowflake"}/nixSnowflake.svg "NixOS Auto Rebuild" "Rebuild complete!" \
          rm -rf /tmp/config_current \
          echo "Rebuild complete"|| \
          #notify-send -i ${findPackage config.environment.systemPackages "nixSnowflake"}/nixSnowflake.svg "NixOS Auto Rebuild" "Rebuild failed!" \
          rm -rf /tmp/config_current \
          echo "Rebuild failed"
        echo "Done!"
      '';
    };
  };
}
