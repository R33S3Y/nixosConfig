{ pkgs, lib, ... }:

# this app is discontinued. use only if you must.
{
  nixpkgs.overlays = [
    (final: prev: {
      internal = (prev.internal or { }) // {

        svrbsctl = prev.rustPlatform.buildRustPackage {
          pname = "svrbsctl";
          version = "unstable-2026-09-15";

          src = prev.fetchFromGitHub {
            owner = "chenxiaolong";
            repo = "svrbsctl";
            rev = "179f07f91d30a7507a35ad6384550aee21aa265a";
            hash = lib.fakeHash; # resolved in step 3
          };

          cargoLock = {
            lockFile = "${
              prev.fetchFromGitHub {
                owner = "chenxiaolong";
                repo = "svrbsctl";
                rev = "179f07f91d30a7507a35ad6384550aee21aa265a";
                hash = lib.fakeHash; # same src, keep in sync
              }
            }/Cargo.lock";
          };

          nativeBuildInputs = [ prev.pkg-config ];
          buildInputs = [ prev.dbus ];

          meta = with lib; {
            description = "Control the operating state of SteamVR 2.0 Base Stations";
            homepage = "https://github.com/chenxiaolong/svrbsctl";
            license = licenses.gpl3Only;
            mainProgram = "svrbsctl";
          };
        };
      };
    })
  ];

  environment.systemPackages = [ pkgs.internal.svrbsctl ]; # install it.
}
