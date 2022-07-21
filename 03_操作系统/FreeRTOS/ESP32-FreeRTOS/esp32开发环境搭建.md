# ESP32开发环境

## 一、Linux开发环境搭建

> 参考[乐鑫官网-ESP-IDF-快速入门](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/get-started/)
>
> ①打开终端，在合适的目录运行以下命令：
>
> ```shell
> mkdir -p ~/esp
> cd ~/esp
> git clone https://github.com/espressif/esp-idf.git
> #官网上指定了一个v4.3.1版本，可以不指定，直接下载。
> ```
>
> ②进入/esp-idf目录下安装开发环境：
>
> ```shell
> cd ~/esp/esp-idf
> ./install.sh
> ```
>
> ③根据提示安装需要的配置，安装完成后如下图所示：
>
> ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326193329.png)
>
> ④ 在/esp-idf目录下运行
>
> ```shell
> . ./export.sh
> ```
>
> > **【提醒】**
> >
> > 在./install.sh运行过程中，如果出现Download Failure<urlopen [SSL:CERTIFICATE_VERIFY_FAILED] certificate verify failed: unable to get local issuer certificate>
> >
> > 运行`export SSL_CERT_DIR=/etc/ssl/certs`,可以把这句命令加入.baserc文件中
> >
> > 如下图所示，运行后，重新运行`.install.sh`
> >
> > ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326193334.png)



## 二、SDK上传远程仓库

> 开项目过程中，需要把代码上传到bitbucket，以便版本管控和协同开发。
>
> SDK上传过程如下：
>
> ① 拷贝一份例程到 /esp/目录下
>
> ```shell
> ~/esp$ cp -r esp-idf/examples/get-started/hello_world/ ../esp/
> ```
>
> ② 在/esp目录下,也就是 esp-idf/ 和 hello_worl/的同层目录下作为开发工程,创建git
>
> ```shell
> git init
> git submodule add https://github.com/espressif/esp-idf.git ./esp-idf  # 把原始SDK作为一个子模块
> git add --all
> git commit -m 'feat: SDK'
> #然后上传bitbucke
> ```

## 三、SDK下载

> ① 从仓库下载SDK并下载子模块
>
> ```shell
> git clone --recurse-submodules <bitbucket地址>
> #注意不要使用 git clone <bitbucket地址>
> ```
>
> ② 进入esp-idf/目录，依次运行`./install.sh` `. .export.sh` 
>
> ③ 编译sdk工程:
>
> ```shell
> cd esp/hello_world
> make clean
> make
> ```

