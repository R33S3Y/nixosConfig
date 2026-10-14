{ ... }:

let
  user = {
    # uid is and public Keys is used in core/user/user-nix.nix
    uid = 1000;
    publicKeys = [
      # optional
      "ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIPGspq2G8adszEonoETTJ9s8RWFCJfthqCqd5fjq+wXm reese@diamond"
    ];

    prettyName = "Reese";
    email = "r3es3y@gmail.com";
    profile = ./profile.png;

    screenshotFolder = "~/Pictures";
    theme = "diamond";

    # access options in core/users/access-nix.nix
    access = {
      canDeploy = true;
      canSsh = true;
      canSudo = true;
    };

    # locale options in core/users/locale-nix.nix
  };
in
user
