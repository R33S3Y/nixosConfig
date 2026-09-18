{ ... }: {
  imports = [
    # CORE

    # btop
    btop/style-home.nix
    # Fast Fetch
    fastfetch/settings-home.nix # Fastfetch  -  run on bash init
    # man
    man/settings-home.nix
    # Other
    other/xdgMime-home.nix # Sets default apps
  ];
}
