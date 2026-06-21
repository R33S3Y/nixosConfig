{
  inputs,
  home-manager,
  nur,
}:

let
  pkgs = import inputs.nixpkgs {
    system = "x86_64-linux"; # or inherit system if passed from the flake
    config.allowUnfree = true;
  };
  host = {
    imports = [
      # Nix modules
      inputs.stylix.nixosModules.stylix
      home-manager.nixosModules.home-manager
      nur.modules.nixos.default
      inputs.nix-minecraft.nixosModules.minecraft-servers

      # Core
      ../../../core/core-nix.nix

      # Desktop
      # You can comment and uncomment these as needed

      # Firefox
      ../../../desktop/firefox/enable-nix.nix
      # Hyprland
      ../../../desktop/hyprland/enable-nix.nix
      # Hyprlock
      ../../../desktop/hyprlock/enable-nix.nix
      # Hyprpanel
      ../../../desktop/hyprpanel/enable-nix.nix
      # Kitty
      ../../../desktop/kitty/enable-nix.nix
      # lazyUpdate - update on rebulid script - requres passwordless nixos-rebuild provided by sudo-nix.nix
      ../../../desktop/lazyUpdate/enable-nix.nix
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
      # VScode
      ../../../desktop/vscode/enable-nix.nix
      # Other
      ../../../desktop/other/alvr-nix.nix # ALVR  -  For my vr nerds
      ../../../desktop/other/print-nix.nix # Print  -  How old are you?
      ../../../desktop/other/x11-nix.nix # x11  -  needed for Xwayland??

      # Services

      # Auth DNS
      ../../../services/authDNS/knot-nix.nix
      ../../../services/authDNS/options-nix.nix
      ../../../services/authDNS/settings-nix.nix
      ../../../services/authDNS/zoneFiles-nix.nix

      # Jellyfin
      ../../../services/jellyfin/jellyfin-nix.nix

      # Minecraft
      ../../../services/minecraft/minecraft-nix.nix
      ../../../services/minecraft/minecraftBackup-nix.nix
      ../../../services/minecraft/authDNS-nix.nix

      # pihole
      ../../../services/pihole/pihole-nix.nix

      # OTHER
      # boot
      ../../../other/boot/bios-nix.nix
      ../../../other/boot/uefi-nix.nix
      ../../../other/boot/style-nix.nix

      # LapisLazuli
      ../../../other/lapisLazuli/home-nix.nix # Lapis  -  Mount NAS to home
      ../../../other/lapisLazuli/smb-nix.nix # Lapis  -  My NAS! It's here cause I want it!
      ../../../other/lapisLazuli/nfs-nix.nix # Lapis  -  NAS
    ];
    homeImports = [
      # CORE
      ../../../core/core-home.nix

      # DESKTOP
      # You can comment and uncomment these as needed

      # Firefox
      ../../../desktop/firefox/settings-home.nix
      # git
      ../../../desktop/git/settings-home.nix
      # Hyprland
      ../../../desktop/hyprland/bind-home.nix # Keyboard bindings
      ../../../desktop/hyprland/monitor-home.nix # Monitor settings
      ../../../desktop/hyprland/settings-home.nix # Settings
      ../../../desktop/hyprland/style-home.nix # Styles tweaks  -  (Most styling is handled by stylix)
      # Hyprlock
      ../../../desktop/hyprlock/style-home.nix # Styles + What to display and where
      # Hyprpanel
      ../../../desktop/hyprpanel/style-home.nix
      # Kitty
      ../../../desktop/kitty/bind-home.nix # Key binds
      ../../../desktop/kitty/style-home.nix # Styles  -  You should be fine to get away with disabling this
      ../../../desktop/kitty/settings-home.nix # Settings
      # lazyUpdate - update on rebulid script - requres passwordless nixos-rebuild provided by sudo-nix.nix
      ../../../desktop/lazyUpdate/run-home.nix
      # vencord
      ../../../desktop/vencord/settings-home.nix
      # obsidian
      ../../../desktop/obsidian/settings-home.nix
      # Rofi
      ../../../desktop/rofi/style-home.nix # Styles
      # Sound
      ../../../desktop/sound/sound-home.nix
      # steam
      ../../../desktop/steam/startup-home.nix
      # Strawberry
      ../../../desktop/strawberry/bind-home.nix # Global Hotkeys for music player
      # VScode
      ../../../desktop/vscode/settings-home.nix
      ../../../desktop/vscode/language/cpp-home.nix
      ../../../desktop/vscode/language/javascript-home.nix # ESlint
      ../../../desktop/vscode/language/markdown-home.nix
      ../../../desktop/vscode/language/nix-home.nix # Nix LSP and FMT support

    ];

    user = "reese";

    network = "homelab";

    hostName = "template";

    static = {
      enable = true;

      interface = "enp10s0";
      ipv4 = {
        address = "192.168.1.1";
        prefixLength = 24;
      };
      gatewayAddress = "192.168.1.1";

      nameservers = [ "192.168.1.249" ];
    };

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

    microphone = {
      # use wpctl status to get device names
      name = "Blue Microphones Analog Stereo";
      volume = "1";
      bluetooth = {
        enable = false;
        id = ""; # type bluetoothctl and then info to get this info
      };
    };
    speaker = {
      name = "Baseus Inspire XC1";
      volume = "0.4";
      bluetooth = {
        enable = true;
        id = "54:84:50:67:C9:A2"; # type bluetoothctl and then info to get this info
      };
    };
    primaryMonitor = "HDMI-A-1";
    monitor = [
      "HDMI-A-1, 1920x1080@60, 0x0, 1"
      "DP-3, 2560x1440@165, 1920x-200, 2, transform,3"
      "DP-2, 1920x1080@60, -960x375, 2"
    ];
  };
in
host
