from conan import ConanFile
from conan.tools.cmake import CMakeDeps, CMakeToolchain


class ZerrCoreConan(ConanFile):
    name = "zerr_core"
    version = "0.1.0"
    
    requires = "fftw/3.3.10", "yaml-cpp/0.8.0"
    
    settings = "os", "compiler", "build_type", "arch"
    
    def configure(self):
        # Set C++ standard to C++20
        self.settings.compiler.cppstd = "20"
    
    def generate(self):
        # Generate CMake files
        deps = CMakeDeps(self)
        deps.generate()
        
        tc = CMakeToolchain(self)
        tc.generate()

