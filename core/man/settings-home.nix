{ ... }:
{
  programs.bash = {
    enable = true;

    bashrcExtra = ''
      export MANPAGER="sh -c 'col -bx | bat -l man -p'"
      export MANROFFOPT="-c"
      export BAT_THEME="ansi"
    '';
  };
}
