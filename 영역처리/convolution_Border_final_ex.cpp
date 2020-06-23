#include <iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;


void convolution_valid(Mat image, Mat &dst,Mat mask){//중첩되는 부분만 처리하는 컨볼루젼

    dst = Mat(image.size(),CV_32F,Scalar(0));

    Point center = mask.size() / 2;

    int pointX = 0;
    int pointY = 0;
    float sum = 0;
    for (int i = center.y;i<image.rows-center.y;i++) {
        for (int j = center.x;j<image.cols-center.x;j++) {
            sum = 0;
            for (int u = 0;u<mask.rows;u++) {
                for (int v = 0;v<mask.cols;v++) {
                    pointY = i + u - center.y;
                    pointX = j + v - center.x;

                    sum += (float)image.at<uchar>(pointY,pointX)*mask.at<float>(u,v);

                }
            }
            dst.at<float>(i, j) = sum;

        }
    }

}

void convolution_borderExtension(Mat image, Mat& dst, Mat mask, uchar mode, float value=0) {//끝 부분을 늘리거나, 임의의 상수 값을 넣어 끝 부분을 추가
  

    if (mode==0) {  //mode==0 0을 기본적으로 넣는다. mode==1 사용자가 입력한 값을 기본적으로 넣는다.  
        value = 0;
    }

    dst = Mat(image.size(),CV_32F,Scalar(0));

    Point center = mask.size() / 2;

    int pointX = 0;
    int pointY = 0;

    float sum = 0;

    for (int i = 0;i<image.rows;i++) {
        for (int j = 0;j<image.cols; j++) {
            sum = 0;
            for (int u = 0;u<mask.rows;u++) {
                for (int v = 0;v<mask.cols;v++) {
                    pointY = i + u - center.y;
                    pointX = j + v - center.x;

                    if (pointX>=image.cols||pointX<0||pointY<0||pointY>=image.rows) {
                        sum = value;
                    }
                    else {
                        sum += (float)image.at<uchar>(pointY,pointX)*mask.at<float>(u,v);
                    }
                }
            }

            dst.at<float>(i, j) = sum;
        }
    }
}

void convolution_mirror(Mat image, Mat& dst, Mat mask) {//경계부분에 거울을 가져다 댄것 처럼 값을 복사해 처리
    dst = Mat(image.size(), CV_32F, Scalar(0));

    Point center = mask.size() / 2;

    int pointX = 0;
    int pointY = 0;

    float sum = 0;

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {

            sum = 0;
            for (int u = 0; u < mask.rows; u++) {
                for (int v = 0; v < mask.cols; v++) {

                    pointY = i + u - center.y;
                    pointX = j + v - center.x;

                   
                       
                   
                    if (pointX<0) {
                        pointX *= -1;//음의 수로 갈 경우 양수로 반전
                    }
                    else if (pointX>=image.cols) {
                        pointX = image.cols - (pointX - (image.cols - 1));//넘어갈경우 넘어간 변위를 구해서 길이에서 빼줌
                    }

                    if (pointY < 0) {
                        pointY *= -1;//음의 수로 갈 경우 양수로 반전
                    }
                    else if (pointY >=image.rows) {
                        pointY = image.rows - (pointY - (image.rows - 1));//넘어갈 경우 넘어간 변위를 구해서 높이에서 빼줌
                    }
                    
                    sum += (float)image.at<uchar>(pointY, pointX) * mask.at<float>(u, v);

                }
            }

            dst.at<float>(i, j) = sum;

        }
    }
}

void convolution_wraping(Mat image, Mat& dst, Mat mask) {//위 아래를 만것처럼 컨볼루젼
    dst = Mat(image.size(), CV_32F, Scalar(0));

    Point center = mask.size() / 2;

    int pointX = 0;
    int pointY = 0;

    float sum = 0;

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {

            sum = 0;
            for (int u = 0; u < mask.rows; u++) {
                for (int v = 0; v < mask.cols; v++) {

                    pointY = i + u - center.y;
                    pointX = j + v - center.x;

                    if (pointX < 0) {
                        pointX = image.cols + pointX;
                    }
                    else if (pointX >= image.cols) {
                        pointX = pointX - image.cols;
                    }

                    if (pointY < 0) {
                        pointY = image.rows + pointY;
                    }
                    else if (pointY >= image.rows) {
                        pointY = image.rows - image.rows;
                    }

                    sum += (float)image.at<uchar>(pointY, pointX) * mask.at<float>(u, v);

                }
            }

            dst.at<float>(i, j) = sum;

        }
    }
}
/////////////////////////////////////////

