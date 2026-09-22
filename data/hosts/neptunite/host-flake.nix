{ inputs }:

let
  pkgs = import inputs.nixpkgs {
    system = "x86_64-linux"; # or inherit system if passed from the flake
    config.allowUnfree = true;
  };
  host = {
    imports = [
      # Nix modules
      inputs.stylix.nixosModules.stylix
      inputs.home-manager.nixosModules.home-manager
      inputs.nur.modules.nixos.default
      inputs.disko.nixosModules.disko

      # Core
      ../../../core/all-nix.nix

      # Desktop
      # You can comment and uncomment these as needed

      # Firefox
      ../../../desktop/firefox/enable-nix.nix
      # Hypr
      ../../../desktop/hypr/all-nix.nix
      # Kitty
      ../../../desktop/kitty/enable-nix.nix
      # lazyUpdate - update on rebulid script - requres passwordless nixos-rebuild provided by sudo-nix.nix
      ../../../desktop/lazyUpdate/enable-nix.nix
      ../../../desktop/lazyUpdate/snowflake-nix.nix
      # Lutris
      ../../../desktop/lutris/enable-nix.nix
      # obsidian
      ../../../desktop/obsidian/enable-nix.nix
      # PCmanFM
      ../../../desktop/pcmanfm/enable-nix.nix
      # Plasma
      ../../../desktop/plasma/enable-nix.nix
      # Rofi
      ../../../desktop/rofi/enable-nix.nix
      # SDDM
      ../../../desktop/sddm/enable-nix.nix
      # Sound
      ../../../desktop/sound/sound-nix.nix
      # steam
      #../../../desktop/steam/enable-nix.nix
      # Strawberry
      #../../../desktop/strawberry/enable-nix.nix # Music player
      # vencord
      ../../../desktop/vencord/enable-nix.nix
      # VScode
      ../../../desktop/vscode/all-nix.nix
      # Other
      ../../../desktop/other/print-nix.nix # Print  -  How old are you?

      # OTHER
      # boot
      #../../../other/boot/bios-nix.nix
      ../../../other/boot/uefi-nix.nix
      ../../../other/boot/style-nix.nix

      # LapisLazuli
      #../../../other/lapisLazuli/home-nix.nix # Lapis  -  Mount NAS to home
      #../../../other/lapisLazuli/smb-nix.nix # Lapis  -  My NAS! It's here cause I want it!
      #../../../other/lapisLazuli/nfs-nix.nix # Lapis  -  NAS

      # My stuff
      ./hardware-configuration.nix # hardware  -  your hardware settings
      ./disko-nix.nix
    ];
    homeImports = [
      # CORE
      ../../../core/all-home.nix

      # DESKTOP
      # You can comment and uncomment these as needed

      # Firefox
      ../../../desktop/firefox/settings-home.nix
      # git
      ../../../desktop/git/settings-home.nix
      # Hypr
      ../../../desktop/hypr/all-home.nix
      # Kitty
      ../../../desktop/kitty/bind-home.nix # Key binds
      ../../../desktop/kitty/style-home.nix # Styles  -  You should be fine to get away with disabling this
      ../../../desktop/kitty/settings-home.nix # Settings
      # vencord
      ../../../desktop/vencord/settings-home.nix
      # obsidian
      ../../../desktop/obsidian/settings-home.nix
      # Rofi
      ../../../desktop/rofi/style-home.nix # Styles
      # steam
      #../../../desktop/steam/startup-home.nix
      # Strawberry
      #../../../desktop/strawberry/bind-home.nix # Global Hotkeys for music player
      ../../../desktop/strawberry/startup-home.nix
      # VScode
      ../../../desktop/vscode/all-home.nix
    ];

    user = "reese";

    network = "homelab";

    hostName = "neptunite";

    lapisLazuli = {
      mount = "/home/reese/lapis_lazuli";
      share = "lapis_lazuli";
      credentials = "~/Projects/nixosConfig/secrets/diamond-user-access";
    };

    programs = with pkgs; [
      featherpad
      krita
      gimp3

      git
      vlc
      nano
    ];

    bluetooth = true;

    primaryMonitor = "";
    monitors = [
    ];
  };
in
host
