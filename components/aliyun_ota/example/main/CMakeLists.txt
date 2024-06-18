idf_component_register(SRCS 
                           "../../src/aliyun_ota.c" 
                           "../../src/aliyun_ota_json.c"
                           "app_aliyun_ota.c"
                           "example_main.c"
                    INCLUDE_DIRS 
                            "."
                            "../../include" 
                 EMBED_TXTFILES ${project_dir}/server_certs/root.crt
)