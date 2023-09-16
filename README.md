<div align=center>
    <img src="./readme-resource/developing-image.png">
    <h1>VuiLib：一个开源，轻巧的 C++ 界面库</h1>
    <img src="https://img.shields.io/badge/build-passing-successimportant"/>
    <img src="https://img.shields.io/badge/license-MIT-green"/>
    <img src="https://img.shields.io/badge/version-3.x.x%20developing-orange"/>
</div>

## Installation - 安装

### Requirements - 必要条件

* 编译目标平台：x64 x32

* 操作系统： Windows Vista 及以上

* 构造工具: VS2010 及以上

* API：D2D9 及以上
  
  ### Steps - 安装过程
1. 拉取仓库代码
   
   ```
   git clone https://github.com/FSMargoo/VuiLib.git
   ```

2. 将 include 与 src 拷贝到您的项目目录中，即可开始使用 vuilib（您也可以选择自己构造静态链接库或动态链接库的 vuilib）
   
   ## Features - 特点
* 基于 D2D，编译出的程序体积极小。

* DirectX2D 渲染效率极高。

* 库文件简单，开包即用。

* 可用于写些打发时间的小程序。

* 自带一个美观的 UI 样式。

* 自带一个信号槽系统，整个 UI 由事件驱动。

* 可使用 VML 构建界面，实现前后端分离。

* 若对原样式不满意，可以使用 VSS 来自定义主题和控件样式，实现一键切换软件皮肤等操作。
  
  ## Visual - 使用 VuiLib 创造的界面
  
  <div align=center>
    <img src="./readme-resource/capture 1.png"/>
    <img src="./readme-resource/capture 2.png"/>
    <img src="./readme-resource/capture 3.png"/>
    <img src="./readme-resource/capture 4.png"/>
    <img src="./readme-resource/capture 5.png"/>
    <img src="./readme-resource/capture 6.png"/>
    <img src="./readme-resource/capture 7.png"/>
      <img src="./readme-resource/capture 8.png"/>
  </div>

## Support - 支持

可在 GitHub 中提交 issues 来取得支持和帮助

## License - 开源协议

本仓库所有源代码均使用 MIT License 开源协议

## 鸣谢
<div align="center">
<img src="https://resources.jetbrains.com/storage/products/company/brand/logos/jb_beam.svg?_ga=2.131997191.943128415.1691401327-1601569045.1689673917&_gac=1.159309000.1691401337.Cj0KCQjwrMKmBhCJARIsAHuEAPR50zMvz61eq9bA3ZpA4DBGLt6-p1UXyvcLhA1YTppAdwkt5KsaiH0aAuFNEALw_wcB&_gl=1*1x95f8*_ga*MTYwMTU2OTA0NS4xNjg5NjczOTE3*_ga_9J976DJZ68*MTY5MTU2OTI4OS4xOC4xLjE2OTE1Njk3OTQuNTIuMC4w"/>

感谢 JetBrains 提供的 [OpenSource 许可证](https://jb.gg/OpenSourceSupport)助力 VUILib 的开发。
</div>

## NewFeatures - 新版本 2.0.x beta 新特性

* 对于 vml 和 vss 语法进行调整，使得其能够更加优雅地构造界面
* 重写部分 API，理论上效率提升
* 使用了 Direct2D Render，渲染效率较 GDI+ 大幅提升