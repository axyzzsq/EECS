# librws-websocket-ssl
## 证书解析
>  `openssl pkcs12 -in example.com.pfx -nokeys -out certificate.certificate`
> 再输入密码获取证书文件

## 证书内容
> 解析完的证书会有多个certificate,各个cert的CN字段和证书字段不同,后面的cert与前面的cert存在父级的关系。
