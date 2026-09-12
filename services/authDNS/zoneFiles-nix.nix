{
  lib,
  pkgs,
  config,
  ...
}:

# this file takes all the info in services.internal.authDNS.domains and converts it into a zone files and then pkgs it. For knot DNS to pickup and use.

{
  nixpkgs.overlays = [
    (final: prev: {
      internal =
        (prev.internal or { })
        // lib.concatMapAttrs (name: info: {
          "${name}.zone" = pkgs.stdenv.mkDerivation {
            name = "${name}.zone";
            pname = "${name}.zone";
            version = "1.0";

            src = null;
            dontUnpack = true;

            buildPhase = ''
              echo "
              \$ORIGIN ${name}.
              \$TTL ${toString info.ttl}

              @ IN SOA ${info.primaryNameserver.name}.${name}. ${lib.strings.replaceString "@" "." info.email}. (
                1                           ; serial (version number) - this will only matter if we pointed a secondary server towards this.
                ${toString info.ttl}        ; refresh (how often should the secondary server contact main)
                ${toString (info.ttl / 2)}  ; retry (how long should the secondary server wait before try to contact the primary server in the case of failure)
                ${toString (info.ttl * 16)} ; expire (If the secondary fails to connect to the primary for this amount of time. It should stop serving requests.)
                ${toString info.ttl}        ; minimum
              )

              @   IN NS ${info.primaryNameserver.name}.${name}.
              ${info.primaryNameserver.name} IN A ${info.primaryNameserver.address}

              ${lib.concatStrings (
                lib.map (record: "${record.name} IN ${record.type} ${record.value} \n") info.records
              )}
              " >> ${name}.zone
            '';

            installPhase = ''
              mkdir -p $out
              cp ${name}.zone $out
            '';
          };
        }) config.services.internal.authDNS.domains;
    })
  ];
  #install pkgs
  environment.systemPackages = lib.mapAttrsToList (
    name: info: pkgs.internal."${name}.zone"
  ) config.services.internal.authDNS.domains;
}