void EdgeFinder(Mat image, Mat& total_img, Mat& dst_v, Mat& dst_h, Mat mask_h, Mat mask_v, int thresh = 200, int thresh_max = 255, uchar mode = 0) {
    //엣지를 찾아준다. totalimg_는 thresh 가 적용되고 mode==0 Thresh_bin mode==1 Thresh_bin_inv
    convolution_mirror(image, dst_v, mask_v);
    convolution_mirror(image, dst_h, mask_h);

    magnitude(dst_h, dst_v, total_img);

    dst_v.convertTo(dst_v, CV_8U);
    dst_h.convertTo(dst_h, CV_8U);
    total_img.convertTo(total_img, CV_8U);
    if (mode == 0) {
        threshold(total_img, total_img, thresh, thresh_max, THRESH_BINARY);
    }
    else if (mode == 1) {
        threshold(total_img, total_img, thresh, thresh_max, THRESH_BINARY_INV);
    }

}


/////////////////////////////////////////////////////////////////// 필터 종류정의

float BLUR_FILTER_33[] = {
1 / 9.f,1 / 9.f ,1 / 9.f,
1 / 9.f,1 / 9.f ,1 / 9.f,
1 / 9.f,1 / 9.f ,1 / 9.f,
};

float BLUR_FILTER_55[] = {
1 / 25.f,1 / 25.f,1 / 25.f,1 / 25.f,1 / 25.f,
1 / 25.f,1 / 25.f,1 / 25.f,1 / 25.f,1 / 25.f,
1 / 25.f,1 / 25.f,1 / 25.f,1 / 25.f,1 / 25.f,
1 / 25.f,1 / 25.f,1 / 25.f,1 / 25.f,1 / 25.f,
1 / 25.f,1 / 25.f,1 / 25.f,1 / 25.f,1 / 25.f,

};


float SHARPEN_FILTER_33_STRONG5[] = {

    0,-1,0,
    -1,5,-1,
    0,-1,0
};

float SHARPEN_FILTER_33_STRONG9[] = {

    -1,-1,-1,
    -1,9,-1,
    -1,-1,-1
};

float EDGE_SOBEL_VERTICAL_FILTER_33[] = {
       1,0,-1,
       2,0,-2,
       1,0,-1,
};
float EDGE_SOBEL_HORIZONTAL_FILTER_33[] = {
    -1,-2,-1,
     0,0,0,
    1,2,1,
};

float EDGE_PREWITT_VERTICAL_FILTER_33[] = {
       1,0,-1,
       1,0,-1,
       1,0,-1,
};
float EDGE_PREWITT_HORIZONTAL_FILTER_33[] = {
    -1,-1,-1,
     0,0,0,
    1,1,1,
};

float  EDGE_ROBERTS_VERTICAL_FILTER_33[] = {
     0,0,-1,
    0,1,0,
    0,0,0
};

float  EDGE_ROBERTS_HORIZONTAL_FILTER_33[] = {
    -1,0,0,
    0,1,0,
    0,0,0

};



/////////////////////필터






int main()
{
    Mat image = imread("test2.jpg",IMREAD_GRAYSCALE);
    CV_Assert(image.data);
    Mat dst_v,dst_h,dst_total;
   

    Mat mask_v(Size(3,3),CV_32F, EDGE_ROBERTS_VERTICAL_FILTER_33);
    Mat mask_h(Size(3, 3), CV_32F, EDGE_ROBERTS_HORIZONTAL_FILTER_33);

    EdgeFinder(image,dst_total,dst_v,dst_h,mask_v,mask_h,30,255,0);

    imshow("dst_total", dst_total);
    imshow("dst_V", dst_v);
    imshow("dst_h", dst_h);
    imshow("image",image);
    waitKey(0);

}
