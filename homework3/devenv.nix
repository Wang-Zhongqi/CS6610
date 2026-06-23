{
  pkgs,
  lib,
  config,
  inputs,
  ...
}:

let
  freeglutEGL = pkgs.freeglut.overrideAttrs (old: {
    cmakeFlags = (old.cmakeFlags or [ ]) ++ [
      "-DFREEGLUT_USE_EGL=ON"
    ];
  });
in
{
  stdenv = pkgs.ccacheStdenv;

  env = {
    CC = "${pkgs.ccache}/bin/ccache gcc";
    CXX = "${pkgs.ccache}/bin/ccache g++";
    WCC = "${pkgs.ccache}/bin/ccache gcc";
  };

  packages = with pkgs; [
    ccache
    clang
    cmake
    lldb
    ninja
    pkg-config
    freeglutEGL
    glfw
    glew
    glm
  ];

  languages.cplusplus.enable = true;

  enterShell = ''
    echo "✓ Initliazing devenv"
  '';
}
