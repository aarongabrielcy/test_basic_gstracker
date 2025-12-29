idf_component_register(
    SRCS "sim7000.c"
    INCLUDE_DIRS "include"
    REQUIRES driver main_uart serial
)