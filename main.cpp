#include <iostream>
#include <memory>
#include <fstream>
#include <vector>
#include <assert.h>
#include "convert.h"

int main(int argc, char *argv[])
{
    int width = 1164;
    int height = 874;

    if (argc < 2)
    {
        std::cout << "Please specify an image file. There is one in ./data folder." << std::endl;
        return -1;
    }

    av_register_all();

    std::ifstream img(argv[1], std::ios::in | std::ios::binary);
    img.seekg(0, std::ios::end);
    std::streamsize size = img.tellg();
    img.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);

    if (img.read(buffer.data(), size))
    {
        AVFrame *frame = AvFrameFromBuffer(reinterpret_cast<uint8_t*>(buffer.data()), 
            width, 
            height, 
            AV_PIX_FMT_YUV420P);

        SaveYUV420FrameAsJPEG(frame, "test.jpeg");
        AVFrame *frameRGB = ConvertFrameToRGB(frame);
        SaveRGBFrameAsPPM(frameRGB, "test.ppm");
        av_frame_free(&frame);
        av_frame_free(&frameRGB);
    }
    else
    {
        std::cout << "Failed to load image" << std::endl;
        return -1;
    }

    std::cout << "Done" << std::endl;
    return 0;
}
