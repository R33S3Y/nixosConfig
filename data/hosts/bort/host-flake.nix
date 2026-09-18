{ inputs }:

let
  pkgs = import inputs.nixpkgs {
    system = "x86_64-linux"; # or inherit system if passed from the flake
    config.allowUnfree = true;
  };
  bort = {
    imports = [
      # Nix modules
      inputs.stylix.nixosModules.stylix
      inputs.home-manager.nixosModules.home-manager
      inputs.nix-minecraft.nixosModules.minecraft-servers

      # Core
      ../../../core/all-nix.nix

      # Minecraft
      ../../../services/minecraft/minecraft-nix.nix
      ../../../services/minecraft/minecraftBackup-nix.nix

      # OTHER
      # boot
      ../../../other/boot/bios-nix.nix
      ../../../other/boot/style-nix.nix

      # LapisLazuli
      ../../../other/lapisLazuli/nfs-nix.nix # Lapis  -  NAS

      # My stuff
      ./hardware-configuration.nix # hardware  -  your hardware settings
    ];
    homeImports = [
      # CORE
      ../../../core/all-home.nix
    ];

    user = "reese";

    network = "homelab";

    hostName = "bort";

    static = {
      enable = true;

      interface = "ens18";
      ipv4 = {
        address = "192.168.1.246";
        prefixLength = 24;
      };
      gatewayAddress = "192.168.1.1";

      nameservers = [ "192.168.1.249" ];
    };
  };
in
bort
