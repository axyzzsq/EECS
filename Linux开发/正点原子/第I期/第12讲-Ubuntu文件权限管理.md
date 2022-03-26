# 第12讲-Ubuntu文件权限管理

## 一、Ubuntu/Linux文件权限

`ls -l` 查看文件，一共十位来表示文件类型和权限。

 ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326192728.png)

在/dev目录下，b开头的指的是块设备，c开头的指的是字符设备。

第一个字母表示的是文件的类型。

后面九个字母依次表示用户权限，用户组内成员权限，组外系统内其他成员的权限。

 ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326192748.png)

-rwxrwxrwx 就表示系统内成员都有读写执行权限

chmod命令加权限就是根据文件的用户权限和分组设定的。每个组rwx都是用421来按照位次赋权。



## 二、Ubuntu/Linux文件权限修改

### 1、修改文件权限

```shell
chmod 777 filename  #对单个文件赋权
chmod -R  777 ./foldername  #对文件夹及其附属子文件夹赋权
```

也可以一次只对一个所属类别用户进行文件权限设置

 ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326192806.png)

### 2、修改文件所属用户

```shell
sudo chown user1 filename  #将filename文件的所属者指定为user1
sudo chown .group filename #将filename文件的所属用户组指定为group,注意命令中的点号用来指定用户组
sudo chown user.group filename #将filename文件所属用户改成user，所属组改成group
sudo chown user:group filename #用分号和用点号有同样的效果
sudo chown -R user.group foldername #文件夹及其下层附属文件修改文件所属者
```

 ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326192822.png)

