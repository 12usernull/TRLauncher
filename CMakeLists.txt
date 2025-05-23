cmake_minimum_required(VERSION 3.1)

if(WIN32)
    # In Qt 5.1+ we have our own main() function, don't autolink to qtmain on Windows
    cmake_policy(SET CMP0020 OLD)
endif()

project(Launcher)
enable_testing()

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BUILD_DIR}" IS_IN_SOURCE_BUILD)
if(IS_IN_SOURCE_BUILD)
    message(FATAL_ERROR "You are building the Launcher in-source. Please separate the build tree from the source tree.")
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if(CMAKE_HOST_SYSTEM_VERSION MATCHES ".*[Mm]icrosoft.*" OR
        CMAKE_HOST_SYSTEM_VERSION MATCHES ".*WSL.*"
    )
        message(FATAL_ERROR "Building the Launcher is not supported in Linux-on-Windows distributions.")
    endif()
endif()

option(Launcher_RUN_CLANG_TIDY "Run clang-tidy with the compiler." OFF)
if(Launcher_RUN_CLANG_TIDY)
  find_program(CLANG_TIDY_COMMAND NAMES clang-tidy)
  if(NOT CLANG_TIDY_COMMAND)
    message(WARNING "CMake_RUN_CLANG_TIDY is ON but clang-tidy is not found!")
    set(DO_CLANG_TIDY "" CACHE STRING "" FORCE)
  else()
    set(CLANG_TIDY_CHECKS "-modernize-use-trailing-return-type,-readability-magic-numbers,-modernize-avoid-c-arrays")
    set(DO_CLANG_TIDY "${CLANG_TIDY_COMMAND};-checks=${CLANG_TIDY_CHECKS};-header-filter='${CMAKE_SOURCE_DIR}/src/*'")
  endif()
  # TODO: make this per-target, differentiate between libraries and main codebase
  set(CMAKE_CXX_CLANG_TIDY ${DO_CLANG_TIDY})
endif()


##################################### Set CMake options #####################################
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

# Output all executables and shared libs in the main build folder, not in subfolders.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
if(UNIX)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
endif()
set(CMAKE_JAVA_TARGET_OUTPUT_DIR ${PROJECT_BINARY_DIR}/jars)

######## Set compiler flags ########
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_C_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)
include(GenerateExportHeader)
set(CMAKE_CXX_FLAGS " -Wall -pedantic -Wno-deprecated-declarations -D_GLIBCXX_USE_CXX11_ABI=0 -fstack-protector-strong --param=ssp-buffer-size=4 ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE " -O3 -D_FORTIFY_SOURCE=2 ${CMAKE_CXX_FLAGS_RELEASE}")
if(UNIX AND APPLE)
    set(CMAKE_CXX_FLAGS " -stdlib=libc++ ${CMAKE_CXX_FLAGS}")
endif()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Werror -Werror=return-type -O0")

# Fix build with Qt 5.13
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_NO_DEPRECATED_WARNINGS=Y")

##################################### Set Application options #####################################

######## Set URLs ########
set(Launcher_NEWS_RSS_URL "https://multimc.org/rss.xml" CACHE STRING "URL to fetch Launcher's news RSS feed from.")

######## Set version numbers ########
set(Launcher_VERSION_MAJOR    0)
set(Launcher_VERSION_MINOR    7)
set(Launcher_VERSION_HOTFIX   0)

# Build number
set(Launcher_VERSION_BUILD -1 CACHE STRING "Build number. -1 for no build number.")

# Build platform.
set(Launcher_BUILD_PLATFORM "" CACHE STRING "A short string identifying the platform that this build was built for. Only used by the notification system and to display in the about dialog.")

# Channel list URL
set(Launcher_UPDATER_BASE "" CACHE STRING "Base URL for the updater.")

# Notification URL
set(Launcher_NOTIFICATION_URL "" CACHE STRING "URL for checking for notifications.")

# The metadata server
set(Launcher_META_URL "https://meta.multimc.org/v1/" CACHE STRING "URL to fetch Launcher's meta files from.")

# paste.ee API key
set(Launcher_PASTE_EE_API_KEY "utLvciUouSURFzfjPxLBf5W4ISsUX4pwBDF7N1AfZ" CACHE STRING "API key you can get from paste.ee when you register an account")

# Imgur API Client ID
set(Launcher_IMGUR_CLIENT_ID "5b97b0713fba4a3" CACHE STRING "Client ID you can get from Imgur when you register an application")

# Google analytics ID
set(Launcher_ANALYTICS_ID "UA-87731965-2" CACHE STRING "ID you can get from Google analytics")

# Bug tracker URL
set(Launcher_BUG_TRACKER_URL "" CACHE STRING "URL for the bug tracker.")

# Discord URL
set(Launcher_DISCORD_URL "" CACHE STRING "URL for the Discord guild.")

# Subreddit URL
set(Launcher_SUBREDDIT_URL "" CACHE STRING "URL for the subreddit.")

# Use the secrets library or a public stub?
option(Launcher_EMBED_SECRETS "Determines whether to embed secrets. Secrets are separate and non-public." OFF)

# API Keys
# NOTE: These API keys are here for convenience. If you rebrand this software or intend to break the terms of service
# of these platforms, please change these API keys beforehand.
# Be aware that if you were to use these API keys for malicious purposes they might get revoked, which might cause
# breakage to thousands of users.
# If you don't plan to use these features of this software, you can just remove these values.

# By using this key in your builds you accept the terms of use laid down in
# https://docs.microsoft.com/en-us/legal/microsoft-identity-platform/terms-of-use
set(Launcher_MSA_CLIENT_ID "f4404707-7bbe-4e40-80ba-85fb2bb825a1" CACHE STRING "Client ID you can get from Microsoft Identity Platform when you register an application")

#### Check the current Git commit and branch
include(GetGitRevisionDescription)
get_git_head_revision(Launcher_GIT_REFSPEC Launcher_GIT_COMMIT)

message(STATUS "Git commit: ${Launcher_GIT_COMMIT}")
message(STATUS "Git refspec: ${Launcher_GIT_REFSPEC}")

set(Launcher_RELEASE_VERSION_NAME "${Launcher_VERSION_MAJOR}.${Launcher_VERSION_MINOR}.${Launcher_VERSION_HOTFIX}")

#### Custom target to just print the version.
add_custom_target(version echo "Version: ${Launcher_RELEASE_VERSION_NAME}")
add_custom_target(tcversion echo "\\#\\#teamcity[setParameter name=\\'env.LAUNCHER_VERSION\\' value=\\'${Launcher_RELEASE_VERSION_NAME}\\']")

################################ 3rd Party Libs ################################

# Find the required Qt parts
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Test REQUIRED)
find_package(Qt5Xml REQUIRED)

