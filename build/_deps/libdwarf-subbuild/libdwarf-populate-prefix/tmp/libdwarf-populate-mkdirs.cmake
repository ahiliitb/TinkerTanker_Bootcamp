# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/libdwarf-src"
  "/home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/libdwarf-build"
  "/home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/libdwarf-subbuild/libdwarf-populate-prefix"
  "/home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/libdwarf-subbuild/libdwarf-populate-prefix/tmp"
  "/home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/libdwarf-subbuild/libdwarf-populate-prefix/src/libdwarf-populate-stamp"
  "/home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/libdwarf-subbuild/libdwarf-populate-prefix/src"
  "/home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/libdwarf-subbuild/libdwarf-populate-prefix/src/libdwarf-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/libdwarf-subbuild/libdwarf-populate-prefix/src/libdwarf-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/libdwarf-subbuild/libdwarf-populate-prefix/src/libdwarf-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
