from conans import ConanFile, CMake, tools


class ShpongConan(ConanFile):
    name = "Sphong"
    version = "0.0.1"
    license = "The Unlicense"
    author = "adnn"
    url = "https://github.com/Adnn/shpong"
    description = "2D game with balls"
    topics = ("opengl", "2D", "Game")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    generators = "cmake_paths"
    build_policy = "missing"

    requires = (
        ("glfw/3.3@bincrafters/stable"),
    )

    default_options = {
        "shared": False,
    }
