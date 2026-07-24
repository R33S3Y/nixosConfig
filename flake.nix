{
  description = "Here be dragons";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    nixpkgsStable.url = "github:NixOS/nixpkgs/nixos-26.05";
    home-manager = {
      url = "github:nix-community/home-manager/master";
      inputs.nixpkgs.follows = "nixpkgs";
    };
    stylix = {
      url = "github:danth/stylix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
    nur = {
      url = "github:nix-community/NUR";
      inputs.nixpkgs.follows = "nixpkgs";
    };
    nix-minecraft.url = "github:Infinidoge/nix-minecraft";
    disko = {
      url = "github:nix-community/disko/latest";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs =
    {
      nixpkgs,
      home-manager,
      nur,
      nix-minecraft,
      nixpkgsStable,
      ...
    }@inputs:
    let
      mkHost =
        hostName:
        nixpkgs.lib.nixosSystem {
          system = "x86_64-linux";

          specialArgs = {
            system = {
              hosts = hosts;
              networks = networks;
              users = users;
              themes = themes;
              host = hostName;
              network = hosts.${hostName}.network;
              user = hosts.${hostName}.user;
              theme = users.${hosts.${hostName}.user}.theme;
            };
            inherit inputs; # test
            nixpkgs.config.allowUnfree = true;
          };

          modules = hosts.${hostName}.imports;
        };

      hosts = {
        amethyst = import ./data/hosts/amethyst/host-flake.nix {
          inherit inputs home-manager nur;
        };
        bort = import ./data/hosts/bort/host-flake.nix {
          inherit
            inputs
            home-manager
            nur
            nix-minecraft
            ;
        };
        cinnabar = import ./data/hosts/cinnabar/host-flake.nix {
          inherit inputs home-manager nur;
        };
        diamond = import ./data/hosts/diamond/host-flake.nix {
          inherit inputs home-manager nur;
        };
        ghostQuartz = import ./data/hosts/ghostQuartz/host-flake.nix {
          inherit inputs home-manager;
        };
        morganite = import ./data/hosts/morganite/host-flake.nix {
          inherit inputs home-manager;
        };
        neptunite = import ./data/hosts/neptunite/host-flake.nix {
          inherit inputs home-manager nur;
        };
        obsidian = import ./data/hosts/obsidian/host-flake.nix {
          inherit inputs home-manager;
        };
        #template = import ./data/hosts/template/host-flake.nix {
        #  inherit inputs home-manager nur nix-minecraft;
        #};
      };
      networks = {
        homelab = import ./data/networks/homelab/network-flake.nix {
          inherit inputs;
        };
      };
      users = {
        reese = import ./data/users/reese/user-flake.nix {
          inherit inputs;
        };
        juliette = import ./data/users/juliette/user-flake.nix {
          inherit inputs;
        };
      };
      themes = {
        diamond = import ./data/themes/diamond/theme-flake.nix {
          inherit inputs;
        };
        amethyst = import ./data/themes/amethyst/theme-flake.nix {
          inherit inputs;
        };
      };
    in
    {
      nixosConfigurations = {
        amethyst = mkHost "amethyst";
        bort = mkHost "bort";
        cinnabar = mkHost "cinnabar";
        diamond = mkHost "diamond";
        ghostQuartz = mkHost "ghostQuartz";
        morganite = mkHost "morganite";
        neptunite = mkHost "neptunite";
        obsidian = mkHost "obsidian";
        #template = mkHost "template";
      };
    };
}
