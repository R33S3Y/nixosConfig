{ system, ... }:
let
  users = builtins.mapAttrs (
    name: value:
    import ./home-home.nix
    // {
      home = {
        stateVersion = "24.11";
        username = name;
        homeDirectory = "/home/${name}";
      };
    }
  ) system.users;
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
