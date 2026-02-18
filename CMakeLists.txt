cmake_minimum_required(VERSION 3.10)

project(cbarroso
    VERSION 0.6.0
    DESCRIPTION "C library with hashmap implementation"
    LANGUAGES C
)

option(CBR_BUILD_TESTING "Build the testing tree" OFF)

add_library(cbarroso STATIC)

target_sources(cbarroso
    PRIVATE src/hashmap.c
    PRIVATE src/sngllnkdlist.c
    PRIVATE src/dblylnkdlist.c
    PRIVATE src/tree.c
    PRIVATE src/_hash.c
    PRIVATE src/stack.c
    PRIVATE src/queue.c
)

target_include_directories(cbarroso
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(cbarroso PUBLIC c_std_99)

add_library(cbarroso::cbarroso ALIAS cbarroso)

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

install(EXPORT cbarrosoTargets
    FILE cbarrosoTargets.cmake
    NAMESPACE cbarroso::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cbarroso
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/cbarrosoConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

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

if(CBR_BUILD_TESTING)
    enable_testing()

    add_executable(test_runner 
        tests/test_runner.c
        tests/test_hashmap.c
        tests/test_sngllnkdlist.c
        tests/test_dblylnkdlist.c
        tests/test_tree.c
        tests/test_stack.c
        tests/test_queue.c
    )
    
    target_include_directories(test_runner PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/ccauchy/include)
    target_link_libraries(test_runner PRIVATE cbarroso)
    
    add_test(NAME AllTests COMMAND test_runner)
endif()
