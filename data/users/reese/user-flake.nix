{ ... }:

let
  user = {
    uid = 1000;
    prettyName = "Reese";

    profile = ./profile.png;
    theme = "diamond";

    email = "r3es3y@gmail.com";

    screenshotFolder = "~/Pictures";

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
