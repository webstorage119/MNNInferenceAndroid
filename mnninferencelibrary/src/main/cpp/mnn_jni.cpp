//
// Created by ty on 2020/7/11.
//



#include "jni.h"
#include "stdlib.h"
#include "string.h"
#include "MNNAndroidRecongize.h"
#include "Utils.h"

MNNAndroidRecongize* androidRecongize;

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_deeplean_mnninferencelibrary_MNNInference_initRecongize(JNIEnv *env, jobject thiz, jstring mnn_path,
                                           jint resize_width, jint resize_height, jint num_thread, jboolean open_cl) {
    androidRecongize = new MNNAndroidRecongize();
    std::string mnnPath = getFilePath(env, mnn_path);
    float score_threshold = 0.7;
    return androidRecongize->init(mnnPath, resize_width, resize_height, num_thread, score_threshold,open_cl);
}

extern "C"

JNIEXPORT jobject JNICALL
Java_com_deeplean_mnninferencelibrary_MNNInference_recongize(JNIEnv *env, jobject thiz, jstring img_path) {
    const char *imgPath = env->GetStringUTFChars(img_path, 0);
    cv::Mat img = cv::imread(imgPath);
    std::vector<Result> v_result = androidRecongize->recongize(img);

    //获取ArrayList类引用
    jclass list_jcs = env->FindClass("java/util/ArrayList");
    //获取ArrayList构造函数id
    jmethodID list_init = env->GetMethodID(list_jcs, "<init>", "()V");
    //创建一个ArrayList对象
    jobject list_obj = env->NewObject(list_jcs, list_init);
    //获取ArrayList对象的add()的methodID
    jmethodID list_add = env->GetMethodID(list_jcs, "add",
                                          "(Ljava/lang/Object;)Z");
    //获取Result类
    jclass res_cls = env->FindClass("com/deeplean/mnninferencelibrary/Result");
    //获取Result的构造函数
    jmethodID res_init = env->GetMethodID(res_cls, "<init>","(DI)V");

    Result result;



    for (auto iter = v_result.cbegin(); iter != v_result.cend(); iter++) {
        result.index = iter->index;
        result.score = iter->score;
        //通过Result的构造函数创建result对象
        jobject res_obj = env->NewObject(res_cls, res_init, (result.score), (result.index));


        env->CallBooleanMethod(list_obj, list_add, res_obj);
    }
    return list_obj;

}

//extern "C"
//
//JNIEXPORT jstring JNICALL
//Java_com_deeplean_mnninferencelibrary_MNNInference_detect(JNIEnv *env, jobject thiz, jstring img_path) {
//    const char *imgPath = env->GetStringUTFChars(img_path, 0);
//    cv::Mat img = cv::imread(imgPath);
//    char* result = androidRecongize->recongize(img);
//    return env->NewStringUTF(result);

//}










