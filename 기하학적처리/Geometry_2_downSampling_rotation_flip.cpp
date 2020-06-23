// Geometry_2_downSampling_rotation_flip.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;
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
void downSampling(Mat image, Mat &dst, int scaleFactor) {//픽셀을 건너 뛰는 방식으로 이미지를 축소한다. 이것도 역변환이다. 하지만 여전히 계단현상이나, 특징이 사라지는 문제점이 있다. 
    dst = Mat(image.size() / scaleFactor, CV_8U, Scalar(0));
    for (int i = 0;i<dst.rows;i++) {
        for (int j = 0;j<dst.cols;j++) {
            dst.at<uchar>(i, j) = image.at<uchar>(i*scaleFactor,j*scaleFactor);
        }
    }

}

///////////////////////////////////////////////////////
//회전 함수 구현

uchar bilinear_value(Mat image, double x, double y) {//양선형 보간을 진행한 값을 만들어준다.
    if (image.cols - 1 <= x) {
        x--;
    }
    if (image.rows - 1 <= y) {
        y--;
    }
    Point point(x, y);
    int A = image.at<uchar>(point);
    int B = image.at<uchar>(point + Point(0, 1));
    int C = image.at<uchar>(point + Point(1, 0));
    int D = image.at<uchar>(point + Point(1, 1));

    double alpha = x - point.x;
    double betha = y - point.y;

    int E = A + (int)cvRound(alpha * (B - A));
    int F = C + (int)cvRound(alpha * (D - C));

    int G = E + (int)cvRound(betha * (F - E));

    return G;
}

void rotation(Mat image, Mat& dst, double degree) {
    
    double radian = degree / 180 * CV_PI;
    double sin_value = sin(radian);
    double cos_value = cos(radian);

    

    int imageW = image.size().width;
    int imageH = image.size().height;
    double size_radian = (90 - degree) / 180 * CV_PI;
    double cos_90 = cos(size_radian);
    dst = Mat(Size(imageH*cos_90+imageW*cos_value,imageH*cos_value+imageW*cos_90),image.type(),Scalar(0));

    Rect bounding_image_Rect = Rect(Point(0, 0), image.size());//이미지가 dst를 넘어가면 에러를 방지하기 위한 rect
    Rect bounding_dst_Rect = Rect(Point(0, 0), dst.size());
    Point center = image.size() / 2;
    int Height = image.size().height - 1;

    long adding_width = ((dst.size().width - image.size().width) / 2);
    long adding_height = ((dst.size().height - image.size().height) / 2);

    for (int i = 0;i<dst.rows;i++) {
        for (int j = 0;j<dst.cols;j++) {
             
            int new_i=i-adding_height;//새롭게 커진 이미지에서의 좌표
            int new_j=j-adding_width;//새롭게 커진 이미지에서의 좌표

            //반시계방향 회전
            double x = ((cos_value *(new_j -center.x)) +(Height - new_i -center.y) * sin_value)+center.x;//중심점을 기준으로 회전
            double y =  Height-((cos_value * (Height - new_i-center.y))- sin_value * (new_j - center.x) +center.y);//중심점을 기준으로 회전 
            
            /*수학 좌표계 아닌버전 시계방향 회전
            double x = (cos_value *(j-center.x)) +(i-center.y) * sin_value+center.x;//중심점을 기준으로 회전
            double y =  (cos_value * (i-center.y))- sin_value * (j - center.x) +center.y;//중심점을 기준으로
            */

           
            if (bounding_image_Rect.contains(Point(x,y))) {
                if(bounding_dst_Rect.contains(Point(j,i)))
                dst.at<uchar>(i, j) = bilinear_value(image, x, y);
            }
        }
    }

}

void symmetry(Mat image, Mat &dst,int type) {
    dst = Mat(image.size(), CV_8UC1);

    for (int i = 0;i<dst.rows;i++) {
        for (int j = 0;j<dst.cols;j++) {
            if (type==0) {//x축 대칭
                dst.at<uchar>(i, j) = image.at<uchar>((image.rows-1)-i,j);
            }
            else if (type==1) {//y축 대칭
                dst.at<uchar>(i, j) = image.at<uchar>(i,(image.cols-1)-j);
            }
            else if (type==2) {
                dst.at<uchar>(i, j) = image.at<uchar>((image.rows - 1) - i, (image.cols - 1) - j);
            }
        }
    }
}

void translation(Mat image,Mat &dst, Point pt) {
    Rect rect(Point(0, 0), image.size());
    dst = Mat(image.size(),CV_8U,Scalar(0));
    for (int i = 0;i<dst.rows;i++) {
        for (int j = 0;j<dst.cols;j++) {
            Point dst_pt(j,i);//dst 좌표
            Point img_pt = dst_pt - pt;//dst 이전 영상인 원본영상의 좌표
            if(rect.contains(img_pt))
            dst.at<uchar>(dst_pt) = image.at<uchar>(img_pt);
        }
    }
}


int main()
{
    Mat image = imread("test2.jpg",IMREAD_GRAYSCALE);
    CV_Assert(image.data);
    Mat scale,mask,blur,rot,sym,tran;

    float data[] = {
    1/9.f,1 / 9.f,1 / 9.f,
    1 / 9.f,1 / 9.f,1 / 9.f,
    1 / 9.f,1 / 9.f,1 / 9.f,
    };

    mask = Mat(Size(3,3),CV_32F,data);
    convolution_wraping(image, blur,mask);//다운샘플링의 문제 해결
    blur.convertTo(blur,CV_8U);//컨볼루젼 결과는 32F 형식
    downSampling(blur,scale,2);
    symmetry(image,sym,2);
    rotation(image, rot,90);
    translation(image, tran,Point(50,50));

    imshow("sym", sym);

    imshow("tran", tran);
    imshow("scale",scale);
    imshow("rot", rot);
    imshow("blur", blur);
    waitKey(0);
    
}
