
#include <iostream>
#include<opencv2/opencv.hpp>
#include<algorithm>//sort

using namespace std;
using namespace cv;

uchar minMaxMiddFilter(Mat image,Point start, Mat mask, uchar mode=0) {//최대 최소값 필터

	bool isFirst = true;
	uchar min_max_val = -1;
	uchar current_val = -1;
	vector<uchar> values;
	for (int u = 0;u<mask.rows;u++) {
		for (int v = 0;v<mask.cols;v++) {
			int pointX = v + start.x;
			int pointY = u + start.y;
			current_val = image.at<uchar>(pointY,pointX);
			if (isFirst) {
				min_max_val = current_val;
				isFirst = false;
			}
			
			
			if (mode==0&&current_val<min_max_val) {//최소값
				min_max_val = current_val;
			}
			else if (mode==1&&current_val>min_max_val) {//최대값
				min_max_val = current_val;
			}
			else if (mode == 2) {//중간값 필터링
				values.push_back(current_val);
				sort(values.begin(),values.end());

				min_max_val = values[int(values.size()/2)];
			}

		}
	}
	return min_max_val;
}


void middle_value_filtering(Mat image, Mat& dst, Mat mask) {// 중간값 필터링을 통한 임펄스 노이즈 제거
	//중간값 필터링 
	dst = Mat(image.size(), image.type(), Scalar(0));

	Point center = mask.size() / 2;

	for (int i = center.y; i < image.rows - center.y; i++) {
		for (int j = center.x; j < image.cols - center.x; j++) {
			Point start(Point(j, i) - center);
			dst.at<uchar>(i, j) = minMaxMiddFilter(image, start, mask, 2);
		}
	}


}

void erosion(Mat image,Mat &dst, Mat mask) {// 최소값 마스크를 이용한 침식 연산 

	dst = Mat(image.size(),image.type(),Scalar(0));

	Point center = mask.size() / 2;

	for (int i = center.y;i<image.rows-center.y;i++) {
		for (int j = center.x;j<image.cols-center.x;j++) {
			Point start(Point(j, i) - center);
			dst.at<uchar>(i,j)= minMaxMiddFilter(image,start,mask,0);
		}
	}
	

}

void dilation(Mat image, Mat& dst, Mat mask) {//최대값 마스크를 이용한 팽창 연산 

	dst = Mat(image.size(), image.type(), Scalar(0));

	Point center = mask.size() / 2;

	for (int i = center.y; i < image.rows - center.y; i++) {
		for (int j = center.x; j < image.cols - center.x; j++) {
			Point start(Point(j, i) - center);
			dst.at<uchar>(i, j) = minMaxMiddFilter(image, start, mask, 1);
		}
	}


}


////////////////////////////////////////////////////////////////////////////////////

//열림/닫힘연산//최대 최소필터 이용 

void openCloseMopology(Mat image, Mat& dst, Mat mask, int erosionCount = 1, int dilationCount = 1, uchar mode = 0) {//0이면 열림연산 1이면 닫힘현산 
	dst = Mat(image.size(), CV_8U, Scalar(0));

	if (mode == 0) {//열림연산 
		erosion(image, dst, mask);

		for (int i = 0; i < erosionCount - 1; i++) {
			erosion(dst, dst, mask);

		}

		for (int j = 0; j < dilationCount; j++) {
			dilation(dst, dst, mask);
		}
	}
	else if (mode == 1) {//닫힘연산
		dilation(image, dst, mask);
		for (int i = 0; i < dilationCount; i++) {
			dilation(dst, dst, mask);

		}

		for (int j = 0; j < erosionCount; j++) {
			erosion(dst, dst, mask);
		}


	}

}

///열림 닫힘 저걸로 둘다 만들어 놓기

//////////////////////////////////////////////////////////////////////////
//침식마스크, 팽창마스크를 이용한 침식, 팽창연산

bool check_match(Mat image, Point start, Mat mask, uchar mode=0) {//0이면 침식 마스크, 1이면 팽창마스크 
	
	for (int u = 0;u<mask.rows;u++) {
		for (int v = 0;v<mask.cols;v++) {
			int m=mask.at<uchar>(u,v);//현재 마스크 참조 위치
			int p=image.at<uchar>(start+Point(v,u));//이미지상에서의 위치

			bool check = (p == 255);//p 값이 1임을 확인
			if (m==1&&mode==check) {//mode==1일때는 하나라도 1이면 false 로 반환해서 팽창한다. 
				//mode 0일때는 하나라도 0이면 flase로 반환하여 침식한다.
				return false;
			}


		}
	}

	return true;
}

void maskErosionDilation(Mat image,Mat &dst, Mat mask,uchar mode=0) {//침식마스크를 이용하여 침식연산을 실행 0 침식 1 팽창
	dst = Mat(image.size(),CV_8U,Scalar(0));

	Point center = mask.size() / 2;
	for (int i = center.y;i<dst.rows-center.y;i++) {
		for (int j = center.x;j<dst.cols-center.x;j++) {
			Point start(j-center.x,i-center.y);


			bool check=check_match(image,start,mask,mode);

			if (mode==0&&check) {//만약 mode ==0 이면 침식이다. check 함수에서 모두 1일때 true 즉 여기도 255
				dst.at<uchar>(i, j) = 255;
			}
			else if (mode==1&&!check) {//만약 mode ==1 이면 팽창, check 함수에서 모두 0일때 true 즉 물체가없어서 0
				dst.at<uchar>(i, j) = 255;
			}


		}
	}

}







int main()
{
	Mat image= imread("leaf.jpg", IMREAD_GRAYSCALE);
	CV_Assert(image.data);
	Mat dst_open,dst_close,thresh,middleValue;

	
	uchar mopologyData[] = {
		0,1,0,
		1,1,1,
		0,1,0
	};

	Mat mask(Size(3,3),CV_8U, mopologyData);

	middle_value_filtering(image, middleValue,mask);

		
		threshold(image, thresh, 200, 255, THRESH_BINARY_INV);
		openCloseMopology(thresh,dst_open, mask,5,0,0);//열림연산
		openCloseMopology(thresh, dst_close, mask, 1, 1, 1);//닫힘연산
		imshow("dst_erosion", dst_open);
		imshow("dst_dilation", dst_close);
		imshow("middleValue", middleValue);
		imshow("image", image);
	


	

	waitKey(0);
	
}

