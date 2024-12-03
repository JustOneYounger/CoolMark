# CoolMark🔥🔥🔥

**一款小型MarkDown编辑器，使用`Monokai`风格制作**<br>
**编辑`markdown`和`css`时可实时刷新用于显示的html内容**

编写配置:

|QT|QT6.5.3 msvc2019_64|
|--|--|
|Visual Studio|Visual Studio 2022|

## 使用教程

* 1.新建一个文件夹,将 `CoolMark.exe`, `python`, `coolmark_style.css`, `markdown.ico` 放到该文件夹内

  形成如下结构样式
  
  ```
  - 根目录文件夹
    - python
      - MarkDownToHtml.py
    - coolmark_style.css
    - markdown.ico
    - CoolMark.exe
  ```

* 2.在python文件夹内,打开终端,使用 `pyinstaller` 编译 `MarkDownToHtml.py` 文件

  ```besh
  pip install pyinstaller
  pyinstaller MarkDownToHtml.py
  ```

* 3.使用QT的MSVC打包工具,QT 6.5.3 (MSVC 2019 64-bit)进行打包

  ```besh
  cd 存放CoolMark.exe的文件夹位置
  windeployqt CoolMark.exe
  ```

## 介绍

markdown转义为html依靠python的`markdown`库实现,且加入了`mdx_math`使其可以解析数学公式

在主程序中对于`html`文件的<head>部分,引入
```html
<script type="text/x-mathjax-config">MathJax.Hub.Config({config:["MMLorHTML.js"],jax:["input/TeX","output/HTML-CSS","output/NativeMML"],extensions:["MathMenu.js","MathZoom.js"]});</script>
<script type="text/javascript" src="https://cdn.staticfile.org/mathjax/2.7.7/MathJax.js"></script>
```
用于解析渲染$...$或者$$...$$的数学公式标签

效果展示:

![数学公式效果图](images/math.jpg)
