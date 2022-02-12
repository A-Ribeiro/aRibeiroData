set( LIB_ARIBEIROCORE FromGit CACHE STRING "Choose the Library Source." )
set_property(CACHE LIB_ARIBEIROCORE PROPERTY STRINGS None FromGit UsingFindPackage)

if (LIB_ARIBEIROCORE STREQUAL FromGit)

    if (GIT_DOWNLOAD_METHOD STREQUAL HTTPS)
        tool_download_git_package("https://github.com/A-Ribeiro/aRibeiroCore.git" aRibeiroCore)
    elseif (GIT_DOWNLOAD_METHOD STREQUAL SSH)
        tool_download_git_package("git@github.com:A-Ribeiro/aRibeiroCore.git" aRibeiroCore)
    else()
        message(FATAL_ERROR "Invalid Git Download Method: ${GIT_DOWNLOAD_METHOD}" )
    endif()

    set(supress_show_info ON)
    tool_include_lib(aRibeiroCore)
    unset(supress_show_info)

elseif (LIB_ARIBEIROCORE STREQUAL UsingFindPackage)

    tool_is_lib(aRibeiroCore aRibeiroPlatform_registered)
    if (NOT ${aRibeiroCore_registered})

        find_package(aRibeiroCore REQUIRED)
        add_library(aRibeiroCore OBJECT ${ARIBEIROCORE_LIBRARIES})
        include_directories(${ARIBEIROCORE_INCLUDE_DIR} PARENT_SCOPE)

        tool_register_lib(aRibeiroCore)
    endif()
else()

    message(FATAL_ERROR "You need to select the source of the aRibeiroCore." )
endif()
