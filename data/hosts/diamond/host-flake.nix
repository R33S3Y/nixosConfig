{ inputs }:

let
  pkgs = import inputs.nixpkgs {
    system = "x86_64-linux"; # or inherit system if passed from the flake
    config.allowUnfree = true;
  };
  diamond = {
    imports = [
      # Nix modules
      inputs.stylix.nixosModules.stylix
      inputs.home-manager.nixosModules.home-manager
      inputs.nur.modules.nixos.default
      inputs.disko.nixosModules.disko

      # CORE
      ../../../core/all-nix.nix

      # Desktop
      # Firefox
      ../../../desktop/firefox/enable-nix.nix
      # Hypr
      ../../../desktop/hypr/all-nix.nix
      # Kitty
      ../../../desktop/kitty/enable-nix.nix
      # lazyUpdate - update on rebulid script - requres passwordless nixos-rebuild provided by sudo-nix.nix
      #../../../desktop/lazyUpdate/enable-nix.nix
      #../../../desktop/lazyUpdate/snowflake-nix.nix
      # Lutris
      ../../../desktop/lutris/enable-nix.nix
      # obsidian
      ../../../desktop/obsidian/enable-nix.nix
      # PCmanFM
      ../../../desktop/pcmanfm/enable-nix.nix
      # Rofi
      ../../../desktop/rofi/enable-nix.nix
      # SDDM
      ../../../desktop/sddm/enable-nix.nix
      # Sound
      ../../../desktop/sound/sound-nix.nix
      # steam
      ../../../desktop/steam/enable-nix.nix
      # Strawberry
      ../../../desktop/strawberry/enable-nix.nix # Music player
      # vencord
      ../../../desktop/vencord/enable-nix.nix
      # VR
      ../../../desktop/vr/index-nix.nix
      ../../../desktop/vr/svrbsctl-nix.nix
      ../../../desktop/vr/wayvr-nix.nix
      # VScode
      ../../../desktop/vscode/enable-nix.nix
      # Other
      ../../../desktop/other/print-nix.nix # Print  -  How old are you?

      # OTHER
      # boot
      ../../../other/boot/uefi-nix.nix
      ../../../other/boot/style-nix.nix

      # LapisLazuli
      ../../../other/lapisLazuli/home-nix.nix # Lapis  -  Mount NAS to home
      ../../../other/lapisLazuli/smb-nix.nix # Lapis  -  My NAS! It's here cause I want it!
      ../../../other/lapisLazuli/nfs-nix.nix # Lapis  -  NAS

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
      # Lutris
      ../../../desktop/lutris/settings-home.nix
      # vencord
      ../../../desktop/vencord/settings-home.nix
      # obsidian
      ../../../desktop/obsidian/settings-home.nix
      # Rofi
      ../../../desktop/rofi/style-home.nix # Styles
      # steam
      ../../../desktop/steam/startup-home.nix
      # Strawberry
      ../../../desktop/strawberry/bind-home.nix # Global Hotkeys for music player
      ../../../desktop/strawberry/startup-home.nix
      # VR
      ../../../desktop/vr/index-home.nix
      # VScode
      ../../../desktop/vscode/all-home.nix
    ];

    user = "reese";

    network = "homelab";

    hostName = "diamond";

    lapisLazuli = {
      mount = "/home/reese/lapis_lazuli";
      share = "lapis_lazuli";
      credentials = "/home/reese/Projects/nixosConfig/secrets/diamond-user-access";
    };

    programs = with pkgs; [
      featherpad
      krita
      gimp3

      git
      git-lfs
      vlc
      nano

      prismlauncher
      python314

      protonup-qt # needed for a vrchat fix

      bs-manager

      r2modman
      super-slicer-beta
      openscad
      blender
      freecad

      obs-studio

      keymapp

      kicad
      libreoffice

      qtcreator
      cmake
      gdb

      qbittorrent

      nfs-utils
      libnotify

      osu-lazer-bin
      inkscape
      thunderbird
      file
    ];

    bluetooth = true;

    primaryMonitor = "HDMI-A-1";
    monitors = [
      {
        output = "HDMI-A-1";
        mode = "3840x2160@60";
        position = "0x0";
        scale = 1;
      }
      {
        output = "DP-1";
        mode = "2560x1440@165";
        position = "3840x0";
        scale = 1.066667;
        transform = 3;
      }
      {
        output = "DP-2";
        mode = "1920x1080@180";
        position = "-1080x0";
        scale = 1;
        transform = 3;
      }
    ];
  };
in
diamond
