{ pkgs, ... }:
{
  environment.systemPackages = with pkgs; [ wayvr ];
}
