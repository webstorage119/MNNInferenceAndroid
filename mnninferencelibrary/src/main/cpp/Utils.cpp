//
// Created by ty on 2020/7/21.
//

#include "Utils.h"





std::string getDirPath(JNIEnv *env, jstring dirPath) {
    if (NULL == dirPath) {
        return nullptr;
    }
    const char *file_path = env->GetStringUTFChars(dirPath, 0);
    if (NULL == file_path) {
        return nullptr;
    }
    std::string t_file_path = file_path;
    std::string tLastChar = t_file_path.substr(t_file_path.length() - 1, 1);
    //目录补齐/
    if ("\\" == tLastChar) {
        t_file_path = t_file_path.substr(0, t_file_path.length() - 1) + "/";
    } else if (tLastChar != "/") {
        t_file_path += "/";
    }
    return t_file_path;
}

std::string getFilePath(JNIEnv *env, jstring filePath) {
    if (NULL == filePath) {
        return nullptr;
    }
    const char *file_path = env->GetStringUTFChars(filePath, 0);
    if (NULL == file_path) {
        return nullptr;
    }
    return file_path;
}

