{ system, lib, ... }:
let
  #allowUsers = lib.mapAttrsToList (name: value: name) (lib.filterAttrs (
  #  name: value: value.access.canSsh == true
  #) system.users);
in
{
  services.openssh = {
    enable = true;
    ports = [ 22 ];
    settings = {
      PasswordAuthentication = true;
      AllowUsers = [
        # allowUsers ++ [
        system.user
        "rebuild"
      ];
      PermitRootLogin = "no";
      X11Forwarding = false;
    };
  };
}
