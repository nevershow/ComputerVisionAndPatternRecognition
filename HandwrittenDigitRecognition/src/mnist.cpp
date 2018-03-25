#include "mnist.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <string>
#include <iostream>

using namespace std;
using namespace cv;

//大端存储转换为小端存储
int reverseInt(int i) {
    unsigned char c1, c2, c3, c4;

    c1 = i & 255;
    c2 = (i >> 8) & 255;//右移位　与操作
    c3 = (i >> 16) & 255;
    c4 = (i >> 24) & 255;

    return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}


SVM svm;

Mat read_mnist_image(const string fileName) {
    int magic_number = 0;
    int number_of_images = 0;
    int n_rows = 0;
    int n_cols = 0;

    Mat DataMat;

    const char* charFileName = fileName.c_str();//string 转换为 char*
    ifstream file(charFileName,ios::binary);

    if (file.is_open()) {
        cout << "成功打开图像集 ... \n";

        file.read((char*)&magic_number, sizeof(magic_number));
        file.read((char*)&number_of_images, sizeof(number_of_images));
        file.read((char*)&n_rows, sizeof(n_rows));
        file.read((char*)&n_cols, sizeof(n_cols));

        magic_number = reverseInt(magic_number);
        number_of_images = reverseInt(number_of_images);
        n_rows = reverseInt(n_rows);
        n_cols = reverseInt(n_cols);
        cout << "MAGIC NUMBER = " << magic_number
            << " ;NUMBER OF IMAGES = " << number_of_images
            << " ; NUMBER OF ROWS = " << n_rows
            << " ; NUMBER OF COLS = " << n_cols << endl;

        cout << "开始读取Image数据......\n";
        DataMat = Mat::zeros(number_of_images, n_rows * n_cols, CV_32FC1);
        for (int i = 0; i < number_of_images; i++) {
            for (int j = 0; j < n_rows * n_cols; j++) {
                unsigned char temp = 0;
                file.read((char*)&temp, sizeof(temp));
                float pixel_value = float((temp + 0.0) / 255.0);
                DataMat.at<float>(i, j) = pixel_value;
            }
        }
        cout << "读取Image数据完毕......\n";
        file.close();
        return DataMat;
    }

    cout << fileName << " dose not exist.\n";
    exit(0);
}

Mat read_mnist_label(const string fileName) {
    int magic_number;
    int number_of_items;

    Mat LabelMat;

    const char* charFileName = fileName.c_str();
    ifstream file(charFileName, ios::binary);
    if (file.is_open()) {
        cout << "成功打开Label集 ... \n";

        file.read((char*)&magic_number, sizeof(magic_number));
        file.read((char*)&number_of_items, sizeof(number_of_items));
        magic_number = reverseInt(magic_number);
        number_of_items = reverseInt(number_of_items);

        cout << "MAGIC NUMBER = " << magic_number << "  ; NUMBER OF ITEMS = " << number_of_items << endl;

        cout << "开始读取Label数据......\n";
        LabelMat = Mat::zeros(number_of_items, 1, CV_32SC1);
        for (int i = 0; i < number_of_items; i++) {
            unsigned char temp = 0;
            file.read((char*)&temp, sizeof(temp));
            LabelMat.at<unsigned int>(i, 0) = (unsigned int)temp;
        }
        cout << "读取Label数据完毕......\n";
        file.close();
        return LabelMat;
    }
    cout << fileName << " dose not exist.\n";
    exit(0);
}

void train() {
    const char* model = "mnist_svm.xml";
    ifstream file(model);
    if (file.is_open()) {
    //------------------------ load the svm ----------------------------------------------------
        cout << "开始导入SVM文件...\n";
        svm.load(model);
        cout << "成功导入SVM文件...\n";
    }
    else {
        //--------------------- 1. Set up training data ---------------------------------------
        Mat trainData;
        Mat labels;
        string trainImage = "../data/trainset/train-images.idx3-ubyte";
        string trainLabel = "../data/trainset/train-labels.idx1-ubyte";
        trainData = read_mnist_image(trainImage);
        labels = read_mnist_label(trainLabel);

        cout << trainData.rows << " " << trainData.cols << endl;
        cout << labels.rows << " " << labels.cols << endl;

        //------------------------ 2. Set up the support vector machines parameters --------------------
        cv::SVMParams params;
        params.svm_type = SVM::C_SVC;

        params.kernel_type = SVM::POLY;
        params.C = 10.0;
        params.gamma = 0.01;
        params.degree= 3;
        params.term_crit = cv::TermCriteria(CV_TERMCRIT_ITER, 1000, FLT_EPSILON);

        svm.get_params();

        //------------------------ 3. Train the svm ----------------------------------------------------
        cout << "Starting training process" << endl;
        svm.train(trainData, labels,Mat(),Mat(),params);
        cout << "Finished training process...\n";

        //------------------------ 4. save the svm ----------------------------------------------------
        svm.save("mnist_svm.xml");
        cout << "save as mnist_svm.xml" << endl;
    }
}

int predict(string name) {
    Mat gray = imread(name.c_str()), img, img_scaled;
    cvtColor(gray, img, CV_RGB2GRAY);
    img.convertTo(img, CV_32FC1, 1.0 / 255.0);
    resize(img, img_scaled, cv::Size(28, 28), 0, 0, CV_INTER_LINEAR);
    return svm.predict(img_scaled.reshape(1, 1));
}
int main(int argc, char const *argv[]) {
    train();
    cout << predict(argv[1]) << endl;
    return 0;
}

