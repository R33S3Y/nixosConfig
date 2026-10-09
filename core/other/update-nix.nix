{ system, ... }:
{
  system.autoUpgrade = {
    enable = true;

    flake = system.networks.${system.network}.repo;

    persistent = true; # this gets it to run if it would of run while off.
    runGarbageCollection = true;

    dates = "02:00";

    fixedRandomDelay = true;
    randomizedDelaySec = "45min";

    # Under the current implementation of allowReboot if the system needs to reboot (due to a
    # kernel update) and if the system never hits the reboot window. It will still apply the
    # update just with "nixos-rebuild boot" instead of "nixos-rebuild switch" as long as the
    # User supplies the reboot it will still get updated.
    #
    # See : https://github.com/NixOS/nixpkgs/blob/master/nixos/modules/tasks/auto-upgrade.nix#L278
    allowReboot = true;
    rebootWindow = {
      lower = "01:00";
      upper = "04:00";
    };
  };
}
