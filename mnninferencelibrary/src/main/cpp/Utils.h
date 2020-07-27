//
// Created by ty on 2020/7/21.
//

#ifndef MNNINFERENCEANDROID_UTILS_H
#define MNNINFERENCEANDROID_UTILS_H

#include <string>
#include <vector>
#include <jni.h>



std::string getDirPath(JNIEnv *env, jstring dirPath);

std::string getFilePath(JNIEnv *env, jstring filePath);

typedef struct Result {
    double score;
    int index;
} result;


#endif //MNNINFERENCEANDROID_UTILS_H
