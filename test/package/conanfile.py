#**************************** INTELLECTUAL PROPERTY RIGHTS ****************************#
#*                                                                                    *#
#*                           Copyright (c) 2025 Terminus LLC                          *#
#*                                                                                    *#
#*                                All Rights Reserved.                                *#
#*                                                                                    *#
#*          Use of this source code is governed by LICENSE in the repo root.          *#
#*                                                                                    *#
#**************************** INTELLECTUAL PROPERTY RIGHTS ****************************#
#
#    File:    conanfile.py
#    Author:  Marvin Smith
#    Date:    11/22/2025
import os

from conan import ConanFile, tools
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.build import can_run

class LogPackageTestConan( ConanFile ):

    name = "terminus_fcs_test"
    version = "0.0.1"
    description = "Test for Terminus FCS Package"

    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("terminus_fcs/0.0.1")

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure()
        return cmake

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def test(self):
        if can_run(self):
            self.run( ".%sexample" % os.sep )