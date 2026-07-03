{ inputs }:

let
  pkgs = import inputs.nixpkgs {
    system = "x86_64-linux"; # or inherit system if passed from the flake
    config.allowUnfree = true;
  };
  theme = {

    polarity = "light";

    # wallpaper
    image = ./wallpaper.jpg;

    base16Scheme = {
      base00 = "ebf5ff"; # #ebf5ff ----
      base01 = "cbe5ef"; # #cbe5ef ---
      base02 = "abd5df"; # #abd5df --
      base03 = "8bc5cf"; # #8bc5cf -
      base04 = "484b58"; # #484b58 +
      base05 = "383b48"; # #383b48 ++
      base06 = "282b38"; # #282b38 +++
      base07 = "181B28"; # #181B28 ++++
      base08 = "4075dc"; # #4075dc red
      base09 = "5075dc"; # #5075dc orange
      base0A = "6075dc"; # #6075dc yellow
      base0B = "7075dc"; # #7075dc green
      base0C = "8075dc"; # #8075dc aqua/cyan
      base0D = "9075dc"; # #9075dc blue
      base0E = "a075dc"; # #a075dc purple
      base0F = "b075dc"; # #b075dc brown
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

    blur = {
      enabled = "true";
      size = 10;
      passes = 4;
    };

    shadow = {
      enabled = "true";
      range = 50;
      render_power = 10;
    };

    opacity = {
      active = 0.95;
      inactive = 0.9;
    };

    icons = {
      package = pkgs.kdePackages.breeze-icons;
      name = "breeze"; # icon name is the folder name of your choice in /nix/store/{{icon Theme}}/share/icons
    };
  };
in
theme
