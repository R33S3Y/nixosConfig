{ config, pkgs, ... }:

{
  # make boots pretty
  boot.plymouth.enable = true; # themes are handled by stylix
  stylix.targets.plymouth.logoAnimated = false;

  stylix.targets.grub.enable = true;
  stylix.targets.grub.useWallpaper = true;

  boot.loader.timeout = 0;
}
