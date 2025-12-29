idf_component_register(SRCS "main.c"
                    PRIV_REQUIRES spi_flash
                    INCLUDE_DIRS ""
                    REQUIRES pwrmanage main_uart serial gnss_uart adc io_manager accel)
