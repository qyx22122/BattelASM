{ pkgs ? import <nixpkgs> {} }:
	pkgs.mkShell {
		nativeBuildInputs = with pkgs; [
          gcc
          gnumake
		  gdb
		  gf
		];
		shellHook = ''
		'';
}
