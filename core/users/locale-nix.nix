{ system, ... }:

{
  # Set your time zone.
  time.timeZone = system.users.${system.user}.locale.timeZone or "Pacific/Auckland";

  # Select internationalisation properties.
  i18n = {
    defaultLocale = system.users.${system.user}.locale.defaultLocale or "en_NZ.UTF-8";

    extraLocaleSettings =
      system.users.${system.user}.locale.extraLocaleSettings or {
        LC_ADDRESS = "en_NZ.UTF-8";
        LC_IDENTIFICATION = "en_NZ.UTF-8";
        LC_MEASUREMENT = "en_NZ.UTF-8";
        LC_MONETARY = "en_NZ.UTF-8";
        LC_NAME = "en_NZ.UTF-8";
        LC_NUMERIC = "en_NZ.UTF-8";
        LC_PAPER = "en_NZ.UTF-8";
        LC_TELEPHONE = "en_NZ.UTF-8";
        LC_TIME = "en_NZ.UTF-8";
      };
  };
}
