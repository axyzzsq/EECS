# iMx6Ull开发环境搭建

## 1、源码下载

- 配置git信息

  ```shell
  git config --global user.email "user@100ask.com"
  git config --global user.name "100ask"
  #查看是否配置成功
  git config --list
  ```

- 依次执行以下四条命令下载BSP

  ```shell
  git clone https://e.coding.net/codebug8/repo.git
  ```

  ```shell
  mkdir -p 100ask_imx6ull-sdk && cd 100ask_imx6ull-sdk
  ```

  ```shell
  ../repo/repo init -u https://gitee.com/weidongshan/manifests.git -b linux-sdk -m imx6ull/100ask_imx6ull_linux4.9.88_release.xml --no-repo-verify
  ```

  ```shell
  ../repo/repo sync -j4
  ```

   ![image-20220927133257499](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220927133257499.png)

- 更新源码的方式

  进入`~/100ask_imx6ull-sdk`

  ```shell
  cd ~/100ask_imx6ull-sdk
  ```

  执行：

  ```shell
  ../repo/repo sync -c
  ```

