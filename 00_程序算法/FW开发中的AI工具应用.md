# FW开发中的AI工具应用

## 导读

本文框架：

- AI工具归类
- AI编程工具对比
- AI编程工具在嵌入式开发中的使用

## 工具归类

[AI工具集](https://ai-bot.cn/#term-38)是比较完整的AI工具归类网站。

软件开发可以更侧重在AI编程工具/AI聊天工具两个项。本文主要介绍下面几个工具：

- AI编程工具
  - 代码生成类：**Github Copilot**(Vscode插件) / **Cursor**(单独的IDE)
- AI聊天工具
  - 微软系：
    - ai.com (ChatGPT原生网站)
    - BingAI
  - 谷歌系：
    - [Google Bard](https://bard.google.com/)

![image-20230413102746343](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230413102746343.png)

## 工具对比

- [Github Copilot](https://github.com/features/copilot)和[Cursor](https://www.cursor.so/)的对比

  - 相同点：
    - 都是接入ChatGPT
  - 不同点：
    - Github Copilot与Vscode同属微软,通过github账号关联，在Vscode中安装Copilot插件；而Cursor是单独的一个IDE，可以在Folder中打开整个SDK。
    - Copilot在构建代码时，在SDK中，任意地方写一行注释，回车之后，Copilot会构建出响应的代码；Cursor要在某个指定的地方构建代码就需要更详细的自然语言描述。

- ai.com / BingAI / Google Bard对比

  - ai.com开放给免费用户的是GPT3.0，优点是会保存历史chat记录，不会对chat做响应限制，随便找出一个条几个月前的chat记录，能接着继续往下聊；缺点是GPT3.0对中文的响应速度还比较慢。对英文的响应速度比较快,没有给出它的参考来源。

    ![image-20230412231628204](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230412231628204.png)

    

  - BingAI给用户的平台是GPT4.0,优点是响应的数据是实时从互联网上更新的，对中文的响应能力较好，而且会给出响应内容的参考来源，用户可以根据BingAI给的参考文献进行double check（例如:当BingAI的参考来源是`百度知道`，那BingAI当次回答的准确性用户就要自己掂量掂量了）

    

  - Bard不支持中文，也没有给出回答内容的参考文献，优点：响应速度快，而且会同时给出几个回答版本给用户选择；缺点：准确性堪忧,用户需要有一定的审查能力。

    ![image-20230412232213107](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230412232213107.png)

## 工具应用

### 在AI chat工具中输入报错信息来debug



### 引导AI chat工具从0开始编码



### 使用cursor分析代码机制/API



### 使用github copilot构建代码



