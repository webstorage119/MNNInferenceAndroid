////
//// Created by ty on 2020/7/20.
////
//
//#include "MNNAndroidDetect.h"
//
//
//MNNAndroidDetect::MNNAndroidDetect(){
//
//}
//
//
//MNNAndroidDetect::~MNNAndroidDetect(){
//    mnn_interpreter->releaseModel();
//    mnn_interpreter->releaseSession(mnn_session);
//
//}
//
//
//
//bool MNNAndroidDetect::init(const std::string &mnn_path, int num_thread_, float score_threshold_, bool openCL){
//    num_thread = num_thread_;
//    score_threshold = score_threshold_;
//    //create mnn interpreter
//    mnn_interpreter = std::shared_ptr<MNN::Interpreter>(MNN::Interpreter::createFromFile(mnn_path.c_str()));
//    MNN::ScheduleConfig config;
//    config.numThread = num_thread;
//    if (openCL) {
//        config.backupType = MNN_FORWARD_OPENCL;
//    }
//    MNN::BackendConfig backendConfig;
//    backendConfig.precision = (MNN::BackendConfig::PrecisionMode) 2;
//    config.backendConfig = &backendConfig;
//
//    //create mnn session
//    mnn_session = mnn_interpreter->createSession(config);
//
//    //get input tensor
//    input_tensor = mnn_interpreter->getSessionInput(mnn_session, nullptr);
//
//    return true;
//
//}
//
//char* MNNAndroidDetect::detect(cv::Mat &img) {
//    char* result;
//    if (img.empty()) {
//        std::cout << "img is empty!!!" << std::endl;
//        return result = "-1:-1";
//    }
//    cv::Mat image;
//    cv::resize(img, image, cv::Size(inputWidth, inputheight));
//    mnn_interpreter->resizeTensor(input_tensor, {1, 3, inputWidth, inputheight});
//    mnn_interpreter->runSession(mnn_session);
//    std::string scores = "scores";
//    std::string boxes = "boxes";
//    MNN::Tensor *tensor_scores = mnn_interpreter->getSessionOutput(mnn_session,
//                                                                         scores.c_str());
//    MNN::Tensor *tensor_boxes = mnn_interpreter->getSessionOutput(mnn_session,
//                                                                        boxes.c_str());
//    MNN::Tensor tensor_scores_host(tensor_scores, tensor_scores->getDimensionType());
//    tensor_scores->copyToHostTensor(&tensor_scores_host);
//    MNN::Tensor tensor_boxes_host(tensor_boxes, tensor_boxes->getDimensionType());
//    tensor_boxes->copyToHostTensor(&tensor_boxes_host);
//
//
//
//    return result;
//
//
//}
//
//
//void MNNAndroidDetect::generateBBox(std::vector<anchor> &bbox_collection, int img_width, int img_height,
//                             MNN::Tensor *scores, MNN::Tensor *boxes) {
//    for (int i = 0; i < num_anchors; i++) {
//        if (scores->host<float>()[i * 2 + 1] > score_threshold) {
//            anchor rects;
//            float x_center =
//                    boxes->host<float>()[i * 4] * center_variance * priors[i][2] + priors[i][0];
//            float y_center =
//                    boxes->host<float>()[i * 4 + 1] * center_variance * priors[i][3] + priors[i][1];
//            float w = exp(boxes->host<float>()[i * 4 + 2] * size_variance) * priors[i][2];
//            float h = exp(boxes->host<float>()[i * 4 + 3] * size_variance) * priors[i][3];
//            rects.x1 = clip(x_center - w / 2.0, 1) * img_width;
//            rects.y1 = clip(y_center - h / 2.0, 1) * img_height;
//            rects.x2 = clip(x_center + w / 2.0, 1) * img_width;
//            rects.y2 = clip(y_center + h / 2.0, 1) * img_height;
//            rects.score = clip(scores->host<float>()[i * 2 + 1], 1);
//            bbox_collection.push_back(rects);
//        }
//    }
//}
//
//void MNNAndroidDetect::nms(std::vector<anchor> &input, std::vector<anchor> &output, int type) {
//    std::sort(input.begin(), input.end(),
//              [](const anchor &a, const anchor &b) { return a.score > b.score; });
//    int box_num = input.size();
//    std::vector<int> merged(box_num, 0);
//    for (int i = 0; i < box_num; i++) {
//        if (merged[i])
//            continue;
//        std::vector<anchor> buf;
//        buf.push_back(input[i]);
//        merged[i] = 1;
//        float h0 = input[i].y2 - input[i].y1 + 1;
//        float w0 = input[i].x2 - input[i].x1 + 1;
//        float area0 = h0 * w0;
//        for (int j = i + 1; j < box_num; j++) {
//            if (merged[j])
//                continue;
//            float inner_x0 = input[i].x1 > input[j].x1 ? input[i].x1 : input[j].x1;
//            float inner_y0 = input[i].y1 > input[j].y1 ? input[i].y1 : input[j].y1;
//            float inner_x1 = input[i].x2 < input[j].x2 ? input[i].x2 : input[j].x2;
//            float inner_y1 = input[i].y2 < input[j].y2 ? input[i].y2 : input[j].y2;
//            float inner_h = inner_y1 - inner_y0 + 1;
//            float inner_w = inner_x1 - inner_x0 + 1;
//            if (inner_h <= 0 || inner_w <= 0)
//                continue;
//            float inner_area = inner_h * inner_w;
//            float h1 = input[j].y2 - input[j].y1 + 1;
//            float w1 = input[j].x2 - input[j].x1 + 1;
//            float area1 = h1 * w1;
//            float score;
//            score = inner_area / (area0 + area1 - inner_area);
//            if (score > iou_threshold) {
//                merged[j] = 1;
//                buf.push_back(input[j]);
//            }
//        }
//        switch (type) {
//            case hard_nms: {
//                output.push_back(buf[0]);
//                break;
//            }
//            case blending_nms: {
//                float total = 0;
//                for (int i = 0; i < buf.size(); i++) {
//                    total += exp(buf[i].score);
//                }
//                anchor rects;
//                memset(&rects, 0, sizeof(rects));
//                for (int i = 0; i < buf.size(); i++) {
//                    float rate = exp(buf[i].score) / total;
//                    rects.x1 += buf[i].x1 * rate;
//                    rects.y1 += buf[i].y1 * rate;
//                    rects.x2 += buf[i].x2 * rate;
//                    rects.y2 += buf[i].y2 * rate;
//                    rects.score += buf[i].score * rate;
//                }
//                output.push_back(rects);
//                break;
//            }
//            default: {
//                printf("wrong type of nms.");
//                exit(-1);
//            }
//        }
//    }
//}