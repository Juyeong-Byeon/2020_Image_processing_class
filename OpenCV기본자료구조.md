# openCV 기본 자료구조 

openCV 는 자체적으로 자체적으로 다양한 자료구조를 제공한다. 

## 기본 템플릿 클래스
### <자료형> 등으로 다른 자료형을 이용하는 클래스를 생성할 수 있는 클래스

## Point_< type t >

Point_<> 클래스는 가로와 세로의 위치를 2차원으로 나타내기위한 클래스이다. 맴버 변수로는 x,y 가 있다.  

``` c++
 Point_<type>();
 Point_<type t>(t x,t y);// x y는 타입
 Point_<type t>(Point p); //다른 포인트로도 초기화 가능
 Point_<type t>(Size()); //Size 객체로도 초기화 가능 
 Point_<type t>(Vec());

 Point_<int> point1(10,12);
 Point_<int> point2(10,20);
 point1.x//10
 point1.y//12
 point1.dot(point2)// 파라메터로 들어온 x,y 나 Point() 객체와 내적 계산
 point2.cross(point1); // 파라메터로 들어온 좌표와 외적 계산

 point2.inside(Rect());// Rect 안에 point가 포함되어있는지 bool 값 반환

 Point point3;//Point_<int> 
 Point2i;//Point_<int> 
 
 point3=point2+point1;// 각 원소끼리 더한다. 
 //point3=point2*point1;//Error!; 
 point3=point1-point;

```
## Size_< type t > size(width,height);
Size 클래스 또한 타입을 i, f, d 등의 간결한 문자로 표현 가능하다.
```c++
Size_<type t>();//생성자
Size_<type t>(width,height);//넓이 길이
Size_<type t>(Size())// 사이즈를 할당해서도 생성 가능
Size_<type t>(Point())// Point class를 할당해서도 생성 가능

Size_<int> size(10,20);
size.area();//30 반환
```

## Rect_<type t> Rect(x,y,width,height);
사각형 정보를 나타내기 위한 템플릿 클래스이다. 

```c++
Rect_<type t>(x,y,width,height);
Rect_<type t>(Point(x,y),Size(width,height));
Rect_<type t>(Rect());
Rect_<type t>(Point(x,y), Point(x,y))// point1을 left Top으로, point2를 right bottom 으로 두개를 잇는 사각형 정의

//내부 메서드
rect.tl()// top left Point() 반환
rect.br();// bottom right Point() 반환
rect.size();//사각형의 크기를 나타내는 Size 인스턴스 반환
rect.area();//사각형의 크기를 나타내는 타입에 맞는 상수 반환
rect.contains(Point(x,y));//point 가 사각형 내부에 포함되어있는지 확인
//내부 프로퍼티
rect.x;
rect.y;
rect.width;
rect.height;

//가능한 연산

rect-=Point(x,y);//사각형의 top-left Point를 Point(x,y)-방향으로 이동.
rect+=Point(x,y);//사각형의 top-left Point를 Point(x,y)+방향으로 이동.
rect+=Size(width,height);//사각형의 넓이와 높이를 width height만큼 변경
rect-=Size(width,height);//사각형의 넓이와 높이를 width height만큼 변경
rect =rect1&rect2;//rect1과 rect2에 모두포함되는 영역(교차영역)의 사각형 반환 
rect =rect1&rect2;//rect1과 rect2를 모두포함하는 영역(모두포함)의 사각형 반환 
```

## Vec<type t, 원소 개수>
원소 개수가 작은 숫자벡터를 위한 템플릿 클래스이다. 

```c++
Vec<int,10> vec(1,2,3,4,5,6,7,8,9,10);
Vec3i vec1(1,2,3)==Vec<int,3>vec2 (1,2,3);
 std::cout<<vec[1]<<std::endl;// 배열첨자 사용가능
Vec<int,3> vec3(1,2,3);
Vec<int,3> vec4(1,2,3);
//내부 메서드
vec3.mul(vec4);//Vec<int,3>(1,4,9); 각각원소간의 곱을 계산한 Vec을 반환한다.

vec1+vec2;//가능 각 요소간 덧셈
vec1*vec//에러!!!
vec1*10//가능

```

## Scalar_< type t >(B,G,R,A);

Scalar 클래스는 Vec<type t,4> 에서 파생된 클래스로 색을 정의하기 위한 클래스이다. 
```c++
Scalar_<int> sc1(10,20,30,10);
Scalar_<int> sc2(10,20,30,10);

cout<<sc1[0]<<endl;// 배열첨자 이용가능

cout<<sc1+sc2<<endl;//각각원소끼리 더하기 가능
cout<<sc1-sc2<<endl;//각각원소끼리 빼기 가능

```
## RotatedRect(Point2f(x,y),Size2f(width,height),float angle);

회전된 사각형을 나타내기 위한 클래스이다. 
```c++
RotatedRect rRect(Point2f(x,y),Size2f(width,height),float angle);

//멤버 메서드
rRect.boundingRect();//회전된 rect를 포함하는 Rect() 반환
rRect.points(Point points[]);// 인수로 입력된 Point 배열에 4개의 꼭지점 좌표 반환

//멤버변수
rRect.center;
rRect.size;
rRect.angle;
```
## Mat(rows,cols,type,Scalar(B,G,R,A));

```c++
 Mat m(rows,cols,type,Scalar(B,G,R,A));
 Mat m(rows,cols,type);
 Mat m(Size(x,y),type);// 

 Mat(Mat());
 Mat(Mat(),Rect());//Rect가 Roi가 된다. 
 Mat(Mat(),ranges{Range(x1,x2),Range(y1,y2)});//Range는 x1번째 열부터 x2번째 열 전까지, y1행부터 y2행 전까지 포함해 Roi를 만들어 준다. 
CV_8U//uchar
CV_8S//char
CV_16U//ushort
CV_16U//short
CV_32S//int
CV_32F//float
CV_64F// double
 
```

