{ system, lib, ... }:
let
  rules = builtins.mapAttrs (name: value: {
    users = [ name ];
    commands = [ "ALL" ];
  }) lib.filterAttrs (name: value: value.access.canSudo == true) system.users;
in
{
  security.sudo = rules // {
    extraRules = [
      {
        groups = [ "wheel" ];
        commands = [
          {
            command = "/run/current-system/sw/bin/nixos-rebuild";
            options = [ "NOPASSWD" ];
          }
        ];
      }
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
