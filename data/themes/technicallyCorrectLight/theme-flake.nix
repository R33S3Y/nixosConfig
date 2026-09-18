{ inputs }:

let
  pkgs = import inputs.nixpkgs {
    system = "x86_64-linux"; # or inherit system if passed from the flake
    config.allowUnfree = true;
  };
  theme = {

    polarity = "dark";

    # wallpaper
    image = "${pkgs.nixos-artwork.wallpapers.nineish}/share/backgrounds/nixos/nix-wallpaper-nineish.png";

    base16Scheme = {
      base00 = "ebf5ff"; # #ebf5ff ----
      base01 = "cbe5ef"; # #cbe5ef ---
      base02 = "abd5df"; # #abd5df --
      base03 = "8bc5cf"; # #8bc5cf -
      base04 = "484ba8"; # #484ba8 +
      base05 = "383b88"; # #383b88 ++
      base06 = "282b58"; # #282b58 +++
      base07 = "181B28"; # #181B28 ++++
      base08 = "FF4444"; # #FF4444 red
      base09 = "FFAA22"; # #FFAA22 orange
      base0A = "DDDD22"; # #DDDD22 yellow
      base0B = "44FF44"; # #44FF44 green
      base0C = "22DDDD"; # #22DDDD aqua/cyan
      base0D = "2244FF"; # #2244FF blue
      base0E = "6622FF"; # #6622FF purple
      base0F = "663300"; # #663300 brown
    };

    fonts = {
      monospace = {
        package = pkgs.nerd-fonts.jetbrains-mono;
        name = "JetBrains Mono Nerd Font";
      };
      sansSerif = {
        package = pkgs.source-sans-pro;
        name = "Source Sans Pro";
      };
      serif = {
        package = pkgs.source-sans-pro;
        name = "Source Sans Pro";
      };
      emoji = {
        package = pkgs.noto-fonts-color-emoji;
        name = "Noto Color Emoji";
      };
      sizes = {
        applications = 13;
        desktop = 15;
        popups = 13;
        terminal = 11;
      };
    };

    borders = {
      thickness = 3;
      rounding = 20;
      padding = {
        inner = 6;
        outer = 12;
      };
    };

    opacity = {
      active = 0.95;
      inactive = 0.9;
    };

    # This sections follows the hyprland decoration.blur options.
    # See: https://wiki.hypr.land/configuring/core/config-options/#blur for options
    blur = {
      enabled = true;
      size = 10;
      passes = 4;
    };

    # This sections follows the hyprland decoration.shadow options.
    # See: https://wiki.hypr.land/configuring/core/config-options/#shadow for options
    shadow = {
      enabled = true;
      range = 50;
      render_power = 10;
    };

    icons = {
      package = pkgs.kdePackages.breeze-icons;
      name = "breeze"; # icon name is the folder name of your choice in /nix/store/{{icon Theme}}/share/icons
    };
  };
in
theme
