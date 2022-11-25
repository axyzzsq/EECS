// test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include <windows.h>
#include <wchar.h>

#include <stdio.h>
#include <string.h>



extern "C" {
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
#include <libavutil/avutil.h>
}

static
AVFormatContext* open_dev() {

	int ret = 0;
	char errors[1024] = { 0, };

	avdevice_register_all();

	//ctx
	AVFormatContext *fmt_ctx = NULL;
	AVDictionary *options = NULL;

	//[[video device]:[audio device]]
	//这里修改为你自己windows音频设备的名字
	const char *devicename = "audio=麦克风阵列 (Realtek(R) Audio)";

	//get format
	AVInputFormat *iformat = av_find_input_format("dshow");

	//open device
	if ((ret = avformat_open_input(&fmt_ctx, devicename, iformat, &options)) < 0) {
		av_strerror(ret, errors, 1024);
		fprintf(stderr, "Failed to open audio device, [%d]%s\n", ret, errors);
		return NULL;
	}

	return fmt_ctx;
}

int main()
{
	open_dev();
    std::cout << "Hello World!\n";
}

