#
# Copyright (C) Troy D. Straszheim
#
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#   http://www.boost.org/LICENSE_1_0.txt
#
#
# Determine if we have a version of boost available that works
#
# Thanks Tronic for some sharp eyes.
# 
get_filename_component(CWD ${CMAKE_CURRENT_LIST_FILE} PATH)

file(GLOB AVAILABLE_VERSIONFILES 
  RELATIVE ${CWD} 
  "C:/Projets/PCL/SuperBuild_boost_1_50_x86/CMakeExternal/Install/Boost/share/boost-*/cmake/BoostConfig.cmake")

message("globbed: ${AVAILABLE_VERSIONFILES}")
if (EXISTS "C:/Projets/PCL/SuperBuild_boost_1_50_x86/CMakeExternal/Install/Boost/share/boost-${PACKAGE_FIND_VERSION_MAJOR}.${PACKAGE_FIND_VERSION_MINOR}.${PACKAGE_FIND_VERSION_PATCH}/cmake/BoostConfig.cmake")
  #
  #  Exact version
  #
  set(BOOST_FOUND TRUE)
  set(PACKAGE_VERSION ${PACKAGE_FIND_VERSION})
  set(PACKAGE_VERSION_EXACT TRUE)
  set(PACKAGE_VERSION_COMPATIBLE TRUE)
  set(PACKAGE_VERSION_UNSUITABLE FALSE)

elseif(AVAILABLE_VERSIONFILES)

  #
  #  Find any version
  #
  list(SORT AVAILABLE_VERSIONFILES)
  list(REVERSE AVAILABLE_VERSIONFILES)
  list(GET AVAILABLE_VERSIONFILES 0 MOSTRECENT)
  #
  #  Warning bugs: this assumes that there are no other x.yy.zz type
  #  numbers in your path
  #
  string(REGEX MATCH 
    "[0-9]+\\.[0-9]+\\.[0-9]+" 
    FOUNDVERSION ${MOSTRECENT})

  message("FOUNDVERSION=${FOUNDVERSION}")
  if(PACKAGE_FIND_VERSION)
    if(${PACKAGE_FIND_VERSION} STRLESS ${FOUNDVERSION})
      #
      #  Okay, we have newer than requested
      #
      set(PACKAGE_VERSION ${FOUNDVERSION})
      set(PACKAGE_VERSION_EXACT FALSE)
      set(PACKAGE_VERSION_COMPATIBLE TRUE)
      set(PACKAGE_VERSION_UNSUITABLE FALSE)
    else(${PACKAGE_FIND_VERSION} STRGREATER ${FOUNDVERSION})
      #
      #  Sorry, we only have older than requested
      #
      set(PACKAGE_VERSION ${FOUNDVERSION})
      set(PACKAGE_VERSION_EXACT FALSE)
      set(PACKAGE_VERSION_COMPATIBLE FALSE)
      set(PACKAGE_VERSION_UNSUITABLE TRUE)
    endif()
  else()
    set(PACKAGE_VERSION ${FOUNDVERSION})
  endif()
endif()

