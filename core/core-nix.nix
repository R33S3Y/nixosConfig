{ ... }: {
  imports = [
    # CORE

    # btop
    btop/enable-nix.nix
    # Deploy
    deploy/enable-nix.nix
    # Fast Fetch
    fastfetch/enable-nix.nix # Fastfetch  -  You got to show something in that cmd for your reddit posts
    # man
    man/enable-nix.nix
    # Style
    style/stylix-nix.nix # stylix  -  this repo expects stylix
    # Other
    other/cmd-nix.nix # CMD  -  Programs that are still needed. But don't need there own section
    other/home-nix.nix # home
    other/journald-nix.nix # journald  -  adds a fix that explicity limits how much storage logs are allow to take at 2GB
    other/local-nix.nix # Local settings
    other/nix-nix.nix # Nix settings  -  enable flakes, state nix version, etc
    other/networking-nix.nix
    other/programs-nix.nix # Install all programs in the programs var
    other/ssh-nix.nix # ssh  -  Enables ssh on port 22 needed for deploy rs
    other/sudo-nix.nix # sudo  -  Sudo settings
    other/user-nix.nix # user  -  Adds a user
  ];
}
