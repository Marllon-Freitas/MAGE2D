# MAGE2D 🚧

MAGE2D is a simple 2D game engine built for educational purposes. It leverages Direct3D for rendering and provides a basic framework for creating 2D games.

## Features

- **Window Management**: Create and manage game windows.
- **Graphics**: Initialize and use Direct3D for rendering.
- **Game Loop**: A real-time loop to handle game updates and rendering.
- **Input Handling**: Basic keyboard and mouse input handling.
- **Timer**: High-resolution timer for precise time measurements and controlling the frame update rate (FPS).
- **Texture Loading**: Load and create Direct3D 11 runtime textures from WIC images.
- **Image Handling**: Load and manage images as textures in the game engine.
- **Spriet Management**: Create and manipulate 2D sprites.

## Project Structure

- **Engine.h / Engine.cpp**: Core of the engine, handles the game loop and initialization.
- **Game.h / Game.cpp**: Abstract base class for the game, which must be inherited to create a specific game.
- **Graphics.h / Graphics.cpp**: Manages Direct3D initialization and rendering.
- **Window.h / Window.cpp**: Manages window creation and input handling.
- **Timer.h / Timer.cpp**: Provides high-resolution timing functionalities and controls the frame update rate (FPS).
- **Texture.h / Texture.cpp**: Functions for loading WIC images and creating Direct3D 11 runtime textures.
- **Image.h / Image.cpp**: Manages image loading and provides access to texture views.
- **Sprite.h / Sprite.cpp**: Represents 2D images that can be drawn on the screen.
- **Renderer.h / Renderer.cpp**: Manages and renders sprites efficiently.
- **Types.h**: Defines specific types used throughout the engine.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- DirectX SDK
- Microsoft Visual Studio

---

## Work in Progress

This personal project is a work in progress and is intended for educational purposes. It serves as a foundation for learning about game engine development and Direct3D.
