cmake_minimum_required(VERSION 3.16)

# Build list: app file + any sources generated under ../c (hvcc)
set(srcs "ime-embarcados-lib.cpp")
file(GLOB hvcc_c "../c/*.c")
file(GLOB hvcc_cpp "../c/*.cpp")
list(APPEND srcs ${hvcc_c} ${hvcc_cpp})

idf_component_register(
    SRCS ${srcs}
    INCLUDE_DIRS "." ".." "../c"
    REQUIRES driver esp_adc
)
