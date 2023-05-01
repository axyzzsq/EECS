//
//  testc.c
//  myapp
//
//  Created by lichao on 2020/1/30.
//  Copyright © 2020年 lichao. All rights reserved.
//

#include "testc.h"
#include <string.h>

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

static void open_encoder(AVFormatContext *fmt_ctx,
                         AVCodecContext **enc_ctx){
    
    int ret = -1;
    const AVCodec *codec;
    
    /* find the mpeg1video encoder */
    codec = avcodec_find_encoder_by_name("libx264");
    if (!codec) {
        fprintf(stderr, "Codec libx264 not found\n");
        exit(1);
    }
    
    *enc_ctx = avcodec_alloc_context3(codec);
    if (!enc_ctx) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }
    
    (*enc_ctx)->profile = FF_PROFILE_H264_HIGH_444;
    (*enc_ctx)->level = 50;
    
    /* put sample parameters */
    (*enc_ctx)->bit_rate = 500000;
    (*enc_ctx)->rc_min_rate = 500000;
    (*enc_ctx)->rc_max_rate = 500000;
    (*enc_ctx)->rc_buffer_size= 500000;
//    (*enc_ctx)->qcompress = 0.0;

    /* resolution must be a multiple of two */
    (*enc_ctx)->width = 640; //不能改变分辩率大小
    (*enc_ctx)->height = 480;
    /* frames per second */
    (*enc_ctx)->time_base = (AVRational){1, 25}; //抓取的数据与编码可以不一样
    (*enc_ctx)->framerate = (AVRational){25, 1};
    
    /* emit one intra frame every ten frames
     * check frame pict_type before passing frame
     * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
     * then gop_size is ignored and the output of encoder
     * will always be I frame irrespective to gop_size
     */
    (*enc_ctx)->gop_size = 250;
    (*enc_ctx)->keyint_min = 25; // keyint_min=25
    (*enc_ctx)->max_b_frames = 3;
    (*enc_ctx)->refs = 3;
    (*enc_ctx)->slices = 2;
    (*enc_ctx)->pix_fmt = AV_PIX_FMT_YUV420P;
    
    //(*enc_ctx)->coder_type = 1;  // coder = 1
    (*enc_ctx)->flags|= AV_CODEC_FLAG_LOOP_FILTER;   // flags=+loop
    (*enc_ctx)->me_cmp|= 1;  // cmp=+chroma, where CHROMA = 1
    //(*enc_ctx)->//partitions|=X264_PART_I8X8+X264_PART_I4X4+X264_PART_P8X8+X264_PART_B8X8; // partitions=+parti8x8+parti4x4+partp8x8+partb8x8
    //(*enc_ctx)->me_method=/ME_HEX;    // me_method=hex
    (*enc_ctx)->me_subpel_quality = 7;   // subq=7
    (*enc_ctx)->me_range = 16;   // me_range=16
    (*enc_ctx)->i_quant_factor = 0.71; // i_qfactor=0.71
    (*enc_ctx)->qcompress = 0; // qcomp=0.6
    (*enc_ctx)->qmin = 51;   // qmin=10
    (*enc_ctx)->qmax = 51;   // qmax=51
    (*enc_ctx)->max_qdiff = 4;   // qdiff=4
    (*enc_ctx)->trellis = 1; // trellis=1
    (*enc_ctx)->pa
//    (*enc_ctx)->flags2|=CODEC_FLAG2_BPYRAMID+CODEC_FLAG2_MIXED_REFS+CODEC_FLAG2_WPRED+CODEC_FLAG2_8X8DCT+CODEC_FLAG2_FASTPSKIP;  // flags2=+bpyramid+mixed_refs+wpred+dct8x8+fastpskip

//    (*enc_ctx)->flags2|=AV_CODEC_FLAG2_  //CODEC_FLAG2_8X8DCT;
//    (*enc_ctx)->flags2^=CODEC_FLAG2_8X8DCT;    // flags2=-dct8x8
    
//    if (codec->id == AV_CODEC_ID_H264)
//        av_opt_set((*enc_ctx)->priv_data, "preset", "slow", 0);
    
    /* open it */
    ret = avcodec_open2((*enc_ctx), codec, NULL);
    if (ret < 0) {
        fprintf(stderr, "Could not open codec: %s\n", av_err2str(ret));
        exit(1);
    }
}

