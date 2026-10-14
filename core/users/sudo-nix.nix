{
  lib,
  config,
  ...
}:
let
  sudoRules = lib.mapAttrsToList (name: value: {
    users = [ name ];
    commands = [ "ALL" ];
  }) (lib.filterAttrs (name: value: value.access.canSudo) config.services.internal.users);
  deployRules = lib.mapAttrsToList (name: value: {
    users = [ name ];
    commands = [
      {
        command = "${pkgs.internal.deployClient}/bin/deployClient";
        options = [ "NOPASSWD" ];
      }
    ];
  }) (lib.filterAttrs (name: value: value.access.canDeploy) config.services.internal.users);
in
{
  security.sudo = {
    extraRules =
      sudoRules
      ++ deployRules
      ++ [
        {
          users = [ "rebuild" ];
          commands = [
            {
              command = "/run/current-system/sw/bin/nixos-rebuild";
              options = [ "NOPASSWD" ];
            }
            {
              command = "/run/current-system/sw/bin/nix-env --profile /nix/var/nix/profiles/system --delete-generations +25";
              options = [ "NOPASSWD" ];
            }
            {
              command = "/run/current-system/sw/bin/nix-collect-garbage";
              options = [ "NOPASSWD" ];
            }
          ];
        }
      ];
  };
}
