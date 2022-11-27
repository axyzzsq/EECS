//
//  testc.c
//  myapp
//
//  Created by lichao on 2020/1/30.
//  Copyright © 2020年 lichao. All rights reserved.
//

#include "testc.h"
#include <string.h>

#define V_WIDTH 640
#define V_HEIGTH 480

static int rec_status = 0;

void set_status(int status){
    rec_status = status;
}

//@brief
//return
static AVFormatContext* open_dev(){
    
    int ret = 0;
    char errors[1024] = {0, };
    
    //ctx
    AVFormatContext *fmt_ctx = NULL;
    AVDictionary *options = NULL;
    
    //[[video device]:[audio device]]
    //0: 机器的摄像头
    //1: 桌面
    char *devicename = "0";
    
    //register audio device
    avdevice_register_all();
    
    //get format
    AVInputFormat *iformat = av_find_input_format("avfoundation");
    
    av_dict_set(&options, "video_size", "640x480", 0);
    av_dict_set(&options, "framerate", "30", 0);
    av_dict_set(&options, "pixel_format", "nv12", 0);
    
    //open device
    if((ret = avformat_open_input(&fmt_ctx, devicename, iformat, &options)) < 0 ){
        av_strerror(ret, errors, 1024);
        fprintf(stderr, "Failed to open video device, [%d]%s\n", ret, errors);
        return NULL;
    }
    
    return fmt_ctx;
}

/**
 * @brief xxxx
 * @param[in] xxx
 * @param[in] xxx
 * @param[out] xxx
 */
static void open_encoder(int width,
                         int height,
                         AVCodecContext **enc_ctx){
    
    int ret = 0;
    AVCodec *codec = NULL;
    
    codec = avcodec_find_encoder_by_name("libx264");
    if(!codec){
        printf("Codec libx264 not found\n");
        exit(1);
    }
    
    *enc_ctx = avcodec_alloc_context3(codec);
    if(!enc_ctx){
        printf("Could not allocate video codec context!\n");
        exit(1);
    }
    
    //SPS/PPS
    (*enc_ctx)->profile = FF_PROFILE_H264_HIGH_444;
    (*enc_ctx)->level = 50; //表示LEVEL是5.0
    
    //设置分辫率
    (*enc_ctx)->width = width;   //640
    (*enc_ctx)->height = height; //480
    
    //GOP
    (*enc_ctx)->gop_size = 250;
    (*enc_ctx)->keyint_min = 25; //option
    
    //设置B帧数据
    (*enc_ctx)->max_b_frames = 3; //option
    (*enc_ctx)->has_b_frames = 1; //option
    
    //参考帧的数量
    (*enc_ctx)->refs = 3;         //option
    
    //设置输入YUV格式
    (*enc_ctx)->pix_fmt = AV_PIX_FMT_YUV420P;
    
    //设置码率
    (*enc_ctx)->bit_rate = 600000; //600kbps
    
    //设置帧率
    (*enc_ctx)->time_base = (AVRational){1, 25}; //帧与帧之间的间隔是time_base
    (*enc_ctx)->framerate = (AVRational){25, 1}; //帧率，每秒 25 帧
    
    ret = avcodec_open2((*enc_ctx), codec, NULL);
    if(ret<0){
        printf("Could not open codec: %s!\n", av_err2str(ret));
        exit(1);
    }
}

/**
 * @brief xxxx
 * @param[in] width,
 * @param[in] height
 * @return AVFrame*
 */
static AVFrame* create_frame(int width, int height){
    
    int ret = 0;
    AVFrame *frame = NULL;
    
    frame = av_frame_alloc();
    if(!frame){
        printf("Error, No Memory!\n");
        goto __ERROR;
    }
    
    //设置参数
    frame->width = width;
    frame->height = height;
    frame->format = AV_PIX_FMT_YUV420P;
    
    //alloc inner memory
    ret = av_frame_get_buffer(frame, 32); //按 32 位对齐
    if(ret < 0){
        printf("Error, Failed to alloc buffer for frame!\n");
        goto __ERROR;
    }
    
    return frame;
    
__ERROR:
    
    if(frame){
        av_frame_free(&frame);
    }
    
    return NULL;
}

