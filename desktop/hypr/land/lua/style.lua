-- ${NIX.type.thing} is replaced by nix, when it reads the file.
hl.config({
  general = {
    gaps_in = "${NIX.theme.borders.padding.inner}",
    gaps_out = "${NIX.theme.borders.padding.outer}",
    border_size = "${NIX.theme.borders.thickness}",

    layout = "dwindle",

    -- Please see https://wiki.hyprland.org/Configuring/Tearing/ before you turn this on
    -- allow_tearing = false
  },
  decoration = {
    -- See https://wiki.hyprland.org/Configuring/Variables/ for more

    rounding = "${NIX.theme.borders.rounding}",

    blur = "${NIX.theme.blur}",

    shadow = "${NIX.theme.shadow}",

    active_opacity = "${NIX.theme.opacity.active}",
    inactive_opacity = "${NIX.theme.opacity.inactive}"
  },
})
