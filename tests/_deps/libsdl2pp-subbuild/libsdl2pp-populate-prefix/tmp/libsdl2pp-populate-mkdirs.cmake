# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/facundo/taller/tp-grupal/duck-game/tests/_deps/libsdl2pp-src"
  "/home/facundo/taller/tp-grupal/duck-game/tests/_deps/libsdl2pp-build"
  "/home/facundo/taller/tp-grupal/duck-game/tests/_deps/libsdl2pp-subbuild/libsdl2pp-populate-prefix"
  "/home/facundo/taller/tp-grupal/duck-game/tests/_deps/libsdl2pp-subbuild/libsdl2pp-populate-prefix/tmp"
  "/home/facundo/taller/tp-grupal/duck-game/tests/_deps/libsdl2pp-subbuild/libsdl2pp-populate-prefix/src/libsdl2pp-populate-stamp"
  "/home/facundo/taller/tp-grupal/duck-game/tests/_deps/libsdl2pp-subbuild/libsdl2pp-populate-prefix/src"
  "/home/facundo/taller/tp-grupal/duck-game/tests/_deps/libsdl2pp-subbuild/libsdl2pp-populate-prefix/src/libsdl2pp-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/facundo/taller/tp-grupal/duck-game/tests/_deps/libsdl2pp-subbuild/libsdl2pp-populate-prefix/src/libsdl2pp-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/facundo/taller/tp-grupal/duck-game/tests/_deps/libsdl2pp-subbuild/libsdl2pp-populate-prefix/src/libsdl2pp-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
