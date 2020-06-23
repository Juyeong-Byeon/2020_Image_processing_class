// DFT_1_2_Furior.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat DFT_1D(Mat one_row, int dir) {//1차원의 퓨리에 변환 정변환과 역변환의 차이는 전체수로 나누어주냐 나누어주냐 차이임. 그래서 동일 함수로 구현
    int N = one_row.cols;//전체 개수
    Mat dst(one_row.size(), CV_32FC2);//결과 영상이 들어갈 목적 행렬 실수와 허수를 각각 담아야 하기때문에 2채널.

    for (int k = 0; k < N; k++) {//전체에 비교할 요소 선택

        Vec2f complex(0, 0);//값을 누적 시켜줌. 허수와 실수 각각

        for (int n = 0; n < N; n++) {//전체를 돌면서 요소를 비교한다.
            float theta = dir * -2 * CV_PI * k * n / N;//역변환과 정방향 변환에따라 부호를 바꿔줌.
            Vec2f value = one_row.at<Vec2f>(n);//원본영상 0~끝까지

            complex[0] += value[0] * cos(theta) - value[1] * sin(theta);//value[0] 실수 value[1] 허수부
            complex[1] += value[1] * cos(theta) + value[0] * sin(theta);//허수 계산 후 

        }
        dst.at<Vec2f>(k) = complex;//모두 누적한 실수와 허수를 체널별로 넣어준다. 
    }


    if (dir == -1) {
        dst /= N;
    }

    return dst;
}

void DFT_2D(Mat complex_img, Mat& dst, int dir) {//이미지에 대한 퓨리에 변환
    complex_img.convertTo(complex_img, CV_32F);//계산을 하기 위해서는 float 이 적합
    Mat tmp(complex_img.size(), CV_32FC2, Vec2f(0, 0));//임시변수로 우리가 받아온 원본이미지를 저장하고 있다. 
    tmp.copyTo(dst);//dst 초기화

    //2번 퓨리에 변환을 해야하기 때문에 tmp 이용
    for (int i = 0; i < complex_img.rows; i++) {
        Mat dft_row = DFT_1D(complex_img.row(i), dir);//DFT_1D를 이용해 한줄씩 돌아가면서 퓨리에 변환 
        dft_row.copyTo(tmp.row(i));
    }
    transpose(tmp, tmp);//전치
    for (int i = 0; i < tmp.rows; i++) {
        Mat dft_row = DFT_1D(tmp.row(i), dir);
        dft_row.copyTo(dst.row(i));//한번 더 퓨리에 변환 후 목적 영상에 넣어줌.
    }
    //다시 전치 시켜주어야 한다.

    transpose(dst, dst);
}

void log_mag(Mat complex_mat, Mat& dst) {
    Mat planes[2];
    split(complex_mat, planes);

    magnitude(planes[0], planes[1], dst);//벡터의 크기를 구해주게됨

    log(dst + 1, dst);//log 적용 0이면 무한대로 가기 때문에 1을 더해줌.

    normalize(dst, dst, 0, 255, CV_MINMAX);

    dst.convertTo(dst, CV_8U);
}

void shuffling(Mat mag_img, Mat& dst) {
    //중심점
    int cx = mag_img.cols / 2;
    int cy = mag_img.rows / 2;
    Rect q1(cx, 0, cx, cy);//1사분면
    Rect q2(0, 0, cx, cy);//2사분면
    Rect q3(0, cy, cx, cy);//3사분면
    Rect q4(cx, cy, cx, cy);//4사분면

    dst = Mat(mag_img.size(), mag_img.type());

    mag_img(q1).copyTo(dst(q3));
    mag_img(q3).copyTo(dst(q1));
    mag_img(q2).copyTo(dst(q4));
    mag_img(q4).copyTo(dst(q2));

}


int main()
{

    Mat image = imread("lenna.png", IMREAD_GRAYSCALE);
    CV_Assert(image.data);

    Mat complex_img, dft_coef, dft_img, shuffling_img, idft_coef, idft_img[2];
    Mat tmp[] = { image,Mat::zeros(image.size(),CV_8U) };//실수부와 허수부를 담는 행렬
    merge(tmp, 2, complex_img);//합쳐서 한 영상으로 만들어줌.
    DFT_2D(complex_img, dft_coef, 1);//complex_img-> dft_coef(푸리에 변환된 수치값);
    //1 벡터를 구한다.
  //2 로그함수를 적용
  //3 노멀라이징 한다. 
    log_mag(dft_coef, dft_img);//보이게 하기 위해서 로그함수 이용
    shuffling(dft_img, shuffling_img);//셔플링을 통해 가운대로 저주파 성분들을 모아준다.



    DFT_2D(dft_coef, idft_coef, -1);//역변환을 통해 원래 값들을 만들어준다.
    split(idft_coef, idft_img);//실 허 분리

    idft_img[0].convertTo(idft_img[0], CV_8U);//0번째 성분이 사진



    imshow("dft_img", dft_img);
    imshow("image", image);
    imshow("shuffling_img", shuffling_img);
    imshow("idft_img[0]", idft_img[0]);
    cout << "end" << endl;
    waitKey(0);
    return 0;
}
