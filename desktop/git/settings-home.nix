{ system, ... }:
{
  programs.git = {
    enable = true;
    settings = [
      {
        user = {
          email = system.users.${system.user}.email;
          name = system.users.${system.user}.prettyName;
        };
      }
    ];
  };
  programs.ssh = {
    enable = true;
    settings = {
      "github.com" = {
        hostname = "github.com";
        user = "git";
        identityFile = "~/.ssh/github"; # TODO - Maybe we can get better key management
      };
    };
  };
}
