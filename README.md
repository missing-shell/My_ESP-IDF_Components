# 私有组件

## 项目结构

```tree
.
├── CMakeLists.txt
├── main
│   ├── CMakeLists.txt
│   └── main.c 

├── components
    |
    task_list
       ├── CMakeLists.txt
       ├── include
       │   └── mult_paramenters.h
       │   └── mytask_list.h
       |   └── single_paramenters.h
       └── src
            └── mult_paramenters.c
            └── mytask_list.c
            └── single_paramenters.c
            
```

## 使用方式

- 使用时，只需要将对应的组件放置在项目的components目录下，当然也可以直接使用本项目的components。
- 本项目中的 components 目录只是作为一个通用的组件容器，关于组件的设置请查看对应组件的`README.md`文件。
- 不需要其他操作例如`add_subdirectory(components/task_list)  # 添加 task_list 目录`
