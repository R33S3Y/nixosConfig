{ ... }: {
  imports = [
    # CORE

    # btop
    btop/enable-nix.nix
    # Deploy
    deploy/server-nix.nix
    deploy/client-nix.nix
    # Fast Fetch
    fastfetch/enable-nix.nix # Fastfetch  -  You got to show something in that cmd for your reddit posts
    # man
    man/enable-nix.nix
    # Style
    style/stylix-nix.nix # stylix  -  this repo expects stylix

    # Users - implements and sets most user settings in system.users.<name>
    users/access-nix.nix
    users/locale-nix.nix
    users/ssh-nix.nix
    users/sudo-nix.nix
    users/user-nix.nix
    # Other
    other/cmd-nix.nix # CMD  -  Programs that are still needed. But don't need there own section
    other/home-nix.nix # home
    other/journald-nix.nix # journald  -  adds a fix that explicity limits how much storage logs are allow to take at 2GB
    other/nix-nix.nix # Nix settings  -  enable flakes, state nix version, etc
    other/networking-nix.nix
    other/programs-nix.nix # Install all programs in the programs var
    other/stable-nix.nix
    other/update-nix.nix
  ];
}
