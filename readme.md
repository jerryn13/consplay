# Consplay

## 简介

Consplay是一个可以在Windows命令行中创建、管理和游玩基于AI对话的文字游戏的平台。

使用“创意工坊”功能，你可以自由安装和创建你的游戏“卡带”。

## Credits

设计&程序：nyy

## 预置游戏

1.海龟汤

2.俺不是孬种

3.你要不要（命）吧

4.假如全世界计算机水平倒退1000倍

## 正在制作中的游戏

5.三句话，让阿姨给我多打了两勺菜（借鉴于豆包社区）

6.哼

7.小张！

## LLM配置和游戏卡带管理

在使用前，您需要参照`/userdata/env.example.ini`的指示填写`/userdata/env.ini`配置文件内容，以设置LLM模型的API地址、模型名称和访问密钥。

本游戏的科技协会大作业分发版本中已预先配置了一个豆包大模型的API配置以供测试体验，其中包含访问密钥，请勿外泄！且该密钥额度有限，超额使用会自动吊销，若对话无反应或需要进一步分发可以联系nyy获取新的密钥，或是自己配置一个兼容OpenAI ChatCompletion API格式的LLM API。

你可以增改`/userdata/games`目录下的cg格式卡带文件来安装或优化游戏。

## 项目代码结构

`dependency`：封装了一些常用依赖库，如网络请求、字符串操作、文本文件读写等

`app`：项目主要实现代码

`test` ：测试用例代码

`assets`：游戏资源包

`userdata`：用户数据，包含游戏卡带、存档、配置文件

## 第三方库使用情况

`cJSON`：用于在网络请求中创建和解析JSON数据

`winhttp`：Windows的网络请求库

## TODOs

0.加入聊天记录存档

1.加入数据存档系统

2.直接在命令行中设计、编辑游戏

3.联网的创意工坊

4.可以打断游戏过程，回到主菜单重新选择游戏

## 提建议/反馈BUG/游戏投稿/获取更新

（小声BB：欢迎Star）

https://github.com/jerryn13/consplay

## 声明

本项目部分代码由ChatGPT编写，约25%，主要为`dependency/fetch` 对winhttp的封装。

`dependency/string`中文本编码转换代码来源于[[C语言]字符串处理 - ANSI - Unicode - UTF8 转换_c语言unicode和utf-8互转-CSDN博客](https://blog.csdn.net/sime507/article/details/8587439)

## 注意

由于这个项目开发得比较匆忙，安全防范和异常处理方面不严谨，存在缓冲区溢出等漏洞。请不要安装来自未知来源的游戏卡带，也不要使用未经验证的第三方大模型API。

## 编译与运行环境

仅支持Windows系统。使用GCC在Windows11系统下编译，编译时需要加上`-lwinhttp`参数。