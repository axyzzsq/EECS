# 第10讲-Ubuntu下压缩与解压缩

> Linux下常用的压缩扩展名：tar、tar.bz2、tar.gz
>
> > ## 1、gzip工具
> >
> > gzip工具负责压缩和解压缩 .gz格式的压缩包
> >
> > ```shell
> > gzip xxx  #压缩
> > 
> > gzip -d xxx.gz #解压缩
> > ```
> >
> > > gizp对文件夹进行压缩
> > >
> > > ```shell
> > > gzip -r xxx  //对文件夹进行压缩
> > > 
> > > gzip -rd xxx.gz //对文件夹进行解压缩
> > > ```
> >
> > gzip虽然对文件夹进行压缩，但是并不能提供打包服务，只是对文件夹中的所有文件进行了单独的压缩。
> >
> > 
> >
> > ## 2、bzip2工具
> >
> > ```shell
> > bzip2 -z xxx #压缩
> > 
> > bzip2 -d xxx.bz2  #解压缩
> > ```
> >
> > 和gzip一样，bzip2不支持对整个文件夹进行打包
> >
> >  
> >
> > ## **3、tar工具**
> >
> > ```shell
> > tar -zcvf xxx/   xxx.tar   #-c表示创建新的归档 -z表示gzip压缩,z替换为j表示用bz2格式压缩
> > tar -zxvf xxx.tar.gz  #-x从归档中解压缩
> > ```
> >
> >  
> >
> > ## 4、其他格式
> >
> > ### ① rar格式
> >
> > ```shell
> > rar x xxx.rar     # 解压
> > rar a  xxx.rar xxx/ # 压缩
> > ```
> >
> > ### ②zip格式
> >
> > ```shell
> > zip -rv  xxx.zip  xxx/  #压缩  -v打印过程
> > unzip -v  xxx.zip  #解压
> > ```

