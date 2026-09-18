{
  system,
  pkgs,
  ...
}:
let
  theme = system.themes.${system.theme};
in
{

  home.pointerCursor = {
    enable = true;
    name = "BreezeX-RosePine-Linux"; # Cursor name is the folder name of your choice in /nix/store/{{cursor Theme}}/share/icons
    package = pkgs.rose-pine-cursor;
    size = 32;

    gtk.enable = true;
    x11.enable = true;

    # Pointer is still not 100% consisant bettween apps, desktop and the phase of the moon. (when pixel peeping)
    # But it's close enough that I dont care.
    # That and I'm pretty sure that it's down to things like diffs with gtk and qt or X11 scaling or something like that

  };
}
