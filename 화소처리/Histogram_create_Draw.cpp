// Histogram_create_Draw.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void calc_Histo(const Mat& inputMat,Mat& outputArray,int bins,int max_bins=256) {
    int histSize[] = {bins};
    float range[] = { 0,(float)max_bins };
    const float* ranges[] = {range};
     int channels[] = { 0 };

    calcHist(&inputMat,1,channels,Mat(),outputArray,1, histSize,ranges);

   
}



void Draw_Histo(const Mat hist,Mat &output ,Size size=Size(256,200)) { 

    output = Mat(size, CV_8U, Scalar(255));
  
    cout << "hist" << endl << hist.t();
    float bin = (float) output.cols/ hist.rows;
    normalize(hist, hist, 0, output.rows, NORM_MINMAX);
    for (int i = 0;i<hist.rows;i++) {
    
        Point2f pt1(i*bin,0);
        Point2f pt2((i+1)*bin,hist.at<float>(i));

        rectangle(output,Rect2f(pt1,pt2),Scalar(0),-1,LINE_AA);

        
    }
    flip(output,output,0);
}

int serch_terminal_Value(Mat hist,int bias=0) {

    for (int i = 0;i<hist.rows;i++) {
        int idx = abs(bias - i);

        if (hist.at<float>(idx)>0) {
            return idx;
        }
    }


    return -1;
}

int main()
{
   
    Mat image = imread("minMax.jpg",IMREAD_GRAYSCALE);
    
    CV_Assert(image.data);

    

   /* Mat hist, hist_dst, hist_img, hist_dst_img;
    int histsize = 64, ranges = 256;
    calc_Histo(image,hist, histsize,ranges);
    float bin_width = (float)ranges / histsize;
    int low_value = (int)(serch_terminal_Value(hist, 0) * bin_width);
    int high_value = (int)(serch_terminal_Value(hist, hist.rows - 1) * bin_width);
    
    int d_value = high_value - low_value;
    Mat dst = (image - low_value) * (255.0 / d_value);
    
    calc_Histo(dst,hist_dst,histsize,ranges);
    Draw_Histo(hist_dst, hist_dst_img);
    Draw_Histo(hist,hist_img);

    imshow("image",image);
    imshow("hist_dst_img", hist_dst_img);
    imshow("dst",dst);
    imshow("hist_img",hist_img);*/

    waitKey(0);
    return 0;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
