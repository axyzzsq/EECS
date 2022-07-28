# 大小写字母转换

```C
    for (int i = 0;  MacAddress[i] != '\0'; i++ )
	{
		if ((MacAddress[i] >= 'a') && (MacAddress[i] <= 'z'))//寻找小写字母
		{
			MacAddress[i] = MacAddress[i] - 32;//根据ASCALL码将小写字母转化为大写字母
        }
	}
```



