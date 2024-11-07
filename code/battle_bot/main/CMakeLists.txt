idf_component_register(SRCS "main.c" "i2c_esp.c" "mcf8316a.c"
                    INCLUDE_DIRS "." "includes"
                    PRIV_REQUIRES driver)