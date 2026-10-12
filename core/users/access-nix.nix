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
              Gives user access to sudo
            '';
            example = true;
          };
          canSsh = lib.mkOption {
            type = lib.types.bool;
            default = false;
            description = ''
              Gives user access to ssh
            '';
            example = true;
          };
          canDeploy = lib.mkOption {
            type = lib.types.bool;
            default = false;
            description = ''
              Gives user access to deploy
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
    access = value.access;
  }) system.users;
}
