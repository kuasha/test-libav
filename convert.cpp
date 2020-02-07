#include <fstream>
#include "convert.h"

extern "C"
{
#include "libavutil/cpu.h"
#include "libavutil/imgutils.h"
#include "libavutil/mem.h"
#include "libavutil/avutil.h"
#include "libavutil/crc.h"
#include "libavutil/pixdesc.h"
#include "libavutil/lfg.h"
}

AVFrame *AvFrameFromBuffer(const uint8_t *data, int width, int height, AVPixelFormat pix_fmt, int align = 1)
{
    AVFrame *frame = av_frame_alloc();

    frame->width = width;
    frame->height = height;
    frame->format = pix_fmt;

    int res = av_image_fill_arrays(frame->data,
                                   frame->linesize,
                                   data,
                                   pix_fmt,
                                   frame->width,
                                   frame->height,
                                   align);

    return frame;
}

AVFrame *ConvertFrameToRGB(const AVFrame *frame)
{
    auto img_convert_ctx = sws_getContext(frame->width,
                                          frame->height,
                                          (AVPixelFormat)frame->format,
                                          frame->width,
                                          frame->height,
                                          AV_PIX_FMT_RGB24,
                                          SWS_BICUBIC, NULL, NULL, NULL);

    AVFrame *frameRGB = av_frame_alloc();
    frameRGB->width = frame->width;
    frameRGB->height = frame->height;
    frameRGB->format = AV_PIX_FMT_RGB24;

    av_frame_get_buffer(frameRGB, 0);

    sws_scale(img_convert_ctx,
              frame->data,
              frame->linesize,
              0,
              frame->height,
              frameRGB->data,
              frameRGB->linesize);

    return frameRGB;
}

// https://stackoverflow.com/questions/40472653/how-to-write-avframe-out-as-jpeg-image
int SaveYUV420FrameAsJPEG(AVFrame *pFrame, std::string filename)
{
    AVCodec *jpegCodec = avcodec_find_encoder(AV_CODEC_ID_MJPEG);
    if (!jpegCodec)
    {
        std::cerr << "bad codec" << std::endl;
        return -1;
    }

    AVCodecContext *jpegContext = avcodec_alloc_context3(jpegCodec);
    if (!jpegContext)
    {
        std::cerr << "bad context" << std::endl;
        return -1;
    }

    jpegContext->pix_fmt = AV_PIX_FMT_YUVJ420P;
    jpegContext->height = pFrame->height;
    jpegContext->width = pFrame->width;
    jpegContext->time_base = AVRational{1, 1};

    if (avcodec_open2(jpegContext, jpegCodec, NULL) < 0)
    {
        std::cerr << " can not open " << std::endl;
        return -1;
    }

    AVPacket packet;
    packet.data = NULL;
    packet.size = 0;

    av_init_packet(&packet);
    int gotFrame;
    if (avcodec_encode_video2(jpegContext, &packet, pFrame, &gotFrame) < 0)
    {
        std::cerr << " encode error " << std::endl;
        return -1;
    }

    std::ofstream f(filename.c_str(), std::ios::binary | std::ios::out);
    f.write(reinterpret_cast<const char *>(packet.data), packet.size);

    av_free_packet(&packet);
    avcodec_close(jpegContext);

    return 0;
}

void SaveRGBFrameAsPPM(AVFrame *pFrame, std::string filename)
{
    std::ofstream f(filename.c_str(), std::ios::out | std::ios::binary);

    // https://en.wikipedia.org/wiki/Netpbm#PPM_example
    f << "P6" << std::endl;
    f << pFrame->width << " " << pFrame->height << std::endl;
    f << 255 << std::endl;

    int lineBinaryWidth = pFrame->width * 3;
    for (int y = 0; y < pFrame->height; y++)
    {
        int offset = y * pFrame->linesize[0];
        f.write(reinterpret_cast<const char *>(pFrame->data[0] + offset), lineBinaryWidth);
    }
}