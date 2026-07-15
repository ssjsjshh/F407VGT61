cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#开关oled显示
#set(USE_SPI_OLED ON CACHE INTERNAL "")
#set(USE_I2C_OLED ON CACHE INTERNAL "")
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
#生成o2
#set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "" FORCE)
#生成o3
#set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
#生成os
set(CMAKE_BUILD_TYPE "MinSizeRel" CACHE STRING "" FORCE)

# Set the project name
set(CMAKE_PROJECT_NAME F407VGT6)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

#基础BSP外设依赖
set(BASE_SOURCES
    Bsp/exti/bsp_exti.c
    Bsp/rtc/bsp_rtc.c
    Bsp/delay/bsp_delay.c
    Bsp/soft_i2c/bsp_i2c.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_atan2_f32.c
)
#Moudle相关依赖
set(MOUDLE_SOURCES
    Moudle/mpu/moudle_mpu.c
    Moudle/oled/moudle_oled.c
    Moudle/usart/moudle_usart.c
)
#Service相关依赖
set(SERVICE_SOURCES
    Service/oled/oled_font.c
    Service/oled/service_oled.c
    Service/mpu/service_mpu.c
    Service/usart/service_usart.c
)
#APP相关依赖
set(APP_SOURCES
    App/rtc/app_rtc.c
    App/oled/app_oled.c
    App/mpu/app_mpu.c
    App/usart/app_usart.c
    App/usart/cmd_list.c
)
set(SOURCES
    ${BASE_SOURCES}
    ${SERVICE_SOURCES}
    ${MOUDLE_SOURCES}
    ${APP_SOURCES}
)

# 2. 根据开关条件追加对应OLED驱动
if(DEFINED USE_SPI_OLED)
    list(APPEND BASE_SOURCES Core/Src/spi_oled.c)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE USE_SPI_OLED)
    message(STATUS "【编译SPI OLED驱动】")
elseif(DEFINED USE_I2C_OLED)
    list(APPEND BASE_SOURCES Core/Src/i2c_oled.c)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE USE_I2C_OLED)
    message(STATUS "【编译I2C OLED驱动】")
endif()

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
     ${SOURCES}
    # Add user sources here
)


# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/DSP/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Bsp/delay
    ${CMAKE_CURRENT_SOURCE_DIR}/Bsp/exti
    ${CMAKE_CURRENT_SOURCE_DIR}/Bsp/soft_i2c
    ${CMAKE_CURRENT_SOURCE_DIR}/Bsp/rtc
    ${CMAKE_CURRENT_SOURCE_DIR}/Service/oled
    ${CMAKE_CURRENT_SOURCE_DIR}/Service/mpu
    ${CMAKE_CURRENT_SOURCE_DIR}/Service/usart
    ${CMAKE_CURRENT_SOURCE_DIR}/Moudle/mpu
    ${CMAKE_CURRENT_SOURCE_DIR}/Moudle/oled
    ${CMAKE_CURRENT_SOURCE_DIR}/Moudle/usart
    ${CMAKE_CURRENT_SOURCE_DIR}/App/oled
    ${CMAKE_CURRENT_SOURCE_DIR}/App/rtc
    ${CMAKE_CURRENT_SOURCE_DIR}/App/mpu
    ${CMAKE_CURRENT_SOURCE_DIR}/App/usart

    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    ARM_MATH_CM4
    # Add user defined symbols
)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    -Wno-unused-function
    -Wno-unused-but-set-variable
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -u _printf_float   # 开启printf %f浮点格式化
    #-u _scanf_float    # 如需scanf浮点可保留，不用可删除
)
# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
set(ELF_FILE ${CMAKE_PROJECT_NAME}.elf)
set(BIN_FILE ${CMAKE_PROJECT_NAME}.bin)
set(HEX_FILE ${CMAKE_PROJECT_NAME}.hex)

# 生成 .elf
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${ELF_FILE}
    COMMENT "Created ELF file: ${ELF_FILE}"
)

# 生成 .bin 固件
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary ${ELF_FILE} ${BIN_FILE}
    COMMENT "Created BIN firmware: ${BIN_FILE}"
)

# 生成 .hex 固件
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${ELF_FILE} ${HEX_FILE}
    COMMENT "Created HEX firmware: ${HEX_FILE}"
)

# 输出文件路径提示
add_custom_target(show_firmware ALL
    COMMAND ${CMAKE_COMMAND} -E echo "====================================="
    COMMAND ${CMAKE_COMMAND} -E echo "Output files in build dir:"
    COMMAND ${CMAKE_COMMAND} -E echo "ELF: ${CMAKE_BINARY_DIR}/${ELF_FILE}"
    COMMAND ${CMAKE_COMMAND} -E echo "BIN: ${CMAKE_BINARY_DIR}/${BIN_FILE}"
    COMMAND ${CMAKE_COMMAND} -E echo "HEX: ${CMAKE_BINARY_DIR}/${HEX_FILE}"
    COMMAND ${CMAKE_COMMAND} -E echo "====================================="
)
add_dependencies(show_firmware ${CMAKE_PROJECT_NAME})

message(STATUS "=== 当前构建类型 ===")
message(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")



