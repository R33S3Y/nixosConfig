{ lib, system, ... }:

let
  userOpts =
    { ... }:
    {
      options = {
        access = {
          canSudo = lib.mkOption {
            type = lib.types.bool;
            default = false;
            description = ''
              Gives user access to sudo. Implemented in ./sudo-nix.nix
            '';
            example = true;
          };
          canSsh = lib.mkOption {
            type = lib.types.bool;
            default = false;
            description = ''
              Gives user access to ssh. Implemented in ./ssh-nix.nix
            '';
            example = true;
          };
          canDeploy = lib.mkOption {
            type = lib.types.bool;
            default = false;
            description = ''
              Gives user access to deploy. Implemented in ./sudo-nix.nix
            '';
            example = true;
          };
        };
      };
    };
in
{
  options.services.internal.users = lib.mkOption {
    type = lib.types.attrsOf (lib.types.submodule userOpts);
    default = { };
    description = ''
      A attrset for all the users and their options
    '';
    example = {
      "example.user" = {
        access = {
          canSudo = false;
          canSsh = true;
          canDeploy = false;
        };
      };
    };
  };

  config.services.internal.users = lib.mapAttrs (name: value: {
    access = value.access or { };
  }) system.users;
}
