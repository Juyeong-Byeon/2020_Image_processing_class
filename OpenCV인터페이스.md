# openCV 인터페이스 


## 윈도우 함수
```c++
namedWindow("윈도우 이름",WINDOW_AUTOSIZE);
//윈도우 이름을 정하고, 해당 이름으로 윈도우를 생성한다. 
imshow("윈도우 이름",Mat());
//윈도우 이름의 윈도우에 mat행렬을 영상으로 표시한다. 
destroyWindow("윈도우 이름");
// 인수로 지정된 타이틀의 윈도우를 파괴한다.
destroyAllWindows();
//모든 윈도우들을 파괴한다.
moveWindow(Mat(),x,y);
// 이동하고 싶은 위치로 윈도우를 이동한다. 
resizeWindow(Mat(),width,height);
//윈도우의 크기를 조절한다.
```
## 이벤트 처리 함수

키보드 이벤트, 마우스 이벤트 트랙바 이벤트 등을 지원한다.

## 키보드 이벤트 
```c++
waitKey(0);
// key 입력을 인수로 받는 millsec 만큼 기다리고 int 형으로 반환한다. 입력이 X<=0 일경우 무한대기
'a' //a키
0x250000//왼쪽 화살표
0x270000//오른쪽 화살표
0x260000//윗쪽 화살표
0x280000//아래쪽 화살표 
```

## 마우스 이벤트 함수
```c++
setMouseCallback("창 이름", onMouse, void * userdata);//사용자가 정의한 이벤트 함수를 시스템에 등록해준다.
mouseClick(event e, int x, int y, int flags, userdata);//발생한 마우스 이벤트에 대해서 처리 및 제어를 구현하는 콜백함수이다. 

EVENT_FLAG_LBUTTON 1 왼쪽 버튼 누름
EVENT_FLAG_RBUTTON 오른쪽 버튼 누름
EVENT_FLAG_MBUTTOM 가운데 버튼 누름
EVENT_FLAG_CTRLKEY 컨트롤키 누름
EVENT_FLAG_SHIFTKEY shift 키 누름
EVENT_FLAG_ALTKEY Alt 키누름 

```
## Trackbar 이벤트  제어

일정한 범위 내의 값을 선택하고자 하는 슬라이더바를 말한다. 

```c++

createTrackbar("트랙바 이름","윈도우 이름",int *vlaue,int 최고값, onchange,void* userdata);
//트랙바를 윈도우에 생성시켜주고 사용자가 정의한 이벤트 콜백을 시스템에 등록해준다.
 void onchage(pos,void* userdata);
 getTrackbarPos("트랙바 이름","윈도우 이름");
 setTrackbarPos("트랙바 이름","윈도우 이름", 값);


```

## 그리기 함수

```c++
line(Mat(),Point(x,y),Point(x,y),color,굵기, LINE_AA,shift(입력 좌표에서 shift할 값) );
//직선

rectangle(Mat,Rect(),Scalar(),굵기, LINE_AA, int shift);
//사각형
rectangle(Mat,Point(),Point(),Scalar(),굵기, LINE_AA, int shift);

putText(Mat,"뿌릴 문자",Point(), FONT_HERSHEY_PLIAIN,color,굵기, line_AA,BottomLeftOrigin=false);

circle(Mat, Point center,radius, color, thickniss, linetype, shift);

ellipse(Mat(),Point center, Size(width,height),기준angle, startangle, endEngle, Scalar(color), 굵기, 선종류, shift);

```

## 이미지 파일 처리 

```c++

imread("파일이름.jpg",CV_GRAYSCACLE);

vector<int> params_jpg;
params_jpg.push_back(IMWRITE_JPEG_QUAITY);//0~100
params_jpg.push_back(50);
params_jpg.push_back(IMWRITE_PNG_COMPRESSON);//0~9
params_png.push_back(9);

imwrite("파일이름", Mat(),params_jpg);

```
## 비디오 입출력
### 비디오 입력
``` c++
VideoCapture capture();
VideoCapture capture("파일 이름");
VideoCapture capture(0);//0번 카메라 옵흔

capture.open("파일 이름");
capture.open(0);//0번 카메라 옵흔
capture.isOpened(); //해당객체가 파일이나 디바이스를 열었는지 확인 
capture.release();// 해당객체가 연 디바이스나 파일 놔주기
capture.get(CAP_FRAME_WIDTH);// 플래그에 따라 캡춰객체의 정보 가져오기 
capture.set(CAP_FRAME_WIDTH,800);
// 플래그에 따라 정보설정
capture.grab();//장치나 파일로부터 다음 프레임을 잡아옴
capture.retrieve();// grab으로 가져온 프레임을 디코드해서 이미지 행렬로 전달
capture.read(),>>,;값을 잡아와서 image 로 디코드 해서 전달
```
set(),get() 의 속성상수는 책180쪽 참고
### 비디오 출력
```c++
    Writer wr();
    wr.open("파일이름",fourcc,프레임수, 프레임사이즈, isColor=true);
    Writer wr("파일이름",fourcc,프레임수, 프레임사이즈, isColor=true);//fourcc 코덱 4개 문자
    Writer wr();
    wr.isOpened();
    wr.write(),<< image 행렬을 프레임으로 저장한다. 
    
```
fourcc 의 설정은 181쪽 참고

## 파일-FileStorage 클래스

```c++
FileStorage file();// XML YML 등의 파일 저장
FileStorage file("파일이름",READ,저장데이터 인코딩 방식);//
file.isOpened();//클래스에 지정된 파일이 열려있는지 확인하여 열려있으면 true;
file.release();//파일을 닫고, 모든 메모리 버퍼를 해제한다.
file.writeRaw();//다중의 숫자들을 저장한다.

```
### FileNode();
XML,YML의 매핑{}과, 시퀀스[]에 접근하기 위해서 사용하는 것.
```c++
FileNode fNode();
FileNode fNode(FileStorage(),FileNode());
FileNode fNode(FileNode());
fNode.name();//노드 이름 반환
fNode.size();//해당 노드의 크기 반환
fNode.type();// 시퀀스인지, 매핑인지, FileNode::XX 형식으로 반환
fNode.empty();//노드가 비었는지 확인
fNode.isNamed();//노드의 이름이 있는지 확인
fNode.isNone();//노드가 None 객체인지 확인
fNode.isInt();
fNode.isBool();
fNode.isString();
fNode.isMap();
fNode.isSeq();

```
