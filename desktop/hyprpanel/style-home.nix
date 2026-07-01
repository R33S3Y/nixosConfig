# From: https://github.com/anotherhadi/nixy/tree/main
# Hyprpanel is the bar on top of the screen
# Display informations like workspaces, battery, wifi, ...
{
  config,
  system,
  ...
}:
let
  theme = system.themes.${system.theme};

  transparentButtons = true;

  accent = "#${config.lib.stylix.colors.base0D}";
  accentAlt = "#${config.lib.stylix.colors.base08}";
  background = "#${config.lib.stylix.colors.base00}";
  backgroundAlt = "#${config.lib.stylix.colors.base01}";
  foreground = "#${config.lib.stylix.colors.base05}";
  foregroundAlt = "#${config.lib.stylix.colors.base06}";

  rounding = theme.borders.rounding;
  borderSize = theme.borders.thickness;

  gapsOut = theme.borders.padding.outer;
  gapsIn = theme.borders.padding.inner;

  transparent = true;

  notificationOpacity = 90;

  location = "";
in
{

  stylix.targets.hyprpanel.enable = false;

  programs.hyprpanel = {
    enable = true;

    settings = {

      bar.layouts = {
        "${system.hosts.${system.host}.primaryMonitor}" = {
          "left" = [
            "dashboard"
            "workspaces"
            "windowtitle"
          ];
          "middle" = [
            "media"
            "cava"
          ];
          "right" = [
            "systray"
            "volume"
            "bluetooth"
            "network"
            "clock"
            "notifications"
          ];
        };
        "*" = {
          "left" = [
            "dashboard"
            "workspaces"
            "windowtitle"
          ];
          "right" = [
            "volume"
            "clock"
            "notifications"
          ];
        };
      };

      #Main bar
      theme.font.name = config.stylix.fonts.serif.name;
      theme.font.size = "${toString config.stylix.fonts.sizes.desktop}px";

      theme.bar.floating = true; # allows gaps around the bar
      theme.bar.outer_spacing = "0"; # use margin instead
      theme.bar.margin_top = toString (gapsOut - borderSize) + "px"; # only works when floating = true
      theme.bar.margin_bottom = toString (gapsIn - borderSize) + "px"; # only works when floating = true
      theme.bar.margin_sides = toString (gapsOut - borderSize) + "px"; # only works when floating = true
      theme.bar.border_radius = toString rounding + "px";

      theme.bar.dropdownGap = toString ((gapsIn - borderSize) * 2) + "px"; # how far down dropdowns are (is from absolte top)

      theme.bar.transparent = transparent;
      theme.bar.location = "top";
      theme.bar.background = background;

      theme.bar.buttons.hover = background;
      theme.bar.buttons.y_margins = "0px";
      theme.bar.buttons.spacing = "0.3em";
      theme.bar.buttons.radius =
        (if transparent then toString rounding else toString (rounding - 8)) + "px";
      theme.bar.buttons.padding_x = "0.8rem";
      theme.bar.buttons.padding_y = "0.4rem";
      theme.bar.buttons.style = "default";
      theme.bar.buttons.monochrome = true;
      theme.bar.buttons.text = if transparent && transparentButtons then foregroundAlt else foreground;
      theme.bar.buttons.icon = accent;

      theme.bar.menus.monochrome = true;
      theme.bar.menus.card_radius = toString rounding + "px";
      theme.bar.menus.border.size = toString borderSize + "px";
      theme.bar.menus.border.radius = toString rounding + "px";
      theme.bar.menus.menu.media.card.tint = 90;
      theme.bar.menus.background = background;
      theme.bar.menus.cards = backgroundAlt;
      theme.bar.menus.label = foreground;
      theme.bar.menus.text = foreground;
      theme.bar.menus.border.color = accent;
      theme.bar.menus.popover.text = foreground;
      theme.bar.menus.popover.background = backgroundAlt;
      theme.bar.menus.listitems.active = accent;
      theme.bar.menus.icons.active = accent;
      theme.bar.menus.switch.enabled = accent;
      theme.bar.menus.check_radio_button.active = accent;
      theme.bar.menus.buttons.default = accent;
      theme.bar.menus.buttons.active = accent;
      theme.bar.menus.iconbuttons.active = accent;
      theme.bar.menus.progressbar.foreground = accent;
      theme.bar.menus.slider.primary = accent;
      theme.bar.menus.tooltip.background = backgroundAlt;
      theme.bar.menus.tooltip.text = foreground;
      theme.bar.menus.dropdownmenu.background = backgroundAlt;
      theme.bar.menus.dropdownmenu.text = foreground;

      # Start menu
      bar.launcher.icon = "";

      menus.dashboard.powermenu.confirmation = false;
      menus.dashboard.powermenu.avatar.image = "${system.users.${system.user}.profile}";

      menus.dashboard.shortcuts.left.shortcut1.icon = "";
      menus.dashboard.shortcuts.left.shortcut1.command = "zen";
      menus.dashboard.shortcuts.left.shortcut1.tooltip = "Zen";
      menus.dashboard.shortcuts.left.shortcut2.icon = "󰅶";
      menus.dashboard.shortcuts.left.shortcut2.command = "caffeine";
      menus.dashboard.shortcuts.left.shortcut2.tooltip = "Caffeine";
      menus.dashboard.shortcuts.left.shortcut3.icon = "󰖔";
      menus.dashboard.shortcuts.left.shortcut3.command = "night-shift";
      menus.dashboard.shortcuts.left.shortcut3.tooltip = "Night-shift";
      menus.dashboard.shortcuts.left.shortcut4.icon = "";
      menus.dashboard.shortcuts.left.shortcut4.command = "menu";
      menus.dashboard.shortcuts.left.shortcut4.tooltip = "Search Apps";

      menus.dashboard.shortcuts.right.shortcut1.icon = "";
      menus.dashboard.shortcuts.right.shortcut1.command = "hyprpicker -a";
      menus.dashboard.shortcuts.right.shortcut1.tooltip = "Color Picker";
      menus.dashboard.shortcuts.right.shortcut3.icon = "󰄀";
      menus.dashboard.shortcuts.right.shortcut3.command = "screenshot region swappy";
      menus.dashboard.shortcuts.right.shortcut3.tooltip = "Screenshot";

      # Workspace Module
      bar.workspaces.show_numbered = false;
      bar.workspaces.workspaces = 5;
      bar.workspaces.numbered_active_indicator = "color";
      bar.workspaces.monitorSpecific = true;
      bar.workspaces.applicationIconEmptyWorkspace = "";
      bar.workspaces.showApplicationIcons = true;
      bar.workspaces.showWsIcons = true;

      theme.bar.buttons.workspaces.hover = accentAlt;
      theme.bar.buttons.workspaces.active = accent;
      theme.bar.buttons.workspaces.available = accentAlt;
      theme.bar.buttons.workspaces.occupied = accentAlt;

      # Windowtitle module
      bar.windowtitle.label = true;
      theme.bar.windowtitle.icon.color = accent;

      # Media
      bar.media.show_active_only = true;
      theme.bar.menus.menu.media.background.color = backgroundAlt;
      theme.bar.menus.menu.media.card.color = backgroundAlt;

      # Cava
      bar.customModules.updates.pollingInterval = 1440000;
      bar.customModules.cava.showIcon = false;
      bar.customModules.cava.stereo = true;
      bar.customModules.cava.showActiveOnly = true;

      # systray

      # volume
      bar.volume.label = false;

      # bluetooth
      bar.bluetooth.label = false;

      # network
      bar.network.truncation_size = 12;

      # clock and weather
      bar.clock.format = "%a %b %d  %I:%M %p";
      menus.clock.weather.location = location;
      menus.clock.weather.unit = "metric";

      # notifications
      bar.notifications.show_total = true;
      notifications.position = "top right";
      notifications.showActionsOnHover = true;
      theme.notification.opacity = notificationOpacity;
      theme.notification.enableShadow = true;
      theme.notification.border_radius = toString rounding + "px";
      menus.power.lowBatteryNotification = true;
      theme.bar.buttons.notifications.background = backgroundAlt;
      theme.bar.buttons.notifications.hover = background;
      theme.bar.buttons.notifications.total = accent;
      theme.bar.buttons.notifications.icon = accent;
      theme.notification.background = background;
      theme.notification.actions.background = accent;
      theme.notification.actions.text = foreground;
      theme.notification.label = accent;
      theme.notification.border = backgroundAlt;
      theme.notification.text = foreground;
      theme.notification.labelicon = accent;
      theme.notification.close_button.background = backgroundAlt;
      theme.notification.close_button.label = accent;

      # osd
      theme.osd.enable = true;
      theme.osd.orientation = "vertical";
      theme.osd.location = "left";
      theme.osd.radius = toString rounding + "px";
      theme.osd.margins = "0px 0px 0px 10px";
      theme.osd.muted_zero = true;
      theme.osd.bar_color = accent;
      theme.osd.bar_overflow_color = accentAlt;
      theme.osd.icon = background;
      theme.osd.icon_container = accent;
      theme.osd.label = accent;
      theme.osd.bar_container = backgroundAlt;

      # other
      wallpaper.enable = false;
    };
  };

  wayland.windowManager.hyprland.settings.exec-once = [
    "hyprpanel"
  ];
}
