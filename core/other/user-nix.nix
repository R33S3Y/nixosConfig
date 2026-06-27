{ system, ... }:
let
  users = builtins.mapAttrs (name: value: {
    isNormalUser = true;
    uid = value.uid;
    description = name;
    extraGroups = [
      "networkmanager"
      "wheel"
      "jellyfin"
      "cms"
      "render"
    ];
  }) system.users;
in
{
  users.users = users // {
    rebuild = {
      isNormalUser = true;
      uid = 1001;
      group = "rebuild";
      description = "Remote rebuild user";
      openssh.authorizedKeys.keys = [
        "ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIPGspq2G8adszEonoETTJ9s8RWFCJfthqCqd5fjq+wXm reese@diamond"
      ];
    };
    jellyfin = {
      isSystemUser = true;
      uid = 1002;
      group = "jellyfin";
      description = "jellyfin";
    };
    cms = {
      isSystemUser = true;
      uid = 1003;
      group = "cms";
      description = "cms";
    };
  };
  users.groups = {
    rebuild = {
      gid = 1001;
    };
    jellyfin = {
      gid = 1002;
    };
    cms = {
      gid = 1003;
    };
  };
}
