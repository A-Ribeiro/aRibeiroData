set( LIB_ARIBEIRODATA FromGit CACHE STRING "Choose the Library Source." )
set_property(CACHE LIB_ARIBEIRODATA PROPERTY STRINGS None FromGit UsingFindPackage)

if (LIB_ARIBEIRODATA STREQUAL FromGit)

    if (GIT_DOWNLOAD_METHOD STREQUAL HTTPS)
        tool_download_git_package("https://github.com/A-Ribeiro/aRibeiroData.git" aRibeiroData)
    elseif (GIT_DOWNLOAD_METHOD STREQUAL SSH)
        tool_download_git_package("git@github.com:A-Ribeiro/aRibeiroData.git" aRibeiroData)
    else()
        message(FATAL_ERROR "Invalid Git Download Method: ${GIT_DOWNLOAD_METHOD}" )
    endif()

    set(supress_show_info ON)
    tool_include_lib(aRibeiroData)
    unset(supress_show_info)

elseif (LIB_ARIBEIRODATA STREQUAL UsingFindPackage)

    tool_is_lib(aRibeiroData aRibeiroData_registered)
    if (NOT ${aRibeiroData_registered})

        find_package(aRibeiroData REQUIRED)
        add_library(aRibeiroData OBJECT ${ARIBEIRODATA_LIBRARIES})
        include_directories(${ARIBEIRODATA_INCLUDE_DIR} PARENT_SCOPE)

        tool_register_lib(aRibeiroData)
    endif()

else()

    message(FATAL_ERROR "You need to select the source of the aRibeiroData." )
endif()
