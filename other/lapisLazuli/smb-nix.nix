{ pkgs, system, ... }:

{
  environment.systemPackages = with pkgs; [
    cifs-utils # For SMB/CIFS
  ];

  fileSystems."${system.hosts.${system.host}.lapisLazuli.mount}" = {
    device = "//192.168.1.253/${system.hosts.${system.host}.lapisLazuli.share}";
    fsType = "cifs";
    options = [
      "credentials=${system.hosts.${system.host}.lapisLazuli.credentials}"
      "uid=${system.user}"
      "gid=users"
      "iocharset=utf8"
    ];
  };
}
