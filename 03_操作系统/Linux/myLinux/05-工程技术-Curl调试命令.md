# curl指令

curl是一个命令行工具，用于发送HTTP请求并获取HTTP响应。curl支持多种协议，包括HTTP、HTTPS、FTP、SMTP等。

curl的基本用法是`curl [options] [URL]`，其中URL是要访问的URL地址。

常用的用法如下：

- -X：指定HTTP请求方法；

  ```shell
  # 以POST方式提交http://www.example.com/
  curl -X POST http://www.example.com/
  ```

- -H：指定HTTP请求头；

  ```shell
  # 以POST方式提交JSON数据到http://www.example.com/
  curl -H “Content-Type: application/json” -d ‘{“name”: “example”}’ http://www.example.com/
  ```

- -d：指定HTTP请求体；

  ```shell
  # 以POST方式提交表单数据到http://www.example.com/。其中，-d选项后面的字符串是要提交的数据，多个数据之间用&符号分隔。
  curl -d “name=example&age=18” http://www.example.com/
  ```

- -o：指定输出文件；

  ```shell
  # 将http://www.example.com/的内容保存到example.html文件中
  curl -o example.html http://www.example.com
  ```

- -O：将输出保存到文件中；

  ```shell
  # 将http://www.example.com/example.jpg保存到当前目录下的example.jpg文件中
  curl -O http://www.example.com/example.jpg
  ```

- -L：跟随重定向；

  ```shell
  # 跟随重定向获取http://www.example.com/redirect的内容
  curl -L http://www.example.com/redirect
  ```

- -v：显示详细的调试信息

  ```shell
  curl -v http://www.example.com/
  ```
