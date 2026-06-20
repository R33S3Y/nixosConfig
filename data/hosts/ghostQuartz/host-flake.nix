{
  inputs,
  home-manager,
}:

let
  ghostQuartz = {
    imports = [
      # Nix modules
      inputs.stylix.nixosModules.stylix
      home-manager.nixosModules.home-manager

      # Core
      ../../../core/core-nix.nix

      # Services

      # Auth DNS
      ../../../services/authDNS/knot-nix.nix
      ../../../services/authDNS/options-nix.nix
      ../../../services/authDNS/settings-nix.nix
      ../../../services/authDNS/zoneFiles-nix.nix

      # Minecraft
      ../../../services/minecraft/authDNS-nix.nix

      # OTHER
      # boot
      ../../../other/boot/bios-nix.nix
      ../../../other/boot/style-nix.nix

      # My stuff
      ./hardware-configuration.nix # hardware  -  your hardware settings
    ];
    homeImports = [
      # CORE
      ../../../core/core-home.nix
    ];

    user = "reese";

    network = "homelab";

    hostName = "ghostQuartz";

    static = {
      enable = true;

      interface = "ens18";
      ipv4 = {
        address = "192.168.1.252";
        prefixLength = 24;
      };
      gatewayAddress = "192.168.1.1";

      nameservers = [ "192.168.1.249" ];
    };
  };
in
ghostQuartz
