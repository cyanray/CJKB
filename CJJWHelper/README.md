# 项目简介
重交大教务系统爬虫的 C++ 实现，方便开发各种小程序。
# 项目依赖
1. **libcurl** 实现模拟表单提交。

2. **libjpeg-turbo** 读取登录验证码，使得数据可以提供给 HamOCR 进行处理。

3. **libicov** 编码转换，转换网页的编码为目标编码。

4. [**CURLWrapper**](https://github.com/cyanray/CURLWrapper) 对 **libcurl** 的封装。

5. [**HTMLReader**](https://github.com/cyanray/HTMLReader) HTML 文档解析器，用于在网页中读取数据。

6. [**HamOCR**](https://github.com/cyanray/HamOCR) 基于汉明距离的 OCR 识别实验项目，用于识别登录验证码。


