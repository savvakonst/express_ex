function(get_interface_link_libraries_recursively out_libs)
    
    set(TMP_OUT_LIBS )
    #message("get_interface_link_libraries_recursively ${ARGN}")
    set(TMP_LIST_DEPTH_0 ${ARGN} )
    set(TMP_CNT 0)
    while(TMP_LIST_DEPTH_0)
        if(NOT TMP_LIST_DEPTH_${TMP_CNT})
            unset(TMP_LIST_DEPTH_${TMP_CNT})
            math(EXPR TMP_CNT "${TMP_CNT}-1" )
            list(POP_FRONT TMP_LIST_DEPTH_${TMP_CNT} )
        endif()

        if(NOT TMP_LIST_DEPTH_${TMP_CNT} )
            continue()
        endif()

        list(GET TMP_LIST_DEPTH_${TMP_CNT} 0 TMP_CURRENT)
        #message("TMP_CURRENT ${TMP_CURRENT}")
        math(EXPR TMP_CNT "${TMP_CNT}+1" )
        if(TARGET ${TMP_CURRENT})
            get_target_property(TMP_LIST_DEPTH_${TMP_CNT} ${TMP_CURRENT} INTERFACE_LINK_LIBRARIES )

            if(NOT ${TMP_CURRENT} IN_LIST TMP_OUT_LIBS)
                set(TMP_OUT_LIBS ${TMP_OUT_LIBS} ${TMP_CURRENT})
            endif()
        else()
            set(TMP_LIST_DEPTH_${TMP_CNT})
        endif()

        if(NOT TMP_LIST_DEPTH_${TMP_CNT})
            unset(TMP_LIST_DEPTH_${TMP_CNT})
            math(EXPR TMP_CNT "${TMP_CNT}-1" )
            list(POP_FRONT TMP_LIST_DEPTH_${TMP_CNT} )
        endif()
    endwhile()

    set(${out_libs} ${TMP_OUT_LIBS} PARENT_SCOPE)
endfunction()


function(get_library_paths out_paths)
    get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if (IS_MULTI_CONFIG)
        message( "get_library_paths returns empty list, since generator is multi config")
        set(LIBS )
        set(TMP_OUT_LIB_PATHS )
    else()
        set(LIBS ${ARGN})
    endif()

    foreach(TMP_VAR ${LIBS})
        string(TOUPPER ${CMAKE_BUILD_TYPE} TMP_CONFIG)
        get_target_property(TEMP_VAR  ${TMP_VAR} IMPORTED_LOCATION_${TMP_CONFIG} )
        set(TMP_OUT_LIB_PATHS  ${TMP_OUT_LIB_PATHS}   ${TEMP_VAR})
    endforeach()
    
    set(${out_paths} ${TMP_OUT_LIB_PATHS} PARENT_SCOPE)
endfunction()