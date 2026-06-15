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
      #inputs.nix-minecraft.nixosModules.minecraft-servers

      # CORE

      # boot
      #../../../core/boot/bios-nix.nix
      ../../../core/boot/uefi-nix.nix
      ../../../core/boot/style-nix.nix
      # btop
      ../../../core/btop/enable-nix.nix
      # Deploy
      ../../../core/deploy/enable-nix.nix
      # Fast Fetch
      ../../../core/fastfetch/enable-nix.nix # Fastfetch  -  You got to show something in that cmd for your reddit posts
      # lazyUpdate - update on rebulid script - requres passwordless nixos-rebuild provided by sudo-nix.nix
      ../../../core/lazyUpdate/enable-nix.nix
      # PCmanFM
      ../../../desktop/pcmanfm/enable-nix.nix
      # Rofi
      ../../../core/rofi/enable-nix.nix
      # Style
      ../../../core/style/stylix-nix.nix # stylix  -  this repo expects stylix
      # Other
      ../../../core/other/cmd-nix.nix # CMD  -  Programs that are still needed. But don't need there own section
      ../../../core/home/home-nix.nix # home
      ../../../core/other/journald-nix.nix # journald  -  adds a fix that explicity limits how much storage logs are allow to take at 2GB
      ../../../core/other/local-nix.nix # Local settings
      ../../../core/other/nix-nix.nix # Nix settings  -  enable flakes, state nix version, etc
      ../../../core/other/networking-nix.nix
      ../../../core/other/programs-nix.nix # Install all programs in the programs var
      ../../../core/other/ssh-nix.nix # ssh  -  Enables ssh on port 22 needed for deploy rs
      ../../../core/other/sudo-nix.nix # sudo  -  Sudo settings
      ../../../core/other/user-nix.nix # user  -  Adds a user

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
      # vencord
      ../../../desktop/vencord/enable-nix.nix
      # obsidian
      #../../../desktop/obsidian/enable-nix.nix
      # SDDM
      ../../../desktop/sddm/enable-nix.nix
      # Sound
      ../../../desktop/sound/sound-nix.nix
      # steam
      #../../../desktop/steam/enable-nix.nix
      # Strawberry
      #../../../desktop/strawberry/enable-nix.nix # Music player
      # VScode
      ../../../desktop/vscode/enable-nix.nix
      # Other
      ../../../desktop/other/alvr-nix.nix # ALVR  -  For my vr nerds
      ../../../desktop/other/print-nix.nix # Print  -  How old are you?
      ../../../desktop/other/x11-nix.nix # x11  -  needed for Xwayland??

      # OTHER

      # LapisLazuli
      #../../../other/lapisLazuli/home-nix.nix # Lapis  -  Mount NAS to home
      #../../../other/lapisLazuli/smb-nix.nix # Lapis  -  My NAS! It's here cause I want it!
      #../../../other/lapisLazuli/nfs-nix.nix # Lapis  -  NAS

      # My stuff
      ./hardware-configuration.nix # hardware  -  your hardware settings
    ];
    homeImports = [
      # CORE

      # btop
      ../../../core/btop/style-home.nix
      # Fast Fetch
      ../../../core/fastfetch/settings-home.nix # Fastfetch  -  run on bash init
      # lazyUpdate - update on rebulid script - requres passwordless nixos-rebuild provided by sudo-nix.nix
      ../../../core/lazyUpdate/run-home.nix
      # Rofi
      ../../../core/rofi/style-home.nix # Styles
      # Other
      ../../../core/other/xdgMime-home.nix # Sets default apps

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
      # vencord
      ../../../desktop/vencord/settings-home.nix
      # obsidian
      ../../../desktop/obsidian/settings-home.nix
      # Sound
      ../../../desktop/sound/sound-home.nix
      # steam
      #../../../desktop/steam/startup-home.nix
      # Strawberry
      #../../../desktop/strawberry/bind-home.nix # Global Hotkeys for music player
      # VScode
      ../../../desktop/vscode/settings-home.nix
      ../../../desktop/vscode/language/cpp-home.nix
      ../../../desktop/vscode/language/javascript-home.nix # ESlint
      ../../../desktop/vscode/language/nix-home.nix # Nix LSP and FMT support

    ];

    user = "reese";

    network = "homelab";

    hostName = "neptunite";

    lapisLazuli = {
      mount = "/home/reese/lapis_lazuli";
      share = "lapis_lazuli";
      credentials = "/etc/nixos/secrets/diamond-user-access";
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
    primaryMonitor = "";
    monitor = [
      ""
    ];
  };
in
host
