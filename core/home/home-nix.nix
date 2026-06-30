{ system, ... }:
let
  users = builtins.mapAttrs (name: value: import ./home-home.nix) system.users;
in
{

  home-manager = {
    useGlobalPkgs = true;
    useUserPackages = true;
    backupFileExtension = "backup";

    extraSpecialArgs = {
      system = system;
    };

    users = users;
  };
}
