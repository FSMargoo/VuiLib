<div align=center>
    <img src="../src/logo.png"/>
    <h1>VuiLib[VSS&VML] 快速上手：使用 VSS+VML 快速构建全新完全布局</h1>
</div>

> 该部分将会通过 VuiLib 中的 VSS 快速构建一个软件的布局
> 
> 本教程的完整 VSS&VML 源码可以在 doc/code/vss-start 找到

## 第一步：认识 VSS
VSS 是一个由 CSS 阉割而来的样式表语言，CSS 的一个基础语法如下：
```css
/* 一个基础选择器 */
my-element {
    color: rgb(0, 0, 0);
    background-color: rgb(255, 255, 255);
}
/* 类选择器 */
.my-class {
    color: rgb(255, 255, 255);
    background-color: rgb(0, 0, 0);
}
```
## 第二步：VSS 和 VML 结合开发界面
VSS 支持小部分的 W3C 标准，且需要 VML 层调用才可以应用，使用方法如下：
```XML
<!-- 从文件引用 VSS -->
<tag style-sheet-src="./file.css">
</tag>
<!-- 直接使用字符 VSS -->
<tag style-sheet="element { property: 8px; }">
</tag>
```
并且，如果要在 VSS 中使用类选择器或 ID 选择器需要在 VML 中定义相关的属性例如：
```XML
<tag class="class-name">
</tag>
<tag id="id-name">
</tag>
```
## 第三步：开始编写你的 VSS 代码
1. 编写 VSS 代码之前，需要先写好一个引用 VSS 的 VML 文件，这里不多赘述 VML 的构造方式，直接贴出 VML 代码
    ```XML
    <widget width="640" height="480" type="mainwindow" style-sheet-src="./vss.css" class="widget">
        <label type="textlabel" width="@scale(0.8)" height="@scale(0.35)" x="@center" y="@center" class="test-label"
               text="VUILIB STYLE SHEET DEMO\nHELLO VUILIB">
        </label>
    </widget>
    ```
2. 对于窗口样式的定义

    VSS 中，mainwindow 拥有一个属性：background-color，以设定窗口的背景色，所以如果我们想定义窗口的背景色，可以这么写：
    ```css
    /* 类选择器 */
    .widget {
        /* 设置背景颜色为 rgb(30, 30, 30) */
        background-color: rgb(30, 30, 30);
    }
    ```

    然后把你的 VML 文件拖入 vmlviewer_x64.exe 中，预览发现窗口的背景色已经变成 rgb(30, 30, 30) 了。
3. 对于文本框样式的定义
   
   VSS 中，textlabel 可以定义的属性略多，这里也步多做赘述，直接上 VSS 源码
   ```css
    .test-label {
        /* 设置背景色 */
        background-color: rgb(255, 255, 255);
        /* 设置文字颜色 */
        color: rgb(30, 30, 30);

        /* 设置文字居中对齐 */
        text-align: center;
        vertical-align: center;
    }
   ```
## 第四步：预览
把你的 VML 拖入 vmlviewer_x64.exe 中，就可以看到你的 VML+VSS 的最终实际效果了。