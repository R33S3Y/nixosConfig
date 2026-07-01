{ system, ... }:
{
  stylix = {
    enable = true;

    polarity = system.themes.${system.theme}.polarity;

    image = system.themes.${system.theme}.image;

    base16Scheme = system.themes.${system.theme}.base16Scheme;

    fonts = system.themes.${system.theme}.fonts;

    icons = {
      enable = true;
      package = system.themes.${system.theme}.icons.package;
      light = system.themes.${system.theme}.icons.name;
      dark = system.themes.${system.theme}.icons.name;
    };
  };
}
