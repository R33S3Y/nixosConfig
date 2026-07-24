{ inputs, ... }:
{
  nixpkgs.overlays = [
    (final: prev: {
      stable = inputs.nixpkgsStable {
        system = "x86_64-linux";
        config.allowUnfree = true;
      };
    })
  ];
}
