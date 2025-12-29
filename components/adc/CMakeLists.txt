idf_component_register(
    SRCS "adc.c"
    INCLUDE_DIRS "include"
    REQUIRES driver config esp_adc
)