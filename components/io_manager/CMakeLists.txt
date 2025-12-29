idf_component_register(SRCS "input.c" "output.c"
                    INCLUDE_DIRS "include"
                    REQUIRES driver config serial)