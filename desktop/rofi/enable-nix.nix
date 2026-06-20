
{ config, pkgs, ... }:

{
  environment.systemPackages = with pkgs; [
    rofi # start menu
  ];
}