## Mat 행렬 초기화 함수
Mat 클래스 내부메서드 중에 단위행렬과 같은 특수한 행렬을 생성해주는 함수들이 있다.

```c++
Mat m1=Mat::ones(3,5,CV_8UC1);// 1로 초기화된 행렬 반환
Mat m2=Mat::zeros(3,5,CV_8UC1);//0으로 초기화된 행렬 반환
Mat m3=Mat::eye(3,5,CV_8UC1);//단위행렬 반환

```

## Matx<type t,rows,cols>();

```c++
Matx<int,3,3> mat(1,2,3,4,5,6,7,8,9);
mat[1]//배열첨자 이용가능
mat<<1,2,3,4;//이렇게 초기화도 가능하다.
```

## Mat class 맴버변수 및 맴버 메서드

```c++

Mat mat(3,4,CV_8U);
mat.dims;//차원 수 2차원
mat.rows;//행의개수 3
mat.cols;//열의 개수 4
mat.data;행렬 원소 데이터에대한 포인터
mat.step;// 한 행이 차지하는 바이트 수 cols*type*체널 바이트 
//멤버 메서드
mat.channels();//몇개의 체널이 있는지
mat.depth();// 행렬의 자료형 반환
mat.elemSize();// 행렬의 한 원소에 대한 바이트(모든 체널 포함)
mat.elemSize1();// 한 원소의 체널에 대한 바이트크기
mat.empty();// 행렬 원소가 비어있는지 확인
mat.isSubmatrix();// 참조행렬인지 확인
mat.size();// 행렬의 크기를Size()로 반환
mat.step1();//한 체널의 
mat.total();// 행렬 원소 전체 개수 반환
mat.type();// 행렬의 자료형 반환

```
## Mat 클래스의 크기 및 형태 변경

## resize(Size(),Scalar());
```c++
Mat mat(2,4,CV_8U);
mat.resize(size);// 행의 개수를 기준으로 사이즈를 늘려준다. //기존 원소 개수와 달라짐
mat.resize(size,Scalar());//새로 추가된 원소를 Scalar로 초기화 
```

## reshape(int channel,int rows);
```c++
Mat mat(3,4,CV_8U);
mat.reshape(channel,rows);//체널 수와 row수를 가지고 조절, 전체 원소의 개수가 같아야 한다. 
```

## Mat 복사및 자료형 변환

```c++
mat.clone();//행렬데이터와 같은 값을 복사해서 새로운 행렬 반환
mat.copyTo(mat2);// 행렬데이터를 복사해서 입력된 행렬에 복사
mat.convertTo(mat2,CV_8U,alpha:원본에서 배율,delta:이동값)//
//기존행렬을 변형해서 입력 행렬에 복사해준다. 

```

## vector< type t >();

vector 템플릿 클래스는 c++의 STL에 속해있는 클래스로서 객체들을 
쉽게 각종 자료구조에 담아 사용할 수 있게 해준다.

```c++
vector v();
vector v(size);
vector v(vector());
vector v(size, 각원소에 할당되는 배열의 포인터);
vector v(시작주소,끝 주소);
v.reserve(size);//

// 메서드
v.erase(addr+index);
v.erase(start_addr,end_addr);

v.insert(insert_addr,value);

v.push_back(1);// 맨뒤에 추가
v.pop_back(); 맨 뒤ㅣ의 값 제거
v.empty(); 백터가 비어있는지 확인 

```

##  Range 클래스 
하나의 시퀀스에서 연속되는 서브 시퀀스를 지정할 때 사용하는 클래스 Range(Start,end); Start=< 선택< end 관계이다. 
``` c++
Range range(start,end);
range.size();// 시퀀스의 사이즈 반환
range.empty();// 시퀀스가 비었는지 반환
range.all();// 서브 시퀀스가 가득 차있는지 반환

```

## 행렬 헤더 함수

행렬 헤더란 행렬의 메모리를 공유하는 sub 행렬을 만드는 것을 말한다. 
```c++
    Mat mat1(3, 10, CV_8U,Scalar(100));
    cout<<mat1.row(1);//1번 index 를 공유하는 row 반환
    cout<<mat1.col(1);//1번 index 를 공유하는 col 반환

    mat1.rowRange(int startRow, int endRow);//여러행 행렬헤더 반환
    mat1.colRange(int startCol,int endCol);//여러열 행렬헤더
    mat1.locateROI(Size&,Point&);//부모행렬의 크기와 부모행렬에서의 헤더의 위치
    mat1.adjustROI(dtop,dbottom,dleft,dright);//위로 얼마나 밑으로 얼마나 양 옆으로 얼마나 늘리고 줄일지.
    mat.operator(Range,Range);

```
## 원소의 추가 및 삭제 

```c++

mat1.push_back(elem);// row에 추가
mat2.pop_back();//row 제거
mat3.release();// 행렬 메모리 해제 

```
## 행렬의 기초적인 연산 

```c++
mat.cross(Mat()); //두 행렬의외적
mat.dot(Mat());// 두 행렬의 내적
mat.inv();// 해당 행렬의 역행렬 반환
DECOMP_LU// 가우시안 소거법 역행렬
DECOMP_SVD// 특이치 분해
DECOMP_CHOLESKY// 숄레스키 
mat.mul(inputARR(), scale);// 두 행렬의 각 원소간 곱셈을 수행함.
mat.t(); 전치행렬 반환 
```



