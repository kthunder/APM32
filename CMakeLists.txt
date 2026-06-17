cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
get_filename_component(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME) 
set(CMAKE_PROJECT_NAME ${PROJECT_DIR})

# Include toolchain file
include(".cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# 彩色诊断输出 - 完整配置
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always)
    add_link_options(-fdiagnostics-color=always)
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-fcolor-diagnostics)
endif()
# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Common compile definitions
add_compile_definitions(APM32F407xx)
add_compile_definitions(APM32F40X)
add_compile_definitions(USE_DAL_DRIVER)
add_compile_definitions(HSE_VALUE=12000000U)

# Add shared libraries
# add_subdirectory(.cmake/stm32cubemx)
add_subdirectory(.cmake/app)
add_subdirectory(.cmake/CherryUSB)
add_subdirectory(.cmake/CherryDAP)

# ==================== Boot Firmware ====================
add_executable(boot
    ${CMAKE_SOURCE_DIR}/Core/Source/boot/boot_main.c
    ${CMAKE_SOURCE_DIR}/Core/Source/boot/msc_ram_template.c
)
set_target_properties(boot PROPERTIES SUFFIX ".elf")

target_link_options(boot PRIVATE
    -T${CMAKE_SOURCE_DIR}/Core/Source/boot/apm32f407xg_flash.ld
)

target_compile_definitions(boot PRIVATE BOOT_FW)

target_link_libraries(boot
    app
    CherryUSB
    CherryDAP
)

set(BOOT_HEX_FILE ${PROJECT_BINARY_DIR}/boot.hex)
set(BOOT_BIN_FILE ${PROJECT_BINARY_DIR}/boot.bin)
set(BOOT_DIS_FILE ${PROJECT_BINARY_DIR}/boot.dis)
set(BOOT_LST_FILE ${PROJECT_BINARY_DIR}/boot.lst)

add_custom_command(TARGET boot POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:boot> ${BOOT_HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:boot> ${BOOT_BIN_FILE}
    COMMAND ${CMAKE_OBJDUMP} -d $<TARGET_FILE:boot> > ${BOOT_DIS_FILE}
    COMMAND ${CMAKE_OBJDUMP} --all-headers --demangle --disassemble $<TARGET_FILE:boot> > ${BOOT_LST_FILE}
    COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:boot>
    COMMENT "Building boot firmware completed"
)

# ==================== App Firmware ====================
add_executable(app_fw
    ${CMAKE_SOURCE_DIR}/Core/Source/app/app_main.c
)
set_target_properties(app_fw PROPERTIES SUFFIX ".elf")

target_link_options(app_fw PRIVATE
    -T${CMAKE_SOURCE_DIR}/Core/Source/app/apm32f407xg_flash.ld
)

target_compile_definitions(app_fw PRIVATE APP_FW)

target_link_libraries(app_fw
    app
    CherryUSB
    CherryDAP
)

set(APP_HEX_FILE ${PROJECT_BINARY_DIR}/app.hex)
set(APP_BIN_FILE ${PROJECT_BINARY_DIR}/app.bin)
set(APP_DIS_FILE ${PROJECT_BINARY_DIR}/app.dis)
set(APP_LST_FILE ${PROJECT_BINARY_DIR}/app.lst)

add_custom_command(TARGET app_fw POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:app_fw> ${APP_HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:app_fw> ${APP_BIN_FILE}
    COMMAND ${CMAKE_OBJDUMP} -d $<TARGET_FILE:app_fw> > ${APP_DIS_FILE}
    COMMAND ${CMAKE_OBJDUMP} --all-headers --demangle --disassemble $<TARGET_FILE:app_fw> > ${APP_LST_FILE}
    COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:app_fw>
    COMMENT "Building app firmware completed"
)

# Combined target to build both
add_custom_target(all_firmware DEPENDS boot app_fw)
