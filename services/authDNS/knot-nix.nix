{
  lib,
  config,
  pkgs,
  ...
}:
{
  networking.firewall = {
    allowedTCPPorts = [ 53 ];
    allowedUDPPorts = [ 53 ];
  };
  services.knot = {
    enable = true;
    checkConfig = true;

    settings = {

      server = {
        listen = [
          "0.0.0.0@53"
          "::@53"
        ];
      };
      policy = [
        {
          id = "DNSSEC";
          signing-threads = 4;
          algorithm = "ECDSAP256SHA256";
          zsk-lifetime = "180d";
        }
      ];
      zone = lib.mapAttrsToList (
        name: info:
        let
          zoneName = "${name}.zone";
        in
        {
          domain = name;
          storage = "${pkgs.internal."${zoneName}"}/";
          file = zoneName;
          dnssec-signing = "on";
          dnssec-policy = "DNSSEC";
        }
      ) config.services.internal.authDNS.domains;

      log = [
        {
          target = "syslog";
          any = "info";
        }
      ];
    };
  };
}
