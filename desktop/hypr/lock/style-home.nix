{ config, system, ... }:
{
  stylix.targets.hyprlock.enable = false;
  programs.hyprlock = {
    enable = true;
    settings = {
      general = {
        grace = 5;
        no_fade_in = false;
        disable_loading_bar = false;
      };

      # BACKGROUND
      background = {
        monitor = "";
        path = "${config.stylix.image}";
      };

      image = [
        # profile pic
        {
          monitor = system.hosts.${system.host}.primaryMonitor;
          size = "100, 100";
          position = "0, 350";
          align = "center";
          valign = "center";

          path = "${system.users.${system.user}.profile}";
          rounding = -1;
          border_size = system.themes.${system.theme}.borders.thickness;
          border_color = "rgba(${config.stylix.base16Scheme.base0D}ff)";
        }
      ];

      shape = [
        # window box
        {
          monitor = system.hosts.${system.host}.primaryMonitor;
          size = "500, 700";
          position = "0, 0";
          halign = "center";
          valign = "center";

          color = "rgba(${config.stylix.base16Scheme.base00}B0)";
          rounding = system.themes.${system.theme}.borders.rounding;
          border_size = system.themes.${system.theme}.borders.thickness;
          border_color = "rgba(${config.stylix.base16Scheme.base0D}ff)";
        }
      ];

      label = [
        # Time
        {
          monitor = system.hosts.${system.host}.primaryMonitor;
          text = ''cmd[update:1000] echo "<span>$(date +"%I:%M")</span>"'';
          color = "rgba(${config.stylix.base16Scheme.base06}ff)";
          font_size = 100;
          position = "0, 150";
          halign = "center";
          valign = "center";
        }
        # Date
        {
          monitor = system.hosts.${system.host}.primaryMonitor;
          text = ''cmd[update:1000] echo -e "$(date +"%A, %B %d")"'';
          color = "rgba(${config.stylix.base16Scheme.base04}ff)";
          font_size = 30;
          position = "0, 50";
          halign = "center";
          valign = "center";
        }
        # USER
        {
          monitor = system.hosts.${system.host}.primaryMonitor;
          text = system.users.${system.user}.prettyName;
          color = "rgba(${config.stylix.base16Scheme.base04}ff)";
          font_size = 25;
          position = "0, -150";
          halign = "center";
          valign = "center";
        }
      ];

      # INPUT FIELD
      input-field = {
        monitor = system.hosts.${system.host}.primaryMonitor;
        size = "325, 60";
        outline_thickness = system.themes.${system.theme}.borders.thickness;
        dots_size = 0.2; # Scale of input-field height, 0.2 - 0.8
        dots_spacing = 0.2; # Scale of dots' absolute size, 0.0 - 1.0
        dots_center = true;
        outer_color = "rgba(${config.stylix.base16Scheme.base0D}ff)";
        check_color = "rgba(${config.stylix.base16Scheme.base08}ff)";
        fail_color = "rgba(${config.stylix.base16Scheme.base0D}ff)";
        inner_color = "rgba(${config.stylix.base16Scheme.base01}B0)";
        font_color = "rgba(${config.stylix.base16Scheme.base04}ff)";
        fade_on_empty = false;
        placeholder_text = "<i>Enter Password</i>";
        hide_input = false;
        position = "0, -250";
        halign = "center";
        valign = "center";
      };
    };
  };
}
