{
  lib,
  gcc13Stdenv,
  meson,
  ninja,
  pkg-config,
  hyprland-debug,
}: let
  version = builtins.readFile ./VERSION;
in
  gcc13Stdenv.mkDerivation {
    pname = "wigglewobble";
    inherit version;
    src = ./.;

    nativeBuildInputs =
      hyprland-debug.nativeBuildInputs
      ++ [ninja meson pkg-config];

    buildInputs = [hyprland-debug] ++ hyprland-debug.buildInputs;

    meta = with lib; {
      homepage = "https://git.allpurposem.at/mat/WiggleWobble";
      description = "Wobbly windows for hyprland";
      license = licenses.bsd3;
      platforms = platforms.linux;
    };
  }
