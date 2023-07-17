# Linked List & Wi-Fi Cookie

## Requirement

设备端的Flash保存最近曾经连接过的5个路由器的信息(SSID/PSWD)

## Implementation

链表的一个元素(结构体)用来保存一个Wi-Fi的信息，如果是新的Wi-Fi采用头插法,作为第一个元素;

如果输入的Wi-Fi是链表中原来就有的,则把这个元素从链表中移动到第一个元素;

超过5个元素则删除链表尾的元素。

## Code

```cpp
#include "pch.h"
#include <iostream>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace winrt;
using namespace Windows::Foundation;
using namespace std;

#define MAX_SSID_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define MAX_WIFI_COUNT 9

typedef struct WifiInfo 
{
    char ssid[MAX_SSID_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int position;
    struct WifiInfo* next;
} WifiInfo;

WifiInfo* createWifiInfo(char ssid[], char password[]) 
{
    WifiInfo* wifi = (WifiInfo*)malloc(sizeof(WifiInfo));
    strncpy(wifi->ssid, ssid, MAX_SSID_LENGTH);
    strncpy(wifi->password, password, MAX_PASSWORD_LENGTH);
    wifi->position = 0;
    wifi->next = NULL;
    return wifi;
}

void insertWifiInfo(WifiInfo** head, char ssid[], char password[]) 
{
    // Check if ssid already exists in the list
    WifiInfo* current = *head;
    WifiInfo* prev = NULL;
    int position = 0;

    while (current != NULL && strcmp(current->ssid, ssid) != 0) 
    {
        prev = current;
        current = current->next;
        position++;
    }

    if (current != NULL) 
    {
        // Move the existing wifi info to the beginning
        if (prev != NULL)
            prev->next = current->next;
        current->next = *head;
        current->position = 0;
        *head = current;
    }
    else 
    {
        // Create a new wifi info and insert at the beginning
        WifiInfo* newWifi = createWifiInfo(ssid, password);
        newWifi->position = 0;
        newWifi->next = *head;
        *head = newWifi;
    }

    // Update the positions of other wifi infos
    current = *head;
    position = 0;
    while (current != NULL)
    {
        current->position = position++;
        current = current->next;
    }

    // Check if the list exceeds the maximum count
    if (position > MAX_WIFI_COUNT)
    {
        current = *head;
        prev = NULL;

        // Traverse to the last element
        while (current->next != NULL)
        {
            prev = current;
            current = current->next;
        }

        if (prev != NULL)
            prev->next = NULL;

        free(current);
    }
}

void displayWifiInfo(WifiInfo* head) 
{
    WifiInfo* current = head;

    while (current != NULL) 
    {
        printf("SSID: %s, Password: %s, Position: %d\n", current->ssid, current->password, current->position);
        current = current->next;
    }
}

void freeWifiInfoList(WifiInfo* head)
{
    WifiInfo* current = head;
    WifiInfo* temp;

    while (current != NULL) 
    {
        temp = current->next;
        free(current);
        current = temp;
    }
}

int main()
{
    WifiInfo* wifiList = NULL;
    char ssid[MAX_SSID_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    while (1) 
    {
        printf("Enter WiFi SSID (or 'q' to quit): ");
        scanf("%s", ssid);

        if (strcmp(ssid, "q") == 0)
            break;

        printf("Enter WiFi Password: ");
        scanf("%s", password);

        insertWifiInfo(&wifiList, ssid, password);
        displayWifiInfo(wifiList);
        printf("\n");
    }

    freeWifiInfoList(wifiList);

    return 0;
}

```

![image-20230717093536747](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230717093536747.png)

