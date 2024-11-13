# Raytracer

## Description

A raytracer that renders non-Euclidean geometries.

## How to run

**UNIX**: You'll need CMake, vcpkg and Ninja installed.

**Windows**: You'll need CMake and vcpkg, but I have no idea what to substitute Ninja with (Visual studio build files?).

Create a folder called "build", navigate to it, and then run the following commands:

```
    cmake --preset=default ..
    ninja
    ./raytracer > image.ppm
```

Then view the ppm file in an application that can see it; you should see a sphere with a blue-ish background.
