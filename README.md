# Air Sensor Arduino
这是一个使用 `PlatformIO + Arduino` 开发的运行在外接了 `AHT10` 传感器的 `ESP32 C3` 开发板
上的程序。它会不断从传感器中读温湿度数据，然后写到蓝牙服务器的通用属性中以供客户端来消费。[这个项目](https://github.com/camelbuba/airsensor)
是与它配套来使用的安卓客户端，可以在手机上查看温湿度数据。

## 开发环境搭建
1. 使用 `pip install platformio` 安装 PlatformIO 的sdk
2. 记得更新`platformio`到最新版本
3. 在项目根目录执行 `pio run -t upload` 用来构建上传代码到开发板
4. 在 CLion 中安装 PlatformIO 插件，新建一个 PlatformIO模板的项目，则可以正确跳转
5. 或者在 VSCode 中安装 PlatformIO 插件，也可以完成代码的正确跳转，但这条路径在国内基本不可行，因为链接platformio官网资源的网络太慢