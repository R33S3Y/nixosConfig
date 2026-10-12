{
  pkgs,
  lib,
  system,
  ...
}:
let
  addNixHosts =
    # this function takes the system.hosts.<host>.hostName and <host>.static.ipv4.address and converts it into:
    # [
    #   "ipv4.address hostName"
    #   "ipv4.address hostName.lan"
    # ]
    # for all hosts that have that info and are in system
    system:
    lib.concatLists (
      lib.mapAttrsToList (
        _: value:
        if lib.hasAttrByPath [ "static" "ipv4" "address" ] value && value ? hostName then
          [
            "${value.static.ipv4.address} ${value.hostName}"
            "${value.static.ipv4.address} ${value.hostName}.lan"
          ]
        else
          [ ]
      ) system.hosts
    );

in
{
  services.pihole-web = {
    enable = true;
    ports = [
      "80r"
      "443s"
    ];
  };
  services.pihole-ftl = {
    enable = true;

    package = pkgs.pihole-ftl;

    openFirewallDHCP = true;
    openFirewallDNS = true;
    openFirewallWebserver = true;

    # Blocklists
    lists = [
      {
        url = "https://easylist.to/easylist/easylist.txt";
        type = "block";
        enabled = true;
        description = "EasyList";
      }
      {
        url = "https://easylist.to/easylist/easyprivacy.txt";
        type = "block";
        enabled = true;
        description = "EasyPrivacy";
      }
      {
        url = "https://raw.githubusercontent.com/StevenBlack/hosts/master/hosts";
        type = "block";
        enabled = true;
        description = "Steven Black Hosts";
      }
    ];

    # Optional settings
    privacyLevel = 0; # 0 = show everything, increase for more privacy
    queryLogDeleter = {
      enable = true;
      interval = "weekly";
      age = 7; # keep logs max 7 days
    };

    settings = {
      dns = {
        upstreams = [
          "8.8.8.8"
          "8.8.4.4"
        ];
        CNAMEdeepInspect = true;
        blockESNI = true;
        EDNS0ECS = true;
        ignoreLocalhost = false;
        showDNSSEC = true;
        analyzeOnlyAandAAAA = false;
        piholePTR = "PI.HOLE";
        replyWhenBusy = "ALLOW";
        blockTTL = 2;
        hosts = [
          "192.168.1.253 lapisLazuli"
          "192.168.1.253 lapisLazuli.lan"
          "192.168.1.254 jade"
          "192.168.1.254 jade.lan"
        ]
        ++ addNixHosts system;
        domainNeeded = false;
        expandHosts = false;
        domain = "lan";
        bogusPriv = true;
        dnssec = false;
        interface = "";
        hostRecord = "";
        listeningMode = "LOCAL";
        queryLogging = true;
        cnameRecords = [ ];
        port = 53;
        revServers = [ ];

        cache = {
          size = 10000;
          optimizer = 3600;
          upstreamBlockedTTL = 86400;
        };

        blocking = {
          active = true;
          mode = "NULL";
          edns = "TEXT";
        };

        specialDomains = {
          mozillaCanary = true;
          iCloudPrivateRelay = true;
          designatedResolver = true;

          reply = {
            host = {
              force4 = false;
              IPv4 = "";
              force6 = false;
              IPv6 = "";
            };
            blocking = {
              force4 = false;
              IPv4 = "";
              force6 = false;
              IPv6 = "";
            };
          };
        };

        rateLimit = {
          count = 1000;
          interval = 60;
        };
      };

      dhcp = {
        active = true;
        start = "192.168.1.2";
        end = "192.168.1.150";
        router = "192.168.1.1";
        netmask = "";
        leaseTime = "";
        ipv6 = false;
        rapidCommit = false;
        multiDNS = false;
        logging = true;
        ignoreUnknownClients = false;
        hosts = [ ];
      };

      ntp = {
        ipv4 = {
          active = true;
          address = "";
        };
        ipv6 = {
          active = true;
          address = "";
        };
        sync = {
          active = true;
          server = "nz.pool.ntp.org";
          interval = 3600;
          count = 8;
          rtc = {
            set = false;
            device = "";
            utc = true;
          };
        };
      };

      resolver = {
        resolveIPv4 = true;
        resolveIPv6 = true;
        networkNames = true;
        refreshNames = "IPV4_ONLY";
      };

      misc = {
        delay_startup = 0;
        nice = -10;
        addr2line = true;
        etc_dnsmasq_d = false;
        dnsmasq_lines = [ ];
        extraLogging = true;
        readOnly = true;

        check = {
          load = true;
          shmem = 90;
          disk = 90;
        };
      };

      debug.all = false;
    };
  };
}
