function(string_starts_with output str search)
    string(FIND "${str}" "${search}" out)
    if("${out}" EQUAL 0)
        set(${output} TRUE PARENT_SCOPE)
        return()
    endif()
    set(${output} FALSE PARENT_SCOPE)
endfunction(string_starts_with output str search)

function(string_split_at_last partb input separator)
  string(FIND "${input}" "${separator}" idx  REVERSE)

  if(${idx} LESS 0 OR "${separator}_" STREQUAL "_")
    set(${partb} "" PARENT_SCOPE)
    return()
  endif()

  math(EXPR idx "${idx} + 1")

  string(SUBSTRING "${input}" ${idx} -1 pb)
  set(${partb} ${pb} PARENT_SCOPE)
endfunction(string_split_at_last partb input separator)

set(TARGET "button_1" "button_2" "switch_1" "joystick_1" "slider_1")
foreach(element ${TARGET})
    add_executable(${element})

    pico_set_program_name(${element} "${element}")
    pico_set_program_version(${element} "0.1")

    pico_enable_stdio_uart(${element} 1)
    pico_enable_stdio_usb(${element} 1)

    include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

    string_starts_with(BUTTON ${element} "button")
    string_starts_with(SWITCH ${element} "switch")
    string_starts_with(POTENTIOMETER ${element} "potentiometer")
    string_starts_with(SLIDER ${element} "slider")
    string_starts_with(JOYSTICK ${element} "joystick")
    string_split_at_last(ID_NUM ${element} "_")
    set(ID "ID_${ID_NUM}")

    if(BUTTON)
        target_compile_definitions(${element} PRIVATE 
            BUTTON DIGITAL ${ID}
        )
    endif()
    if(SWITCH)
        target_compile_definitions(${element} PRIVATE
            SWITCH DIGITAL ${ID}
        )
    endif()
    if(POTENTIOMETER)
        target_compile_definitions(${element} PRIVATE
            POTENTIOMETER ANALOG ${ID}
        )
    endif()
    if(SLIDER)
        target_compile_definitions(${element} PRIVATE
            SLIDER ANALOG ${ID}
        )
    endif()
    if(JOYSTICK)
        target_compile_definitions(${element} PRIVATE
            JOYSTICK ANALOG DIGITAL ${ID}
        )
    endif()

    # Add the standard include files to the build
    target_include_directories(${element} INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/include
    #${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
    )

    target_sources(${element} PUBLIC
            src/module.cpp
            src/mod_utils.cpp
    )

    # Add the standard library to the build
    target_link_libraries(${element} PUBLIC
            pico_stdlib
            i2c_device
            hardware_i2c
            hardware_pwm
            pico_multicore
            pwm_pio
            hardware_gpio
    )

    pico_add_extra_outputs(${element})
endforeach(element ${TARGET})

set(TARGET )