static void encode(AVCodecContext *enc_ctx,
                   AVFrame *frame,
                   AVPacket *newpkt,
                   FILE *outfile){
    
    int ret = 0;
    
//    if(!enc_ctx){
//        //xxxx
//    }
//
//    if(!frame){
//
//    }
    
    if(frame){
        printf("send frame to encoder, pts=%lld", frame->pts);
    }
    //送原始数据给编码器进行编码
    ret = avcodec_send_frame(enc_ctx, frame);
    if(ret < 0) {
        printf("Error, Failed to send a frame for enconding!\n");
        exit(1);
    }
    
    //从编码器获取编码好的数据
    while(ret >=0) {
        ret = avcodec_receive_packet(enc_ctx, newpkt);
        
        //如果编码器数据不足时会返回  EAGAIN,或者到数据尾时会返回 AVERROR_EOF
        if( ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
            return;
        }else if (ret <0){
            printf("Error, Failed to encode!\n");
            exit(1);
        }
        
        fwrite(newpkt->data, 1, newpkt->size, outfile);
        av_packet_unref(newpkt);
    }
}

void rec_video() {
    
    int ret = 0;
    int base = 0;

    //pakcet
    AVPacket pkt;
    AVFormatContext *fmt_ctx = NULL;
    AVCodecContext *enc_ctx = NULL;
    
    //set log level
    av_log_set_level(AV_LOG_DEBUG);
    
    //start record
    rec_status = 1;
    
    //create file
    char *yuvout = "/Users/lichao/Downloads/av_base/video.yuv";
    char *out = "/Users/lichao/Downloads/av_base/video.h264";
    
    FILE *yuvoutfile = fopen(yuvout, "wb+");
    FILE *outfile = fopen(out, "wb+");
    
    //打开设备
    fmt_ctx = open_dev();
    
    //打开编码器
    open_encoder(V_WIDTH, V_HEIGTH, &enc_ctx);
    
    //创建 AVFrame
    AVFrame* frame = create_frame(V_WIDTH, V_HEIGTH);
    
    //创建编码后输出的Packet
    AVPacket *newpkt = av_packet_alloc();
    if(!newpkt){
        printf("Error, Failed to alloc avpacket!\n");
        goto __ERROR;
    }

    //read data from device
    while((ret = av_read_frame(fmt_ctx, &pkt)) == 0 &&
          rec_status) {
        
        int i =0;
        
        av_log(NULL, AV_LOG_INFO,
               "packet size is %d(%p)\n",
               pkt.size, pkt.data);
       
        //（宽 x 高）x (yuv420=1.5/yuv422=2/yuv444=3)
//        fwrite(pkt.data, 1, 460800, yuvoutfile);
//        fflush(yuvoutfile);
        
        //YYYYYYYYUVVU NV12
        //YYYYYYYYUUVV YUV420
        memcpy(frame->data[0], pkt.data, 307200); //copy Y data
        //307200之后，是UV
        for(i=0; i < 307200/4; i++){
            frame->data[1][i] = pkt.data[307200+i*2];
            frame->data[2][i] = pkt.data[307201+i*2];
        }
        
        fwrite(frame->data[0], 1, 307200, yuvoutfile);
        fwrite(frame->data[1], 1, 307200/4, yuvoutfile);
        fwrite(frame->data[2], 1, 307200/4, yuvoutfile);
        
        frame->pts = base++;
        encode(enc_ctx, frame, newpkt, outfile);
        //
        av_packet_unref(&pkt); //release pkt
    }
    
    encode(enc_ctx, NULL, newpkt, outfile);
    
__ERROR:
    if(yuvoutfile){
        //close file
        fclose(yuvoutfile);
    }
    
    //close device and release ctx
    if(fmt_ctx) {
        avformat_close_input(&fmt_ctx);
    }

    av_log(NULL, AV_LOG_DEBUG, "finish!\n");
    
    return;
}

#if 0
int main(int argc, char *argv[])
{
    rec_audio();
    return 0;
}
#endif
