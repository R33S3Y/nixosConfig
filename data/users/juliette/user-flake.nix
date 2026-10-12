{ ... }:

let
  user = {
    uid = 1004;
    prettyName = "Juliette";

    profile = ./profile.png;
    theme = "amethyst";

    email = "";

    screenshotFolder = "~/Pictures";

    # access options in core/users/access-nix.nix

    # locale options in core/users/locale-nix.nix
  };
in
user
