{
  inputs.hyprland.url = "github:hyprwm/Hyprland";

  outputs = {
    self,
    hyprland,
  }: let
    inherit (hyprland.inputs) nixpkgs;
    genPerSystem = fn: nixpkgs.lib.genAttrs (builtins.attrNames hyprland.packages) (system: fn system nixpkgs.legacyPackages.${system});
  in {
    packages = genPerSystem (system: pkgs: {
      default = pkgs.callPackage ./plugin.nix {
        inherit (hyprland.packages.${system}) hyprland-debug;
      };
    });

    devShells = genPerSystem (system: pkgs: {
      default = pkgs.mkShell.override {inherit (hyprland.packages.${system}.hyprland-debug) stdenv;} {
        name = "hyprland-plugin-shell";
        shellHook = ''
          meson setup build --reconfigure
        '';

        nativeBuildInputs = with pkgs; [meson pkg-config ninja];
        buildInputs = [hyprland.packages.${system}.hyprland-debug];
        inputsFrom = [
          hyprland.packages.${system}.hyprland-debug
          self.packages.${system}.default
        ];
      };
    });
  };
}
