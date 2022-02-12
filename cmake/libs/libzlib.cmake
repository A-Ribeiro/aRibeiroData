set( LIB_ZLIB FromPackage CACHE STRING "Choose the Library Source." )
set_property(CACHE LIB_ZLIB PROPERTY STRINGS None FromPackage UsingFindPackage)

if(LIB_ZLIB STREQUAL FromPackage)

    if (NOT LIBS_REPOSITORY_URL)
        message(FATAL_ERROR "You need to define the LIBS_REPOSITORY_URL to use the FromPackage option for any lib.")
    endif()

    tool_download_lib_package(${LIBS_REPOSITORY_URL} zlib)

    set(SKIP_INSTALL_ALL ON)
    tool_include_lib(zlib)
    #unset(SKIP_INSTALL_ALL)

    include_directories(${CMAKE_HOME_DIRECTORY}/include/zlib/ PARENT_SCOPE)

elseif(LIB_ZLIB STREQUAL UsingFindPackage)

    tool_is_lib(zlib zlib_registered)
    if (NOT ${zlib_registered})
        find_package(ZLIB REQUIRED)

        add_library(zlib OBJECT ${ZLIB_LIBRARIES})
        #target_link_libraries(zlib INTERFACE ${ZLIB_LIBRARIES})
        include_directories(${ZLIB_INCLUDE_DIR} PARENT_SCOPE)

        # set the target's folder (for IDEs that support it, e.g. Visual Studio)
        set_target_properties(zlib PROPERTIES FOLDER "LIBS")

        tool_register_lib(zlib)
    endif()

else()
    message( FATAL_ERROR "You need to specify the lib source." )
endif()
