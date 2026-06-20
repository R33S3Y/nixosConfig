{ inputs, home-manager }:

let
  pkgs = import inputs.nixpkgs {
    system = "x86_64-linux"; # or inherit system if passed from the flake
    config.allowUnfree = true;
  };
  obsidian = {
    imports = [
      # Nix modules
      inputs.stylix.nixosModules.stylix
      home-manager.nixosModules.home-manager

      # Core
      ../../../core/core-nix.nix

      # Services

      # pihole
      ../../../services/pihole/pihole-nix.nix

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

    hostName = "obsidian";

    static = {
      enable = true;

      interface = "ens18";
      ipv4 = {
        address = "192.168.1.249";
        prefixLength = 24;
      };
      gatewayAddress = "192.168.1.1";

      nameservers = [ "192.168.1.249" ];
    };
  };

in
obsidian
