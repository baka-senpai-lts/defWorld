# defWorld - ECS Game Framework

A lightweight **Entity-Component-System (ECS)** framework in C++ that abstracts rendering and provides useful utilities for building 2D games or simulations. The framework is designed to be modular, flexible, and easy to extend.

**Warning:** This project is WIP, so you are **highly discouraged** to use it. Things will break, API will change and nothing is sacred. Use at your own risk.

---

## Features

- **Entity-Component-System Architecture**
  - Define components and systems independently.
  - Query entities based on their components.
  - Handle system dependencies and execution phases (`INPUT`, `EARLY`, `LOGIC`, `LATE`, `PHYSICS`, `RENDER`).

- **Rendering Abstraction**
  - Currently built on top of raylib, but it's expected to switch to plain OpenGL in near future.
  - Texture provider for easy resource management.

- **Utilities**
  - EntityBuilder for creating entities with multiple components conveniently.
  - Logging system with configurable levels.
  - Event system for input handling (keyboard, mouse, etc.).
  - Component and system lifecycle management.

---

## Installation

This framework requires a **C++17 compatible compiler**, also things are expected to change when using anything other than GCC.
Fork this repository and use it as a template to begin building and breaking stuff in your own ECS sandbox.

---

## Documentation

No documentation yet. If you want to explore, read the sources. Good luck navigating it.

---

## Contributing

Contributions are welcome!
- Extend the rendering abstraction to support more drawable types.
- Improve performance and ergonomics.
- Add new features, refactor code, or just play with it.
