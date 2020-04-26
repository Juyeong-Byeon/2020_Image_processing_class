# 기본 배열 처리함수 

## 뒤집기 및 반복
```c++
flip(Mat input,Mat output,0 );
//뒤집기, 0: X축을 기준을 1: Y 축을 기준으로 -1: X축 Y축 각각 한번 

repeat(Mat input,X반복,Y반복, Mat output);

transpose(Mat input, Mat output);
입력행렬의 열과 행을 반대로 만들어준다.

```

### 채널 처리 함수 
```c++
 merge(Mat input,합성될 배열 개수 ,Mat output);//각 체널을 하나로 합쳐준다.
 split(Mat arr,outPut Mat);//각 체널별로 나누어준다.
 mixChannels(Mat(),Mat(dst),fromTo(순서쌍 배열임),몇개의 짝);//명시된 체널의 순서쌍에 의해 입력배열들로부터 출력배열들을 복사한다.
```
## 사칙연산 
```c++
add(input Mat1,input Mat2,output Mat,input Mat=mask,dtype);
subtract(Input Mat, InputMat, outMat, mask, 데이터타입);
multiply(Input Mat1,Input Mat 2, output Mat,가중치, 데이터타입 );
divide(Input Mat1,Input Mat 2, output Mat,가중치, 데이터타입 );
divide( 가중치,Input Mat1,Input Mat 2, output Mat, 데이터타입 );
addWeighted(Mat()in,알파,Matin2,베타,감마,Mat out, 데이터 타입)//두 행렬의 가중치가 곱해지고 더해져서 더해진다.
dst=src(i)*알파+src2*베타+감마);

```
## 지수 로그 루트 관련 함수

```c++
exp(inMat,outMat);// 오일러상수 e의 inMat 제곱을 계산
log(inputMat, outPutMat);//오일러 상수를 밑으로 하는 로그계산
sqrt(inputMat, outPutMat);//제곱근을 계산
pow(inputMat, power,outPutMat);// power 승수 계산
magnitude(inputMat x,InputMat y,outputMat);// x y 벡터들의 크기를 계산
phase(inputMat x,inputMat y, outputArray magnitude, 각도, angleInDegrees);
//2차원 벡터의 회전각도를 계산한다.
cartToPolar();
//2차원 벡터들의 크기와 각도를 계산한다.
polarToCart();
// 각도 크기로부터 2차원 벡터들의 좌표 계산
```

## 논리 연산함수

```c++
bitwise_and(Input Mat1,Input Mat2,outMat,마스크 );
bitwise_or(Input Mat1,Input Mat2,outMat,마스크 );
bitwise_xor(Input Mat1,Input Mat2,outMat,마스크 );
bitwise_not(Input Mat1,output,마스크 );
```

## 절댓값 
```c++
abs(Mat m);
absdiff(inM,inM,outM);// 두 행렬을 빼고, 절대값을 반환;
convertScaleAbs(inM,outM,알파, 베타);// 각 원소에 알파만큼 곱하고 베타만큼 더한 후에 절대값 반환
```

## 최솟값과 최댓값

```c++
min(Mat input,Mat input,output dst);// input 되는 두 arr을 비교해서 최소 값을 dst mat 에 저장한다,//arr도 가능
Mat a=min(Mat,double); 스칼라를 적용해서 return 받을 수도 있다.
Mat a=min(double,Mat a);

max(input Mat1,input Mat2,목적Mat);// 두 배열을 비교해서 더 작은값을 목적 배열에 저장
Mat a=max(Mat,double);
Mat a=max(double,Mat a);

minMatIdx(input Mat, double *minVal, double *maxVal, int* minIdxArr, int* maxIdxArr,연산마스크);// 전체 배열에서 최소값과 최대값, 그리고 각각의 index를 포인터 형태로 반환한다.
minMaxLoc(input Mat, double* minVal,double* maxVal,Point* minLoc,Point* maxLoc);// 최댓값과 최소값을 포인터로 반환, 그리고 각각의 인덱스를 Point class pointer로 반환한다.
```

## 통계 관련 함수

```c++
sum(input Mat);//각각의 체널별로 원소들의 합을 스칼라로 return 해준다.
mean(inputMat1,inputMat2,inputMat(Mask));//마스크가 0이 아닌 부분들에서 각 체널별로 평균을 구해서 스칼라로 return
meanStdDev(inputMat,outPutMat_mean,inputMat_Stddev,inputMask);//배열 원소들의 평균과 표준 편차를 계산한다.
int count=countNonZero(inputMat);//0이 아닌 배열 원소를 개수 N으로 반환한다.
reduce(inputMat,outputMat,0||1,감축연산종류{sum,max,min,avg},감소된벡터의 자료형);//행렬을 열방향 혹은 행 방향으로 모두 옵션상수에 따른 연산을 한 후에 벡터로 감축
sort(inputMat,outputMAt,int flgs{오름차순, 내림차순, 열,행 등등의 설정});//각 행별로, 열별로 정렬한다.
sortIndex(inputMat,outputMAt,int flgs{오름차순, 내림차순, 열,행 등등의 설정});//각 행별로 열별로 정렬하되, 반환은 정렬된 index로 반환한다.
```
 

 ## 행렬 연산함수 
 ```c++
 void gemm(Input Mat1, Input Mat2, double alpha, Input Mat3, double beta, int flags);// 행렬의 곱 flag 어느 행렬을 전치할지
//수식 dst=alpha*Mat1.t()*Mat2+beta*Mat3;
void transform(input Mat, outPut Mat, inputArr);//inputARR 은 변환에 사용되는 2X2 혹은 4X4 변환행렬 입력배열의 모든 원소에 행렬 변환을 수행
void perspectiveTransform();
```

## 역행렬과 연립방정식의 해

```c++
double invert(input Mat, output Mat, flag);//역행렬을 계산해주는 함수
double solve(Input Mat1, input Mat2, output Mat, flags);//바로 연립방정식을 풀어주는 함수

```