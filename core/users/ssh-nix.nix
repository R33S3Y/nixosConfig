{ config, lib, ... }:
let
  allowUsers = lib.mapAttrsToList (name: value: name) (
    lib.filterAttrs (name: value: value.access.canSsh == true) config.services.internal.users
  );
in
{
  services.openssh = {
    enable = true;
    ports = [ 22 ];
    settings = {
      PasswordAuthentication = true;
      AllowUsers = allowUsers ++ [
        "rebuild"
      ];
      PermitRootLogin = "no";
      X11Forwarding = false;
    };
  };
}
