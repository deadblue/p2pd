# Add boost dependency to target
macro(add_boost target_)
    # Require boost with minimum version 1.66. 
    find_package(Boost 1.66.0 REQUIRED)
    target_include_directories(${target_}
        PRIVATE ${Boost_INCLUDE_DIRS}
    )
    # Link boost_system library when boost version is less than 1.69.
    if(Boost_VERSION VERSION_LESS 1.69.0)
        find_package(Boost REQUIRED COMPONENTS system)
        target_link_libraries(${target_}
            PRIVATE ${Boost_SYSTEM_LIBRARY} 
        )
    endif()
endmacro()

macro(add_private_library target_ name_)
    find_package(PkgConfig)
    pkg_check_modules(LIB REQUIRED ${name_})
    target_include_directories(${target_}
        PRIVATE ${LIB_INCLUDE_DIRS}
    )
    target_link_libraries(${target_}
        PRIVATE ${LIB_LINK_LIBRARIES}
    )
endmacro()
