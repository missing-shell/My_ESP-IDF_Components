# 注册组件
idf_component_register(
    SRCS        
                        "src/mult_paramenters.c"
                        "src/mytask_list.c"
                        "src/single_paramenters.c"
                        ${SRC_VER}
    INCLUDE_DIRS        "include"
    REQUIRES            driver
    
)
