
#include <iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void scailing_forward(Mat image, Mat& dst,Size size) {
 //전방향 사상으로 구현하는 스케일링 정수크기만 가능하다 overlap과 hole 문제가 발생한다. 

    dst = Mat(size,image.type(),Scalar(0));
    
    double xScail = (double)size.width / image.cols;//새로운 넓이의 기존넓이에 대한 비
    double yScail = (double)size.height / image.rows;//

    for (int i = 0;i<image.rows;i++) {
        for (int j = 0;j<image.cols;j++) {
            Point dstPoint = Point((int)(j*xScail),(int)(i*yScail));
            dst.at<uchar>(dstPoint) = image.at<uchar>(i,j);//목적지 위치를 구해서 넣어준다.
        }
    }
}

void scailing_nearest(Mat image,Mat &dst,Size size) {
    dst = Mat(size, image.type(), Scalar(0));
    
    double xScail = (double)size.width / image.cols;//새로운 넓이의 기존넓이에 대한 비
    double yScail = (double)size.height / image.rows;//

    for (int i = 0; i < dst.rows; i++) {
        for (int j = 0; j < dst.cols; j++) {
            Point oriPoint = Point((int)(j / xScail), (int)(i / yScail));//기존이랑 다르게 나누어준다.
            dst.at<uchar>(i,j) = image.at<uchar>(oriPoint);//원본영상의 위치를 구해서 가져오도록 한다.
        }
    }

}

uchar bilinear_value(Mat image,double x, double y) {//양선형 보간을 진행한 값을 만들어준다.
    if (image.cols-1<=x) {
        x--;
    }
    if (image.rows-1<=y) {
        y--;
    }
    Point point(x,y);
    int A = image.at<uchar>(point);
    int B = image.at<uchar>(point + Point(0,1));
    int C = image.at<uchar>(point + Point(1, 0));
    int D = image.at<uchar>(point + Point(1, 1));

    double alpha = x - point.x;
    double betha = y - point.y;

    int E = A + (int)cvRound(alpha * (B - A));
    int F = C + (int)cvRound(alpha * (D - C));

    int G = E + (int)cvRound(betha * (F - E));

    return G;
}

void scailing_bilinear(Mat image, Mat& dst, Size size) {
    dst = Mat(size, image.type(), Scalar(0));

    double xScail = (double)size.width / image.cols;//새로운 넓이의 기존넓이에 대한 비
    double yScail = (double)size.height / image.rows;//

    for (int i = 0; i < dst.rows; i++) {
        for (int j = 0; j < dst.cols; j++) {
            Point oriPoint = Point((int)(j / xScail), (int)(i / yScail));//기존이랑 다르게 나누어준다.
            dst.at<uchar>(i, j) = bilinear_value(image,oriPoint.x,oriPoint.y);//원본영상의 위치를 구해서 그 위치를 기준으로 양선형 보간을 진행한다.
        }
    }

}



int main()
{

    //resize(image,dst1,size(1024,1024),INTER_CUBIC);//3차 회선 보간법
    //resize(image,dst1,size(1024,1024),INTER_LANCZOS4);//lanzos4 방법
    Mat image = imread("test1.jpg",IMREAD_GRAYSCALE);
    Mat dst,dst2;
    CV_Assert(image.data);

    scailing_forward(image,dst,Size(1000,1000));
    scailing_forward(image, dst2, Size(100, 100));

    imshow("dst1", dst);
    imshow("dst2", dst2);
    imshow("image",image);
    waitKey(0);
}

