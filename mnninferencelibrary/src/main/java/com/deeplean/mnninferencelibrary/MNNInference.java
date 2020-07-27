package com.deeplean.mnninferencelibrary;

import android.content.Context;
import android.os.Environment;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.List;

public class MNNInference {

    static {
        System.loadLibrary("mnnInferenceLibrary");
    }

    public boolean init(Context mContext) {
        String path = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + "mnn";
        String modelPath = copyModel2SD(mContext, "model", path);
        String mnnPath = modelPath + File.separator + "shufflenetv2.mnn";
        boolean ret = initRecongize(mnnPath, 224, 224, 2, false);
        if (!ret) {
            return false;
        }
        return true;
    }

    public native List<Result> recongize(String imgPath);
//    public native String detect(String imgPath);
    public native boolean initRecongize(String mnnPath, int resize_width, int resize_height, int num_thread, boolean open_cl);


    private String copyModel2SD(Context mContext, String model, String path) {
        String modelPath = path + File.separator + model;
        File file = new File(modelPath);
        if (!file.exists()) {
            file.mkdir();
        }
        try {
            copyAssets(mContext, model, modelPath);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return modelPath;
    }

    /**
     * 复制asset文件到指定目录
     *
     * @param mContext 上下文环境
     * @param oldPath  asset下的路径
     * @param newPath  SD卡下保存路径
     * @throws IOException 文件拷贝异常
     */
    public static void copyAssets(Context mContext, String oldPath, String newPath) throws IOException {
        String fileNames[] = mContext.getAssets().list(oldPath);
        if (fileNames.length > 0) {
            File file = new File(newPath);
            file.mkdirs();
            for (String fileName : fileNames) {
                copyAssets(mContext, oldPath + File.separator + fileName, newPath + File.separator + fileName);
            }
        } else {
            InputStream is = mContext.getAssets().open(oldPath);
            FileOutputStream fos = new FileOutputStream(new File(newPath));
            byte[] buffer = new byte[1024];
            int byteCount = 0;
            while ((byteCount = is.read(buffer)) != -1) {
                fos.write(buffer, 0, byteCount);
            }
            fos.flush();
            is.close();
            fos.close();
        }
    }


}
