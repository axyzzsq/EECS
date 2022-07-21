# ESP32 NVS使用方法

非易失性存储 (NVS) 库主要用于在 flash 中存储键值格式的数据。

NVS空间创建后写入数据，关闭之后，NVS命名空间随之销毁，但是键值对保存在Flash中，创建新的非同名NVS空间，只要目标键在Flash空间中存在，键对应的值就可以被读取出来。NVS命名空间只是访问时的一个临时入口。

比如：创建了一个叫做WiFiMsg的NVS空间，存在两个键，分别是ssid和passwd，这两个键的值分别是Wi-Fi账号和密码。关闭之后再创建一个WiFiMsg2的空间，依旧可以读取到ssid和passwd的数据。

如果要更新Flash中存储的Wi-Fi信息，只需要打开创建一个新的NVS空间，把对应键的值重新写入，即可修改Flash数据。

NVS文件系统对用户屏蔽了数据在Flash中的具体位置，操作起来比较简便。但是也存在一个风险，当设备流入终端用户手中，此时无法对存有用户数据的NVS空间进行分区修改，会擦除原有的用户数据，可能包括和服务器通信的设备license。

## 一、ESP32 NVS写操作

### ①流程图

 ![image-20220627162740502](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220627162740502.png)

### ②示例代码

```C
static void mcWriteMacAdr(int argc, char **argv)
{
    close_websocket_client();
    ESP_LOGI(TAG,"mcWriteMacAdr函数,argv[0]:%s,argv[1]:%s",argv[0],argv[1]);
    char readMacAddress[30]={0};
    size_t read_len = sizeof(readMacAddress);
    char write_addr[30] = {0};
    memcpy(write_addr,argv[1],strlen(argv[1]));
    if(strlen(argv[1]) == 12)
    {
        //写nvs
        nvs_handle mac_nvs_handler; 
        esp_err_t err = nvs_open("staMac", NVS_READWRITE, &mac_nvs_handler);
        if (err != ESP_OK)
        {
            nvs_close(mac_nvs_handler);
            printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        }
        else
        {
            ESP_LOGI(TAG,"Done\n");
            ESP_LOGI(TAG,"写入数据: %s",write_addr);
            err = nvs_set_str(mac_nvs_handler,"macAddr",write_addr);
            if (err != ESP_OK)
            {
                nvs_close(mac_nvs_handler);
                ESP_LOGE(TAG,"nvs_set_str err = %d\n",err);
                printf("mac_rw.0\r\n");
                return;
            }
            else
            {
                printf("mac_rw.1\r\n");
                err = nvs_get_str(mac_nvs_handler,"macAddr",readMacAddress,&read_len);
                if(err != ESP_OK)
                {
                    nvs_close(mac_nvs_handler);
                    ESP_LOGE(TAG,"读取mac地址失败2");
                }
                else
                {
                    //mac地址读取成功
                    ESP_LOGI(TAG,"读出原始数据: %s",readMacAddress);
                }
            }
            err = nvs_commit(mac_nvs_handler);
        }
        nvs_close(mac_nvs_handler);
    }
    else{
        printf("mac_rw.0\r\n");
        ESP_LOGI(TAG,"mac地址错误");
    }
}
```





## 二、ESP32 NVS读操作

### ①流程图

 ![image-20220627160653464](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220627160653464.png)



### ②示例代码

```C
    nvs_handle wificfg_nvs_handler; /* 定义一个NVS操作句柄 */
    char wifi_ssid[33] = { 0 };     /* 定义一个数组用来存储ssid*/
    char wifi_passwd[65] = { 0 };   /* 定义一个数组用来存储passwd */
    size_t ssid_len;
    size_t passwd_len;
    /* 打开一个NVS命名空间 */
    // ESP_LOGI(TAG,"打开一个NVS命名空间");
    esp_err_t err = nvs_open("WiFi_cfg", NVS_READWRITE, &wificfg_nvs_handler);
    if(err != ESP_OK)
    {
        return;
    }
    /* 从NVS中获取ssid */
    // ESP_LOGI(TAG,"从NVS中获取ssid");
    ssid_len = sizeof(wifi_ssid);
    err = nvs_get_str(wificfg_nvs_handler,"wifi_ssid",wifi_ssid,&ssid_len);
    if(err != ESP_OK)
    {
        return;
    }
    /* 从NVS中获取ssid */
    passwd_len = sizeof(wifi_passwd);
    err = nvs_get_str(wificfg_nvs_handler,"wifi_passwd",wifi_passwd,&passwd_len);
    if(err != ESP_OK)
    {
        return;
    }
    nvs_close(wificfg_nvs_handler); //关闭NVS空间
    /* 设置WiFi连接的ssid和password参数 */
    ESP_LOGI(TAG,"nvs-ssid:%s,passwd:%s",wifi_ssid,wifi_passwd);
```



