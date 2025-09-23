## Overview
- Game engine made from scratch in c++ with very little dependencies. Intended to
help me learn complicated things in game dev and game engine design

## Dependencies
- GLFW - For cross-platform window creation
- GLAD - For loading modern Opengl functions

## Goals
- Learn how to work with graphics programming
- Learn how to work with c++
- Learn interesting math used in game dev (Matrices, Vectors)
- Learn how game engines (like Unity, Unreal, Godot) work in the backend
- Make games how I want with no limitations (If that makes sense)
- ### Goals for Engine itself
  - Scene saving/loading system
  - Animation system - Done :)
  - GUI for editing scenes
  - Lighting system - like RTX
  - 2 channel audio system (with procedural-ness, like random pitch changes)
  - UI system
  - Physics and colliders
  - Text fonts and animations
  - Anti-aliasing
  - Particle System

## Naming Scheme
  - PascalCase for classes, structs, typedef/using, namespaces, enums
  - camelCase for variables, and functions
    - Private class fields have prefix of '_'
  - ALL_CAPS for template variables and constexprs
  - all_lower for enum options (enumerators)
  - loops use

## Coding Style
  - I implement all of things myself instead of using standard library or other library because i want to learn how its done
  - I find lots of small functions harder to read than 1 big function so expect big functions
  - I really like giving the user (dev) lots of control

## Building
  - Built using Cmake and apple clang
  - On mac install GLFW and openal-soft through homebrew, glad is included inside the project