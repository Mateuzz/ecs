# Ecs Library developed in C++
A very simple library for Entity Component Systems developed in C++, it allows
the client to create entities, which are a simple Id/Index and attach components to it.
It makes very easy to iterate through all components of some kind, allowing fast
execution of computations that requires the same data. 

This library is just a simple learning project, it's not tested and lack many features.

## Requisites
Make or premake5 + some other build tool supported
by premake5, like make, Visual Studio, xcode4 or CodeLite

## Build
This project uses premake as the building tool, but it includes the makefile
in the repository, so you can just run:

```bash
make config=release_x64
# for 32 bits systems
make config=release_x32
```

If you don't want to use make, you need to install premake5 and use it to generate
your favorite build tool.

```bash
# generates a Visual Studio 2022 Solution
premake5 --c=gcc vs2022
```

## License 
MIT License
