//
// Created by ty on 2020/7/14.
//



#include "MNNAndroidRecongize.h"



MNNAndroidRecongize::MNNAndroidRecongize(){

}


MNNAndroidRecongize::~MNNAndroidRecongize(){
    mnn_interpreter->releaseModel();
    mnn_interpreter->releaseSession(mnn_session);

}



bool MNNAndroidRecongize::init(const std::string &mnn_path, int resizeWidth, int resizeHeight,
                               int num_thread_, float score_threshold_, bool openCL){
    num_thread = num_thread_;
    score_threshold = score_threshold_;
    resize_w = resizeWidth;
    resize_h = resizeHeight;
    //create mnn interpreter
    mnn_interpreter = std::shared_ptr<MNN::Interpreter>(MNN::Interpreter::createFromFile(mnn_path.c_str()));
    MNN::ScheduleConfig config;
    config.numThread = num_thread;
    if (openCL) {
        config.backupType = MNN_FORWARD_OPENCL;
    }
    MNN::BackendConfig backendConfig;
    backendConfig.precision = (MNN::BackendConfig::PrecisionMode) 2;
    config.backendConfig = &backendConfig;

    //create mnn session
    mnn_session = mnn_interpreter->createSession(config);

    //get input tensor
    input_tensor = mnn_interpreter->getSessionInput(mnn_session, nullptr);

    return true;

}

std::vector<Result> MNNAndroidRecongize::recongize(cv::Mat &img) {
    std::vector<Result> resultList = {};
    Result result;
    if (img.empty()) {
        std::cout << "img is empty!!!" << std::endl;

        result.index = -1;
        result.score = -1.0;
        resultList.push_back(result);
    }
    int image_h = img.rows;
    int image_w = img.cols;
    cv::Mat image;
    cv::resize(img, image, cv::Size(resize_w, resize_h), cv::INTER_LINEAR);
    mnn_interpreter->resizeTensor(input_tensor, {1, 3, resize_w, resize_h});
    std::shared_ptr<MNN::CV::ImageProcess> pretreat(
            MNN::CV::ImageProcess::create(MNN::CV::BGR, MNN::CV::RGB, mean_vals, 3,
                                          norm_vals, 3));
    pretreat->convert(image.data, resize_w, resize_h, image.step[0], input_tensor);
    mnn_interpreter->runSession(mnn_session);
    MNN::Tensor *outputTensor = mnn_interpreter->getSessionOutput(mnn_session, NULL);
    MNN::Tensor *nchwTensor = new MNN::Tensor(outputTensor, MNN::Tensor::CAFFE);
    outputTensor->copyToHostTensor(nchwTensor);
    int size = nchwTensor->elementSize();
    auto output = nchwTensor->host<float>();
    float score[size];
    float e_score;
    std::vector<float> s_score;
    for(int i =0; i< size; i++){
        score[i] = output[i];
        e_score += exp(output[i]);
    }
    for(int i =0; i< size; i++){
        s_score.push_back(exp(score[i]) / e_score);
    }
    float max_v = *max_element(s_score.begin(), s_score.end());
    int max_index = max_element(s_score.begin(), s_score.end()) - s_score.begin();
    result.score = max_v;
    result.index = max_index;
    resultList.push_back(result);
    return resultList;















}
