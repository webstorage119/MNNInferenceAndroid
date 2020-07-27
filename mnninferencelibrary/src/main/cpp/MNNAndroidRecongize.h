//
// Created by ty on 2020/7/14.
//

#ifndef CIGARETTE_MNNANDROIDRECONGIZE_H
#define CIGARETTE_MNNANDROIDRECONGIZE_H






#include "MNN/Interpreter.hpp"
#include "MNN/MNNDefine.h"
#include "MNN/Tensor.hpp"
#include "MNN/ImageProcess.hpp"
#include "opencv2/opencv.hpp"
#include <cmath>
#include <algorithm>
#include "Utils.h"
#include <iostream>

class MNNAndroidRecongize {
public:
    MNNAndroidRecongize();

    ~MNNAndroidRecongize();

    bool init(const std::string &mnn_path, int resizeWidth = 224, int resizeHeight = 224, int num_thread_ = 4, float score_threshold_ = 0.7, bool openCL = true);

    std::vector<Result> recongize(cv::Mat &img);
    char* detect(cv::Mat &img);

private:
    std::shared_ptr<MNN::Interpreter> mnn_interpreter;

    MNN::Session *mnn_session = nullptr;

    MNN::Tensor *input_tensor = nullptr;

    int num_thread = 4;
    int resize_w = 224;
    int resize_h = 224;
    float score_threshold = 0.7;
    const float mean_vals[3] = {123.675, 116.28, 103.53};//均值
    const float norm_vals[3] = {1.0/58.395, 1.0/57.12, 1.0/57.375};//标准差的导数






};





#endif //CIGARETTE_MNNANDROIDRECONGIZE_H
