////
//// Created by ty on 2020/7/20.
////
//
//#ifndef MNNINFERENCEANDROID_MNNANDROIDDETECT_H
//#define MNNINFERENCEANDROID_MNNANDROIDDETECT_H
//
//#include "MNN/Interpreter.hpp"
//#include "MNN/MNNDefine.h"
//#include "MNN/Tensor.hpp"
//#include "MNN/ImageProcess.hpp"
//#include "opencv2/opencv.hpp"
//
//typedef struct Anchor {
//    float x1;
//    float y1;
//    float x2;
//    float y2;
//    float score;
//} anchor;
//
//
//class MNNAndroidDetect {
//public:
//    MNNAndroidDetect();
//
//    ~MNNAndroidDetect();
//
//    bool init(const std::string &mnn_path, int num_thread_ = 4, float score_threshold_ = 0.7, bool openCL = true);
//
//
//    char* detect(cv::Mat &img);
//
//private:
//    std::shared_ptr<MNN::Interpreter> mnn_interpreter;
//
//    MNN::Session *mnn_session = nullptr;
//
//    MNN::Tensor *input_tensor = nullptr;
//
//    int num_thread = 4;
//
//    int inputWidth = 416;
//    int inputheight = 416;
//
//    float score_threshold = 0.7;
//
//    const float mean_vals[3] = {127, 127, 127};
//    const float norm_vals[3] = {1.0 / 128, 1.0 / 128, 1.0 / 128};
//    const float ancher[18] = {10,13,  16,30,  33,23,  30,61,  62,45,  59,119,  116,90,  156,198,  373,326};
//
//
//    const std::string classes[2] = {
//            "1",
//            "2"
//    };
//
//    void generateBBox(std::vector<anchor> &bbox_collection, int img_width, int img_height,
//            MNN::Tensor *scores, MNN::Tensor *boxes);
//
//    void nms(std::vector<anchor> &input, std::vector<anchor> &output, int type);
//
//
//
//};
//
//
//
//#endif //MNNINFERENCEANDROID_MNNANDROIDDETECT_H
