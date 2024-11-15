idf_component_register(SRCS "pwm.c" "adc.c" "main.c" "i2c_esp.c" "mcf8316a.c"
                    INCLUDE_DIRS "." "includes"
                    PRIV_REQUIRES driver)