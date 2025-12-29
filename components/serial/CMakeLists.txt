idf_component_register(SRCS "monitor.c"
                    INCLUDE_DIRS "include"
                    REQUIRES driver config main_uart adc sim io_manager accel)