static void encode(AVCodecContext *enc_ctx,
                   AVFrame *frame,
                   AVPacket *pkt,
                   FILE *outfile){
    
    int ret;
    
    /* send the frame to the encoder */
    if (frame)
        printf("Send frame %3"PRId64"\n", frame->pts);
    
    ret = avcodec_send_frame(enc_ctx, frame);
    if (ret < 0) {
        fprintf(stderr, "Error sending a frame for encoding\n");
        exit(1);
    }
    
    while (ret >= 0) {
        ret = avcodec_receive_packet(enc_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "Error during encoding\n");
            exit(1);
        }
        
        printf("Write packet %3"PRId64" (size=%5d)\n", pkt->pts, pkt->size);
        fwrite(pkt->data, 1, pkt->size, outfile);
        av_packet_unref(pkt);
    }
}

/**
 * @brief xxxx
 * @return xxx
 */
static
AVFrame* create_frame(int width, int height){
    
    int ret = -1;
    AVFrame *frame = NULL;
    
    //音频输入数据
    frame = av_frame_alloc();
    if(!frame){
        printf("Error, No Memory!\n");
        goto __ERROR;
    }
    
    //set parameters
    frame->width  = width;   //图像的宽
    frame->height = height;  //图像的高
    frame->format = AV_PIX_FMT_YUVA420P; //像素格式
    
    //alloc inner memory
    ret = av_frame_get_buffer(frame, 32); // 一般按 32 位对齐
    if(ret < 0){
        printf("Error, Failed to alloc buf in frame!\n");
        //内存泄漏
        goto __ERROR;
    }
    
    return frame;
    
__ERROR:
    if(frame){
        av_frame_free(&frame);
    }
    
    return NULL;
}


//void rec_audio() {
void rec_video() {
    
    int ret = 0;
    long base=0;
    
    AVFormatContext *fmt_ctx = NULL;
    AVCodecContext *enc_ctx = NULL;
    
    //pakcet
    AVPacket pkt;
    AVFrame *frame = NULL;
    AVPacket *newpkt = NULL;
    
    //set log level
    av_log_set_level(AV_LOG_DEBUG);
    
    //start record
    rec_status = 1;
    
    //create file
//    char *out = "/Users/lichao/Downloads/av_base/video.yuv";
    char *out = "/Users/lichao/Downloads/av_base/video.h264";
    FILE *outfile = fopen(out, "wb+");
    
    //打开设备
    fmt_ctx = open_dev();
    
    //打开编码器
    open_encoder(fmt_ctx, &enc_ctx);
    
    //创建frame
    frame = create_frame(640, 480);
    
    newpkt = av_packet_alloc(); //分配编码后的数据空间
    if(!newpkt){
        printf("Error, Failed to alloc buf in frame!\n");
        goto __ERROR;
    }

    //read data from device
    while((ret = av_read_frame(fmt_ctx, &pkt)) == 0 &&
          rec_status) {
        
        int i = 0;
        
        av_log(NULL, AV_LOG_INFO,
               "packet size is %d(%p)\n",
               pkt.size, pkt.data);
        
        memcpy(frame->data[0], pkt.data, 307200);//921600 307200
        for(i=0; i< 307200/4; i++){
            frame->data[1][i] = pkt.data[307200+i*2];
            frame->data[2][i] = pkt.data[307200+i*2];
        }
        
        //base = base? : pkt.pts;
        frame->pts = base++;//pkt.pts - base; //特别重要
        printf("current pts = %lld \n", frame->pts);
       
        //（宽 x 高）x (yuv420=1.5/yuv422=2/yuv444=3)
//        fwrite(pkt.data, 1, 460800, outfile);
//        fflush(outfile);
//        fwrite(frame->data[0], 1, 307200, outfile);
//        fwrite(frame->data[1], 1, 307200/4, outfile);
//        fwrite(frame->data[2], 1, 307200/4, outfile);
        
        encode(enc_ctx, frame, newpkt, outfile);
        
        //
        av_packet_unref(&pkt); //release pkt
    }
    
    encode(enc_ctx, NULL, newpkt, outfile);
  
__ERROR:
    if(outfile){
        //close file
        fclose(outfile);
    }
    
    //close device and release ctx
    if(fmt_ctx) {
        avformat_close_input(&fmt_ctx);
    }
    
    //释放 AVFrame 和 AVPacket
    if(frame){
        av_frame_free(&frame);
    }
    
    if(newpkt){
        av_packet_free(&newpkt);
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
