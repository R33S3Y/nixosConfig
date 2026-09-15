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
            hash = "sha256-L6oTHyWNQUGahXYZKuzb9pM5x92Rpb9vIzq9HUW+9Rs="
          };

          cargoLock = {
            lockFile = "${
              prev.fetchFromGitHub {
                owner = "chenxiaolong";
                repo = "svrbsctl";
                rev = "179f07f91d30a7507a35ad6384550aee21aa265a";
                hash = lib.fakeHash;
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

  environment.systemPackages = [ pkgs.internal.svrbsctl ];
}
