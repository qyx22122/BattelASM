{ pkgs ? import <nixpkgs> {} }:
	pkgs.mkShell {
		nativeBuildInputs = with pkgs; [
          gcc
          gnumake
		  raylib
		  gdb
		  gf
		];
		shellHook = ''
		'';
}
