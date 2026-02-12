cmake_minimum_required(VERSION 3.10)

project(cbarroso
    VERSION 0.1.0
    DESCRIPTION "C library with hashmap implementation"
    LANGUAGES C
)

# Option to control library type (for future extensibility)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_TESTING "Build the testing tree" OFF)

# Create the library target
add_library(cbarroso STATIC)

# Add source files
target_sources(cbarroso
    PRIVATE src/hashmap.c
    PRIVATE src/_hash.c
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

# Create a simple Config file
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/cbarrosoConfig.cmake"
"include(CMakeFindDependencyMacro)
include(\"\${CMAKE_CURRENT_LIST_DIR}/cbarrosoTargets.cmake\")
"
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/cbarrosoConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/cbarrosoConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cbarroso
)

# Enable testing
if(BUILD_TESTING)
    enable_testing()

    # Add test executable (excluded from default build)
    add_executable(test_hashmap EXCLUDE_FROM_ALL tests/test_hashmap.c)

    # Link test executable with the library
    target_link_libraries(test_hashmap PRIVATE cbarroso)

    # Add test to CTest
    add_test(NAME HashMapTests COMMAND test_hashmap)
endif()
