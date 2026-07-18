{ pkgs, ... }: {
  programs.lutris = {
    enable = true;
    extraPackages = with pkgs; [
      libadwaita
      gtk4
    ];
  };
}
