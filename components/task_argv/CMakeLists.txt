#Add sources from ui directory
file(GLOB_RECURSE SRC_UI ${CMAKE_SOURCE_DIR} "test/*.c")

idf_component_register(
    SRCS        
    #         "esp32_s3_e53.c"
    #         "esp32_s3_peripherals.c"
    #          ${SRC_VER}
               ${SRC_TEST}
    INCLUDE_DIRS "include"
    # PRIV_INCLUDE_DIRS "priv_include"
    # REQUIRES driver 
    # PRIV_REQUIRES  esp_lcd
)
