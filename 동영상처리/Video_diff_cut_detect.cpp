
#include <iostream>
#include<opencv2/opencv.hpp>


using namespace std;
using namespace cv;
//grayscale 이미지만 처리 할 수 있다.
void thresh_image(Mat image, Mat &dst,int thresHold, int maxVal) {
    if (image.type() != CV_8U) {
        cvtColor(image,image,COLOR_BGR2GRAY);
        cout << "\nthresh_image: image is not gray scale change internaly"<<endl;
    }

    //이미지를 threshhold로 정리해주고, 이진화 시켜준다.즉 마스크를 만들어줌
   
    dst = Mat(image.size(), CV_8U, Scalar(0));// 마스크 행렬 초기화

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            if (image.at<uchar>(i, j) >= thresHold) {
                dst.at<uchar>(i, j) = maxVal;
            }
            else {
                dst.at<uchar>(i, j) = 0;
            }
        }
    }

}

void masking_image(Mat image,Mat &dst, Mat mask) {// 이미지에서 상응하는 마스크에서 0인 부분을 제거해줌
    //이미지와 마스크를 주면 마스크가 적용된 이미지를 반환
    
    image.copyTo(dst);

    for (int i = 0; i < mask.rows; i++) {
        for (int j = 0; j < mask.cols; j++) {
            if (mask.at<uchar>(i, j) > 0) {

            }
            else {
                dst.at<Vec3b>(i, j)[0] = 0;
                dst.at<Vec3b>(i, j)[1] = 0;
                dst.at<Vec3b>(i, j)[2] = 0;


            }
        }
    }
}



void diff_bin_Image(Mat image1,Mat image2, Mat &output_mask, Mat &dst_Image,int threshHold=50, int maxVal=255) {
    //차영상을 만들어주는 함수, thresh_image,masking_image 함수를 이용한다.
    //단 흑백으로 차영상을 만들어줌

// 차영상만 구할 땐diff 이미지 하나 쓰면 된다.
    Mat image1_gray, image2_gray, diff;

    cvtColor(image1, image1_gray, COLOR_BGR2GRAY);
    cvtColor(image2, image2_gray, COLOR_BGR2GRAY);

    absdiff(image1_gray, image2_gray, diff);//두 영상을 뺀다.

    thresh_image(diff,output_mask,threshHold,maxVal);//마스크를 만든다.

    masking_image(image1,dst_Image,output_mask);



}

//////////////////////////////////////////////////////////////////////////여기서 부터 영상 컷 확인 함수

void calc_Histo(const Mat &image, Mat & hist, int bins, int range_max=256) {//히스토그램 생성함수

    int histSize[] = { bins };
    float range[] = { 0,(float)range_max };
    int channels[] = { 0 };

    const float* ranges[] = { range };

    calcHist(&image, 1, channels, Mat(), hist, 1, histSize, ranges);

}


int calc_histo_diff(Mat lastFrame, Mat currentFrame) {//두 프레임의 hist차이를 반환해주는 함수
    if (lastFrame.type()!=CV_8U||currentFrame.type()!=CV_8U) {
        cvtColor(lastFrame, lastFrame, COLOR_BGR2GRAY);
        cvtColor(currentFrame, currentFrame, COLOR_BGR2GRAY);
        cout << "calc_histo_diff: cvtColor()" << endl;
    }
    Mat last_hist=Mat(256, 1, CV_32F, Scalar(0));
    Mat current_hist = Mat(256, 1, CV_32F, Scalar(0));

    calc_Histo(lastFrame, last_hist,256);
    calc_Histo(currentFrame, current_hist, 256);

    int diff_sum = 0;
    for (int i = 0;i<256;i++) {
        diff_sum += abs(last_hist.at<float>(i) - current_hist.at<float>(i));
    }
    return diff_sum;
}




int main()
{
   
    

    VideoCapture cap(0);
    cap.open("./VideoTest/test_news.mp4");
    CV_Assert(cap.isOpened());

    Mat frame;
    Mat last_frmae;
    //딜레이 설정
    int delay = 1000/cap.get(CV_CAP_PROP_FPS);
    //넓이 높이
    int width = cap.get(CAP_PROP_FRAME_WIDTH);
    int height = cap.get(CAP_PROP_FRAME_HEIGHT);
    int threshold = height * width+1000;
    int counter = 0;



    while (waitKey(delay)!=27&& cap.read(frame)) {
      

        Mat gray_frame;
        cvtColor(frame,gray_frame,COLOR_BGR2GRAY);
        //frame.copyTo(gray_frame);
        //처음 일때, last_frmae 초기화
        if (counter==0) {
            gray_frame.copyTo(last_frmae);
            counter++;
            continue;
        }
      
        Mat hist;
       

        int sum_histo_differ = calc_histo_diff(last_frmae,gray_frame);
        

        if (sum_histo_differ>threshold) {
            cout << "Scene chang detected: " << counter << endl;
            gray_frame.copyTo(last_frmae);
            imshow("Detected cut: frame"+to_string(counter),frame);
        }
        
        imshow("frame",frame);
        counter++;
    }

    cap.release();

    waitKey(0);
    
}
