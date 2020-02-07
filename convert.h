#include <iostream>
#include <memory>
#include <vector>

extern "C"
{
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
}

AVFrame *AvFrameFromBuffer(const uint8_t *data, int width, int height, AVPixelFormat pix_fmt);
AVFrame *ConvertFrameToRGB(const AVFrame *frame);
int SaveYUV420FrameAsJPEG(AVFrame *pFrame, std::string filename);
void SaveRGBFrameAsPPM(AVFrame *pFrame, std::string filename);
