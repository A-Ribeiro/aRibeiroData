set( LIB_MD5 FromPackage CACHE STRING "Choose the Library Source." )
set_property(CACHE LIB_MD5 PROPERTY STRINGS None FromPackage FromSource)

if(LIB_MD5 STREQUAL FromPackage)

    if (NOT LIBS_REPOSITORY_URL)
        message(FATAL_ERROR "You need to define the LIBS_REPOSITORY_URL to use the FromPackage option for any lib.")
    endif()

    tool_download_lib_package(${LIBS_REPOSITORY_URL} md5)
    tool_include_lib(md5)

elseif(LIB_MD5 STREQUAL UsingFindPackage)

    message(FATAL_ERROR "Build from source not implemented yet." )

else()
    message( FATAL_ERROR "You need to specify the lib source." )
endif()
