{ inputs }:

let
  pkgs = import inputs.nixpkgs {
    system = "x86_64-linux"; # or inherit system if passed from the flake
    config.allowUnfree = true;
  };
  theme = {

    polarity = "dark";

    # wallpaper
    image = ./wallpaper.jpg;

    base16Scheme = {
      base00 = "181B28"; # ----
      base01 = "282b38"; # ---
      base02 = "383b48"; # --
      base03 = "484b58"; # -
      base04 = "c58bcf"; # +
      base05 = "d5abdf"; # ++
      base06 = "e5cbef"; # +++
      base07 = "f5ebff"; # ++++
      base08 = "4075dc"; # red
      base09 = "5075dc"; # orange
      base0A = "6075dc"; # yellow
      base0B = "7075dc"; # green
      base0C = "8075dc"; # aqua/cyan
      base0D = "9075dc"; # blue
      base0E = "a075dc"; # purple
      base0F = "b075dc"; # brown
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
  };
in
theme

#base00 = "#181B28"; # ----
#base01 = "#282b38"; # ---
#base02 = "#383b48"; # --
#base03 = "#484b58"; # -
#base04 = "#c58bcf"; # +
#base05 = "#d5abdf"; # ++
#base06 = "#e5cbef"; # +++
#base07 = "#f5ebff"; # ++++
#base08 = "#4075dc"; # red
#base09 = "#5075dc"; # orange
#base0A = "#6075dc"; # yellow
#base0B = "#7075dc"; # green
#base0C = "#8075dc"; # aqua/cyan
#base0D = "#9075dc"; # blue
#base0E = "#a075dc"; # purple
#base0F = "#b075dc"; # brown
