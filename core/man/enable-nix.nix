{ pkgs, ... }:

{
  environment.systemPackages = with pkgs; [
    bat
    # just more man pages
    man-db
    man-pages
    openssl_4_0
    gcc.man
  ];
}
