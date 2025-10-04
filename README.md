# TSG2

TheStandardGuy's game engine.

## Overview

This is a game engine developed from scratch. It's developed as my case study to improve my computer graphic, architectural and C++ skills.

### Learning purpouse
- Computer graphics
- C++23
- Game engine design
- Software architecture

### Structure

- **egine**: it contains the main library tsg2 that works as full game engine that provides inputs managing, collision detection, physics engine, graphic engine.
- **externals**: it contains the  linked librares like the generic one tsg used to do math, logging stuff and others utilities and the graphic libraries like GLFW.
- **test**: it contains the toy-games to test tsg2 capabilities.
  
### Main features

- **Compile-time metaprogrammin**: choosing the dimension of the game a lot of algorithm are chosen accordingly with it. Furthermore throw the CRTP I reduce the overhead of inheritance.
- **Cross graphic-library**: it is all architectured to works indipendently from which graphic technology is used, OpenGL or also Vulkan (still to do).

## Roadmap

- [x] Core engine architecture
- [ ] OpenGL rendering
  - [x] Sprite rendering
  - [ ] Mesh rendering
- [ ] Input management
  - [x] Keyboard input
  - [ ] Mouse input
- [x] Physics
- [ ] Contact resolution
  - [x] AABB contact
  - [x] Sphere
  - [ ] OBB
- [ ] Audio System 
- [ ] Vulkan rendering

## Acknowledgments

- Thanks to all my Dear Colleagues that knows.
