# ESP-IDF 安装

## 参考

- [安装](https://www.bilibili.com/video/BV1ah4y177mR/?spm_id_from=333.788.top_right_bar_window_custom_collection.content.click&vd_source=0aba2f1927cca4db51a7dad74151f079)
- [入门](https://www.bilibili.com/video/BV1nR4y1o7VE/?spm_id_from=333.788.top_right_bar_window_custom_collection.content.click&vd_source=0aba2f1927cca4db51a7dad74151f079)

不建议参考[安装](https://www.bilibili.com/video/BV1ah4y177mR/?spm_id_from=333.788.top_right_bar_window_custom_collection.content.click&vd_source=0aba2f1927cca4db51a7dad74151f079)中设置项目`project`文件夹的方式，推荐放在非插件文件目录下，方便插件的卸载更新和项目代码的保留

网络参考步骤：

- 首先下载esp-idf(本人未采用)
- 设置用户环境变量(本人未采用)
- 安装vscode
- 安装esp-idf插件

## 非必要选项

### 在Windows环境变量中设置PATH(安装插件后)

IDF_PATH

```C
e:\Espressif\v5.1.1\esp-idf
```

IDF_TOOLS_PATH

```C
e:\Espressif\v5.1.1\.espressif
```

### vscode中出现不影响编译的报错

目前实践成功的解决方法

- 选择esp-idf框架中的编译器，而不是`mingw`之类
- 确保vscode中esp-idf的环境变量正确
- 手动设置系统环境变量
- 更新setting.json
