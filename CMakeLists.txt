cmake_minimum_required(VERSION 3.10)

project(cbarroso
    VERSION 0.1.0
    DESCRIPTION "C library with hashmap implementation"
    LANGUAGES C
)

# Option to control library type (for future extensibility)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Create the library target
add_library(cbarroso STATIC)

# Add source files
target_sources(cbarroso PRIVATE
    src/hashmap.c
)

# Set include directories
target_include_directories(cbarroso
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Set C standard
target_compile_features(cbarroso PUBLIC c_std_99)

# Add an alias for consistency when using add_subdirectory
add_library(cbarroso::cbarroso ALIAS cbarroso)

# Installation rules (optional, but useful for install workflows)
include(GNUInstallDirs)

install(TARGETS cbarroso
    EXPORT cbarrosoTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Export targets for find_package support
install(EXPORT cbarrosoTargets
    FILE cbarrosoTargets.cmake
    NAMESPACE cbarroso::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cbarroso
)

# Create a ConfigVersion file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/cbarrosoConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Create a Config file
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cbarrosoConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cbarrosoConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cbarroso
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/cbarrosoConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/cbarrosoConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cbarroso
)
