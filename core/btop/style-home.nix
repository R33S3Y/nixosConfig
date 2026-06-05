{ config, pkgs, ... }:
{
  programs.btop = {
    enable = true;
    package = pkgs.btop-rocm;
    settings = {
      color_theme = "stylix";
      update_ms = 100;
      shown_boxes = "cpu mem net proc gpu1 gpu2";
      presets = "cpu:1:default,proc:0:default cpu:0:default,mem:0:default,net:0:default,gpu1:0:default,gpu2:0:default";
    };
  };

  home.file."share/btop/themes/stylix.theme".text = ''
    # Was stolen from https://github.com/catppuccin/btop/blob/main/themes/catppuccin_latte.theme
    # Main background, empty for terminal default, need to be empty if you want transparent background
    theme[main_bg]="#${config.stylix.base16Scheme.base00}"

    # Main text color
    theme[main_fg]="#${config.stylix.base16Scheme.base05}"

    # Title color for boxes
    theme[title]="#${config.stylix.base16Scheme.base05}"

    # Highlight color for keyboard shortcuts
    theme[hi_fg]="#${config.stylix.base16Scheme.base0A}"

    # Background color of selected item in processes box
    theme[selected_bg]="#${config.stylix.base16Scheme.base0B}"

    # Foreground color of selected item in processes box
    theme[selected_fg]="#${config.stylix.base16Scheme.base0A}"

    # Color of inactive/disabled text
    theme[inactive_fg]="#${config.stylix.base16Scheme.base01}"

    # Color of text appearing on top of graphs, i.e uptime and current network graph scaling
    theme[graph_text]="#${config.stylix.base16Scheme.base0D}"

    # Background color of the percentage meters
    theme[meter_bg]="#${config.stylix.base16Scheme.base0B}"

    # Misc colors for processes box including mini cpu graphs, details memory graph and details status text
    theme[proc_misc]="#${config.stylix.base16Scheme.base0D}"

    # CPU, Memory, Network, Proc box outline colors
    theme[cpu_box]="#${config.stylix.base16Scheme.base08}" #Mauve
    theme[mem_box]="#${config.stylix.base16Scheme.base0A}" #Green
    theme[net_box]="#${config.stylix.base16Scheme.base0C}" #Maroon
    theme[proc_box]="#${config.stylix.base16Scheme.base0E}" #Blue

    # Box divider line and small boxes line color
    theme[div_line]="#${config.stylix.base16Scheme.base0F}"

    # Temperature graph color (Green -> Yellow -> Red)
    theme[temp_start]="#${config.stylix.base16Scheme.base08}"
    theme[temp_mid]="#${config.stylix.base16Scheme.base0F}"
    theme[temp_end]="#${config.stylix.base16Scheme.base0F}"

    # CPU graph colors (Teal -> Lavender)
    theme[cpu_start]="#${config.stylix.base16Scheme.base08}"
    theme[cpu_mid]="#${config.stylix.base16Scheme.base0C}"
    theme[cpu_end]="#${config.stylix.base16Scheme.base0F}"

    # Mem/Disk free meter (Mauve -> Lavender -> Blue)
    theme[free_start]="#${config.stylix.base16Scheme.base08}"
    theme[free_mid]="#${config.stylix.base16Scheme.base0C}"
    theme[free_end]="#${config.stylix.base16Scheme.base0F}"

    # Mem/Disk cached meter (Sapphire -> Lavender)
    theme[cached_start]="#${config.stylix.base16Scheme.base08}"
    theme[cached_mid]="#${config.stylix.base16Scheme.base0C}"
    theme[cached_end]="#${config.stylix.base16Scheme.base0F}"

    # Mem/Disk available meter (Peach -> Red)
    theme[available_start]="#${config.stylix.base16Scheme.base08}"
    theme[available_mid]="#${config.stylix.base16Scheme.base0C}"
    theme[available_end]="#${config.stylix.base16Scheme.base0F}"

    # Mem/Disk used meter (Green -> Sky)
    theme[used_start]="#${config.stylix.base16Scheme.base08}"
    theme[used_mid]="#${config.stylix.base16Scheme.base0C}"
    theme[used_end]="#${config.stylix.base16Scheme.base0F}"

    # Download graph colors (Peach -> Red)
    theme[download_start]="#${config.stylix.base16Scheme.base08}"
    theme[download_mid]="#${config.stylix.base16Scheme.base0C}"
    theme[download_end]="#${config.stylix.base16Scheme.base0F}"

    # Upload graph colors (Green -> Sky)
    theme[upload_start]="#${config.stylix.base16Scheme.base08}"
    theme[upload_mid]="#${config.stylix.base16Scheme.base0C}"
    theme[upload_end]="#${config.stylix.base16Scheme.base0F}"

    # Process box color gradient for threads, mem and cpu usage (Sapphire -> Mauve)
    theme[process_start]="#${config.stylix.base16Scheme.base08}"
    theme[process_mid]="#${config.stylix.base16Scheme.base0C}"
    theme[process_end]="#${config.stylix.base16Scheme.base0F}"
  '';
}
