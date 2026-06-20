{ pkgs, ... }:

{
  # Bootloader.
  boot.loader.grub.enable = true;

  boot.loader.grub.efiSupport = true;
  boot.loader.efi.canTouchEfiVariables = true;
  boot.loader.grub.device = "nodev";

  boot.kernelPackages = pkgs.linuxPackages_latest;
}
