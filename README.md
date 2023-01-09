## Qt 项目结构

```
├─appgitlvvcdecoder           VVC解码器
│  ├─VTM_16.0
│  │  ├─CommonLib
│  │  ├─DecoderApp
│  │  ├─DecoderLib
│  │  ├─Utilities
│  │  ├─SysuAnalyzerLib      VTM 16.0拓展库，完成编码信息输出
├─libgitlmvc                 MVC框架库
├─libplugins                 绘制项插件
├─src
│  ├─Forms                   UI模板
│  ├─Resources               图片等资源文件
│  ├─commands                控制器调用的各个命令
│  ├─exceptions
│  ├─model
│  │  ├─common               CU、PU、MV等数据结构
│  │  ├─drawengine           插件加载和绘制接口
│  │  ├─io                   YUV转RGB
│  ├─parsers                 解析解码器输出的文件
│  ├─views                   视图
│  ├─main.cpp                程序入口
```

## 代码下载

```shell
git clone --recursive https://github.com/fanghaocong/VVCBitstreamAnalyzer
```

## Qt 环境搭建（Qt 5.15.2）

### Windows

1. 安装 Qt & Qt Creator（https://www.qt.io/download-open-source）：
   + 运行安装程序，在安装组件时选择 Qt - Qt 5.15.2 - MinGW 8.1.0 64-bit；
2. 下载 ffmpeg（https://github.com/BtbN/FFmpeg-Builds/releases）：
   + 选择 win64-gpl-shared 版本进行下载；
   + 将其中的 include 和 lib 两个文件夹添加到项目目录下的 ffmpeg 文件夹下（需要新建 ffmpeg 文件夹）；
   + 将其中的 bin 目录添加到环境变量 Path 中，或将 bin 目录下的 avutil-xx.dll 和 swscale-x.dll 两个动态库添加到编译输出目录下（即可执行文件所在的目录）；
3. 编译运行。

### Linux

1. 安装 Qt5（如果只需编译的话使用 Qt 即可，进行开发则需安装 Qt Creator）：

   ```shell
   sudo apt-get install qt5-default
   ```

2. 下载 ffmpeg（https://github.com/BtbN/FFmpeg-Builds/releases）：

   + 选择 linux64-gpl-shared 版本进行下载；

   + 将其中的 include 文件夹和 lib 文件夹复制到项目目录下的 ffmpeg 文件夹下（需要新建 ffmpeg 文件夹）；

3. 编译运行：

   ```shell
   qmake VVCBitstreamAnalyzer.pro -r "CONFIG+=Release"
   make
   ./VVC_Bitstream_Analyzer 
   ```

### Mac OS

1. 安装Qt5：

   ```
   brew install qt@5
   // 按照安装完成的提示配置环境变量
   echo 'export PATH="/opt/homebrew/opt/qt@5/bin:$PATH"' >> /Users/YourUserName/.bash_profile
   export LDFLAGS="-L/opt/homebrew/opt/qt@5/lib"
   export CPPFLAGS="-I/opt/homebrew/opt/qt@5/include"
   export PKG_CONFIG_PATH="/opt/homebrew/opt/qt@5/lib/pkgconfig"
   ```

2. 编译ffmpeg：

   ```
   // 任意进入一个文件夹（e.g. 文稿）
   mkdir ffmpeg
   wget http://ffmpeg.org/releases/ffmpeg-4.2.1.tar.gz
   tar -xzvf ffmpeg-4.2.1.tar.gz
   cd ffmpeg-4.2.1
   ./configure --enable-shared --prefix=/usr/local/ffmpeg
   make -j8 && sudo make install
   ```

3. 将 ffmpeg 编译产物添加到工程中：

   将 `/usr/local/ffmpeg/include` 和 `/usr/local/ffmpeg/lib` 两个文件夹复制到项目目录下的 ffmpeg 文件夹下（需要新建 ffmpeg 文件夹）

4. 编译运行

   ```
   qmake VVCBitstreamAnalyzer.pro -r "CONFIG+=Release"
   make
   ./VVC_Bitstream_Analyzer
   ```

   