# The Qt5 cmake files don't provide its install paths, so ask qmake.
include(QMakeQuery)
query_qmake(QT_INSTALL_PLUGINS QT_PLUGINS_DIR)
query_qmake(QT_INSTALL_IMPORTS QT_IMPORTS_DIR)
query_qmake(QT_INSTALL_LIBS QT_LIBS_DIR)
query_qmake(QT_INSTALL_LIBEXECS QT_LIBEXECS_DIR)
query_qmake(QT_HOST_DATA QT_DATA_DIR)
set(QT_MKSPECS_DIR ${QT_DATA_DIR}/mkspecs)

if (Qt5_POSITION_INDEPENDENT_CODE)
    SET(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

####################################### Secrets #######################################

if(Launcher_EMBED_SECRETS)
    add_subdirectory(secrets)
else()
    add_subdirectory(notsecrets)
endif()

####################################### Install layout #######################################

# How to install the build results
set(Launcher_LAYOUT "auto" CACHE STRING "The layout for the launcher installation (auto, win-bundle, lin-nodeps, mac-bundle)")
set_property(CACHE Launcher_LAYOUT PROPERTY STRINGS auto win-bundle lin-nodeps mac-bundle)

if(Launcher_LAYOUT STREQUAL "auto")
    if(UNIX AND APPLE)
        set(Launcher_LAYOUT_REAL "mac-bundle")
    elseif(UNIX)
        set(Launcher_LAYOUT_REAL "lin-nodeps")
    elseif(WIN32)
        set(Launcher_LAYOUT_REAL "win-bundle")
    else()
        message(FATAL_ERROR "Cannot choose a sensible install layout for your platform.")
    endif()
else()
    set(Launcher_LAYOUT_REAL ${Launcher_LAYOUT})
endif()

if(Launcher_LAYOUT_REAL STREQUAL "mac-bundle")
    set(BINARY_DEST_DIR "${Launcher_Name}.app/Contents/MacOS")
    set(LIBRARY_DEST_DIR "${Launcher_Name}.app/Contents/MacOS")
    set(PLUGIN_DEST_DIR "${Launcher_Name}.app/Contents/MacOS")
    set(RESOURCES_DEST_DIR "${Launcher_Name}.app/Contents/Resources")
    set(JARS_DEST_DIR "${Launcher_Name}.app/Contents/MacOS/jars")

    set(BUNDLE_DEST_DIR ".")

    # Apps to bundle
    set(APPS "\${CMAKE_INSTALL_PREFIX}/${Launcher_Name}.app")

    # Mac bundle settings
    set(MACOSX_BUNDLE_BUNDLE_NAME "${Launcher_Name}")
    set(MACOSX_BUNDLE_INFO_STRING "${Launcher_Name}:  Minecraft launcher and management utility.")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.multimc.${Launcher_Name}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "${Launcher_VERSION_MAJOR}.${Launcher_VERSION_MINOR}.${Launcher_VERSION_HOTFIX}.${Launcher_VERSION_BUILD}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${Launcher_VERSION_MAJOR}.${Launcher_VERSION_MINOR}.${Launcher_VERSION_HOTFIX}.${Launcher_VERSION_BUILD}")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${Launcher_VERSION_MAJOR}.${Launcher_VERSION_MINOR}.${Launcher_VERSION_HOTFIX}.${Launcher_VERSION_BUILD}")
    set(MACOSX_BUNDLE_ICON_FILE ${Launcher_Name}.icns)
    set(MACOSX_BUNDLE_COPYRIGHT "Copyright 2015-2023 ${Launcher_Copyright}")

    # directories to look for dependencies
    set(DIRS ${QT_LIBS_DIR} ${QT_LIBEXECS_DIR} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    # install as bundle
    set(INSTALL_BUNDLE "full")

    # Add the icon
    install(FILES ${Launcher_Branding_ICNS} DESTINATION ${RESOURCES_DEST_DIR} RENAME ${Launcher_Name}.icns)

elseif(Launcher_LAYOUT_REAL STREQUAL "lin-nodeps")
    set(BINARY_DEST_DIR "bin")
    set(LIBRARY_DEST_DIR "bin")
    set(PLUGIN_DEST_DIR "plugins")
    set(BUNDLE_DEST_DIR ".")
    set(RESOURCES_DEST_DIR ".")
    set(JARS_DEST_DIR "bin/jars")

    # install as bundle with no dependencies included
    set(INSTALL_BUNDLE "nodeps")

    # Set RPATH
    SET(Launcher_BINARY_RPATH "$ORIGIN/")

    # Install basic runner script
    configure_file(launcher/Launcher.in "${CMAKE_CURRENT_BINARY_DIR}/LauncherScript" @ONLY)
    install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/LauncherScript" DESTINATION ${BUNDLE_DEST_DIR} RENAME ${Launcher_Name})

elseif(Launcher_LAYOUT_REAL STREQUAL "win-bundle")
    set(BINARY_DEST_DIR ".")
    set(LIBRARY_DEST_DIR ".")
    set(PLUGIN_DEST_DIR ".")
    set(BUNDLE_DEST_DIR ".")
    set(RESOURCES_DEST_DIR ".")
    set(JARS_DEST_DIR "jars")

    # Apps to bundle
    set(APPS "\${CMAKE_INSTALL_PREFIX}/${Launcher_Name}.exe")

    # directories to look for dependencies
    set(DIRS ${QT_LIBS_DIR} ${QT_LIBEXECS_DIR} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    # install as bundle
    set(INSTALL_BUNDLE "full")
else()
    message(FATAL_ERROR "No sensible install layout set.")
endif()

################################ Included Libs ################################

include(ExternalProject)
set_directory_properties(PROPERTIES EP_BASE External)

option(NBT_BUILD_SHARED "Build NBT shared library" ON)
option(NBT_USE_ZLIB "Build NBT library with zlib support" OFF)
option(NBT_BUILD_TESTS "Build NBT library tests" OFF) #FIXME: fix unit tests.
set(NBT_NAME Launcher_nbt++)
set(NBT_DEST_DIR ${LIBRARY_DEST_DIR})
add_subdirectory(libraries/libnbtplusplus)

add_subdirectory(libraries/ganalytics) # google analytics library
add_subdirectory(libraries/systeminfo) # system information library
add_subdirectory(libraries/hoedown) # markdown parser
add_subdirectory(libraries/launcher) # java based launcher part for Minecraft
add_subdirectory(libraries/javacheck) # java compatibility checker
add_subdirectory(libraries/xz-embedded) # xz compression
add_subdirectory(libraries/quazip) # zip manipulation library
add_subdirectory(libraries/rainbow) # Qt extension for colors
add_subdirectory(libraries/iconfix) # fork of Qt's QIcon loader
add_subdirectory(libraries/LocalPeer) # fork of a library from Qt solutions
add_subdirectory(libraries/classparser) # google analytics library
add_subdirectory(libraries/optional-bare)
add_subdirectory(libraries/tomlc99) # toml parser
add_subdirectory(libraries/katabasis) # An OAuth2 library that tried to do too much

############################### Built Artifacts ###############################

add_subdirectory(buildconfig)

# NOTE: this must always be last to appease the CMake deity of quirky install command evaluation order.
add_subdirectory(launcher)
