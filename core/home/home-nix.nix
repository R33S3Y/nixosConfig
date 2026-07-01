{ system, ... }:
let
  users = builtins.mapAttrs (name: value: {
    imports = system.hosts.${system.host}.homeImports;

    programs.home-manager.enable = true;

    home = {
      stateVersion = "24.11";
      username = name;
      uid = value.uid;
      homeDirectory = "/home/${name}";
      backupFileExtension = "/home/${name}/.config/backup";
    };
  }) system.users;
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
