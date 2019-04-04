# toyrenderer

参考大神的[仓库](https://github.com/ssloy/tinyrenderer/tree/1cce85258d1f1cf75fd10fe4d62ebfdb669f8cf9)，完成了自己心心念的一个轮子——「渲染器」。



![](https://github.com/TensShinet/toyrenderer/blob/master/image/final1.png?raw=true)



![没有画眼睛](https://github.com/TensShinet/toyrenderer/blob/master/image/final.png?raw=true)







## 介绍

完全参考大神的[仓库](https://github.com/ssloy/tinyrenderer/tree/1cce85258d1f1cf75fd10fe4d62ebfdb669f8cf9)，**用 c++ 从底层去学习渲染器的原理，去理解 OpenGL 到底是怎么工作的**。整体涉及的算法并不是很难。

+ 一些简单的图形学算法，包括但不限于「画直线」、「画三角形」、「漫反射」、「镜面反射」
+ 一些基础的线性代数

大神的[仓库](https://github.com/ssloy/tinyrenderer/tree/1cce85258d1f1cf75fd10fe4d62ebfdb669f8cf9)很基础地讲解了这些内容。我在学习了一段时间后，参考大神的代码，写出自己的玩具渲染器。大致如下

![](https://github.com/TensShinet/toyrenderer/blob/master/image/toyrenderer.png?raw=true)





`code` 文件夹里面放着我的代码。`image`文件夹里面放着一些过程图片。`obj`文件夹里面放着模型

## 学习

学习的过程十分艰辛，没有 c++ 的基础，没有图形学的基础，这导致中间有一段时间很想放弃。但是最终还是慢慢熬了过来。英语底子并不好，有时候为了静下心来看文章，只能把文章边读边翻译。



学习完一章以后，也会写一些博客。



**以下内容是我写过的博客，希望能对你看大神的仓库、学习图形学有帮助**。



+ [Bresenham's line algorithm](https://www.jianshu.com/p/fc237110d9d4)
+ [填充三角形的光栅化](https://www.jianshu.com/p/d80182fa8ffd)
+ [平行光照算法](https://www.jianshu.com/p/f9ccf2ec4ceb)
+ [3d 到 2d 的转换](https://www.jianshu.com/p/9a4d89ba19c5)
+ [透视投影](https://www.jianshu.com/p/61cdcba18832)
+ [移动相机](https://www.jianshu.com/p/e2fc2f52ce91)
+ [Matrices 矩阵](https://www.jianshu.com/p/9f8422b5bf5f)
+ [为渲染器添加着色器（ Shaders for the software renderer ）](https://www.jianshu.com/p/b54e8971f715)
+ [切线空间法线贴图（tangent space normal mapping）](https://www.jianshu.com/p/a41f4e06aa00)
+ [纹理贴图、法线贴图、切线空间法线贴图](https://www.jianshu.com/p/5bdb7bbdfa6c)
+ [阴影贴图（Shdow Mapping）](https://www.jianshu.com/p/a561c0b1565f)



## 最后

欣赏一下我在实现「渲染器」踩过的[坑]()吧！



扫个码我们做朋友吧！

![](https://github.com/TensShinet/learn_statistical-learning-method/blob/master/images/wx.png?raw=true)







