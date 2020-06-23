
#include <iostream>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

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

void doWarping(Mat image, Mat &dst, vector<Rect> dstLines, vector<Rect> oriLines) {
    
    dst = Mat(image.size(),image.type(),Scalar(0));
    Rect rect(Point(0, 0), image.size());
    

    for (int i = 0;i<dst.rows;i++) {
        for (int j = 0;j<dst.cols;j++) {
            
            double x = j;
            double y = i;

            double tx = 0;
            double ty = 0;
            double totoalWeight = 0;

            for (int k = 0;k<dstLines.size();k++) {
                //목적 제어선 위치
                Point P = Point(dstLines.at(k).x, dstLines.at(k).y);
                Point Q = Point(dstLines.at(k).width,dstLines.at(k).height);
                //원본 제어선 위치
                Point _P = Point(oriLines.at(k).x, oriLines.at(k).y);
                Point _Q = Point(oriLines.at(k).width, oriLines.at(k).height);
                //u P에서 임의의점 v 의 수직 교차점까지의 거리
                double u = ((x - P.x) * (Q.x - P.x) + (y - P.y) * (Q.y - P.y)) / (pow(Q.x - P.x, 2) + (pow(Q.y - P.y, 2)));
                //h P-Q 와 V의 수직교차점 c 와 P 간의 거리 
                double h = ((y - P.y) * (Q.x - P.x) - (x - P.x) * (Q.y - P.y)) / (sqrt(pow(Q.x - P.x, 2) + pow(Q.y - P.y, 2)));
               
                //_x_y 목적 영상에 해당하는 원본영상의 위치 v' 가중치가 적용되기 전  
                double _x = _P.x + u * (_Q.x - _P.x) - ((h * (_Q.y - _P.y)) / (sqrt(pow(_Q.x - _P.x, 2) + pow(_Q.y - _P.y, 2))));
                double _y = _P.y + u * (_Q.y - _P.y) + ((h * (_Q.x - _P.x)) / (sqrt(pow(_Q.x - _P.x, 2) + pow(_Q.y - _P.y, 2))));

                double d = 0;
                //제어선과 v 사이의 거리 
                if (u<0) {
                    d = sqrt(pow(x - P.x, 2) + pow(y - P.y, 2));
                }
                else if (u>1) {
                    d = sqrt(pow(x - Q.x, 2) + pow(y - Q.y, 2));
                }
                else{

                    d = abs(h);
                
                }

                double a = 0.01;
                double b = 1;
                double p = 0.5;
                //가중치 계산
                double weight = pow((pow(sqrt(pow(Q.x - P.x, 2) + pow(Q.y - P.y, 2)), p) / (a + d)), b);
               
                tx = tx + (_x - x) * weight;
                ty = ty + (_y - y) * weight;

                totoalWeight += weight;
            }
            //사용할 원본 위치 계산
            double X = x + tx / totoalWeight;
            double Y = y + ty / totoalWeight;
           
            if (rect.contains(Point(int(X), int(Y)))) {
                dst.at<uchar>(i, j) = bilinear_value(image, X, Y);
            }
            else {
                dst.at<uchar>(i, j) = 255;
            }
            
        }
    }



}
vector<Rect> warpingBaseCodination(Mat image) {
    vector<Rect> base_cord = {
      Rect(0,0,0,image.rows - 1),
       Rect(0, 0, image.cols - 1, 0),
       Rect(0, image.rows - 1, image.cols - 1, image.rows - 1),
       Rect(image.cols - 1, image.rows - 1,image.cols - 1,0),
    };
    return base_cord;
}

Mat image = imread("lenna.png", IMREAD_GRAYSCALE);
Mat line_draw;
Mat dst;
vector<Rect> ori_cord = warpingBaseCodination(image);
vector<Rect> dst_cord = warpingBaseCodination(image);

void mouseCallback(int event, int x, int y, int flags, void *) {
    
    static Point pl(-1,-1);
    static Point pr(-1, -1);
    if (event==EVENT_LBUTTONDOWN) {
        if (pl.x == -1)
            pl = Point(x, y);
        else {
            ori_cord.push_back(Rect(pl, Point(x, y)));
            line(line_draw, pl, Point(x, y), Scalar(255));
            pl = Point(-1, -1);
         
            cout << 0;


        }

    }
    else if (event == EVENT_RBUTTONDOWN) {
        
        if (pr.x == -1)
            pr = Point(x, y);
        else {
            dst_cord.push_back(Rect(pr, Point(x, y)));
            line(line_draw, pr, Point(x, y), Scalar(255));
            doWarping(image, dst, dst_cord, ori_cord);
            imshow("dst", dst);
            cout << 1;
            pr = Point(-1, -1);
        }
    
    }

    imshow("line_draw", line_draw);
}


int main()
{
   
    CV_Assert(image.data);
    image.copyTo(line_draw);




    namedWindow("dst", WINDOW_AUTOSIZE);

    setMouseCallback("dst", mouseCallback);

    doWarping(image, dst, dst_cord, ori_cord);
    imshow("line_draw", line_draw);
    imshow("dst", dst);

    waitKey(0);

}

