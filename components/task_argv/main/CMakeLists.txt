SET(SOURCES 
                "main.c"
                "mytask_list.c"
                "single_paramenters.c"
                "mult_paramenters.c"
)

include_directories(include)

add_library(TEST ${SOURCES})