#include "mainwindow.h"
#include <QApplication>
#include "car.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <ctime>

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <cmath>
#include <algorithm>
using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( Mat frame );
Car findClosest(Car obj,vector<Rect> objs);
vector<Car> filter(Car obj,vector<Car> objs);
vector<Rect> filterRect(vector<Rect> detected, vector<Car> cars);
vector<Car> updateFX(vector<Car> cars);
vector<Car> filterSame(vector<Car> cars);
bool compareCar(Car x, Car y);
/** Global variables */
String bike_cascade_name = "classifierFront/bike1.0.xml";
String car_cascade_name = "classifierFront/car1..xml";
CascadeClassifier bike_cascade;
CascadeClassifier car_cascade;
string window_name = "Capture - Vehicle detection";
RNG rng(12345);
int minFaceSize = 0;
int maxFaceSize = 0;
int frames = 0;
int carsNum = 0;
vector<Car> car;
/** @function main */
const string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}
int main( int argc, const char** argv )
{
    VideoCapture capture("video7.mkv");

    //-- 1. Load the cascades
    if( !bike_cascade.load( bike_cascade_name ) ){ printf("--(!)Error loading\n"); };
    if( !car_cascade.load( car_cascade_name ) ){ printf("--(!)Error loading\n"); };

    //-- 2. Read the video stream
    if( capture.isOpened() )
    {
        capture.set(CV_CAP_PROP_POS_MSEC, 100);
        while( true )
        {
            Mat frame;
            bool bSuccess = capture.read(frame);
            frames++;
            if(!bSuccess){
                printf("nope no frame here");
                break;
            }
            detectAndDisplay(frame);
            int c = waitKey(30);
            if( (char)c == 'c' ) { break; }
            if( (char)c == 's' )
            { for(int i=0;i<20;i++){
                    capture.grab();
                }
            }
        }
    }
    else{
        printf("error reading the video man");
    }

    //findClosest test
    //          Car c {10,10,5,5,1};
    //          Car a {5,5,5,5,2};
    //          vector<Rect> obj;
    //          Rect one {1,1,5,3};
    //          Rect two {4,4,5,4};
    //          Rect three {7,7,5,5};
    //          obj.push_back(one);
    //          obj.push_back(two);
    //          obj.push_back(three);
    //          Car b = findClosest(c,obj);
    //          cout << b.id << "," << b.cx << "," << cb.cy ;

    // filter test
    //    Car a {2,3,5,6,"1"};
    //    Car b {2,4,6,7,"2"};
    //    Car c {2,5,5,6,"1"};
    //    vector<Car> obj;
    //    obj.push_back(a);
    //    obj.push_back(b);
    //    cout << obj.size();
    //    vector<Car> objNew(filter(c,obj));
    //    for(size_t i=0;i<objNew.size();i++)
    //    {
    //        cout << objNew[i].cy;
    //    }

    // filterRect test
    //    Car a {2,3,5,6,1};
    //    Car b {2,4,6,7,2};
    //    Car c {2,5,5,6,1};
    //    vector<Car> obj;
    //    obj.push_back(a);
    //    obj.push_back(b);
    //    obj.push_back(b);
    //    Rect d {2,3,5,5};
    //    Rect e {2,4,5,7};
    //    Rect f {4,6,5,5};
    //    vector<Rect> rects;
    //    rects.push_back(d);
    //    rects.push_back(e);
    //    rects.push_back(f);
    //    vector<Rect> xe = filterRect(rects,obj);
    //    cout << xe.size();
    //    for(size_t i=0;i<xe.size();i++){
    //        cout << "x:" << xe[i].x << ", y:" << xe[i].y ;
    //    }

    // updateFX test
    //        Car a {2,3,5,6,1};
    //        Car b {2,4,6,7,2};
    //        Car c {2,5,5,6,1};
    //        vector<Car> obj;
    //        obj.push_back(a);
    //        obj.push_back(b);
    //        obj.push_back(b);
    //        for(size_t a=0;a<obj.size();a++){
    //            obj[a].print();
    //        }
    //        obj = updateFX(obj);
    //        for(size_t a=0;a<obj.size();a++){
    //            obj[a].print();
    //        }

    // sort test
    //    Car a {2,3,5,6,-5};
    //    Car b {2,4,6,7,2};
    //    Car c {2,5,5,6,-2};
    //    vector<Car> obj;
    //    obj.push_back(a);
    //    obj.push_back(b);
    //    obj.push_back(c);
    //    sort(obj.begin(),obj.end(),compareCar);
    //    for(size_t a=0;a<obj.size();a++){
    //        obj[a].print();
    //    }

    // filterSame test
    //    Car a {12,30,5,6,1};
    //    Car b {2,4,6,7,5};
    //    Car c {4,5,5,6,1};
    //    Car d {5,5,5,6,5};
    //    Car e {6,5,5,6,1};
    //    Car f {8,5,5,6,5};
    //    vector<Car> obj;
    //    obj.push_back(a);
    //    obj.push_back(b);
    //    obj.push_back(c);
    //    obj.push_back(d);
    //    obj.push_back(e);
    //    obj.push_back(f);
    //    obj = filterSame(obj);
    //    for(size_t a=0;a<obj.size();a++){
    //        obj[a].print();
    //    }
    return 0;
}
vector<Car> filterSame(vector<Car> obj)
{
    sort(obj.begin(),obj.end(),compareCar);
    for(size_t i=0;i<obj.size();i++){
        for(size_t a=i+1;a<obj.size();a++){
            if(abs(obj[i].cx-obj[a].cx)<5){
                obj.erase(obj.begin()+a);
                a--;
            }
        }
    }
    return obj;
}

bool compareCar(Car x, Car y)
{
    return x.id > y.id;
}
vector<Car> updateDXY(vector<Car> cars)
{
    for(size_t i=0;i<cars.size();i++){
        if(cars[i].xs[0]!=0&&cars[i].xs[4]!=0){
            cars[i].dx = abs(cars[i].xs[(cars[i].exe-1)%5] - cars[i].xs[(cars[i].exe-5)%5])/5.0;
            cars[i].dy = abs(cars[i].ys[(cars[i].exe-1)%5] - cars[i].ys[(cars[i].exe-5)%5])/5.0;
        }
    }
    return cars;
}
vector<Car> updateFX(vector<Car> cars)
{
    for(size_t i=0;i<cars.size();i++){
        cars[i].fx = cars[i].fx + cars[i].dx;
        cars[i].fy = cars[i].fy + cars[i].dy;
    }
    return cars;
}
vector<Rect> filterRectOne(vector<Rect> detected, Car car)
{
    vector<Rect> filtered;
    for(size_t i=0;i<detected.size();i++){
        if(car.cx!=detected[i].x&&car.cy!=detected[i].y)
            filtered.push_back(detected[i]);
    }\
    return filtered;
}
// too close we don't want these car
vector<Rect> filterRange(vector<Rect> cars){
    vector<Rect> filtered;
    for(size_t i=0;i<cars.size();i++){
        if(cars[i].x<510)
            filtered.push_back(cars[i]);
    }
    return filtered;
}

// remove matched car after the match process
vector<Rect> filterRect(vector<Rect> detected, vector<Car> cars)
{
    vector<Rect> filtered;
    for(size_t i=0;i<detected.size();i++){
        bool dup = false;
        for(size_t a=0;a<cars.size();a++){
            if(detected[i].x == cars[a].cx && detected[i].y == cars[a].cy)
                dup = true;
        }
        if(!dup)
            filtered.push_back(detected[i]);
    }\
    return filtered;
}
// find the closest
Car findClosest(Car obj,vector<Rect> objs){
    int minx = 0,miny = 0,minrad = 10000,minwid=0,minhei=0;
    // just find the closest using the previous matches
    if(obj.stat == 1){
        for(size_t i=0;i<objs.size();i++)
        {
            if(objs[i].x-obj.cx >= -15 || objs[i].y-obj.cy >= -15){
                if((pow(objs[i].x-obj.cx,2)+pow(objs[i].y-obj.cy,2))<minrad){
                    minx = objs[i].x;
                    miny = objs[i].y;
                    minrad = pow(objs[i].x-obj.cx,2)+pow(objs[i].y-obj.cy,2);
                    minwid = objs[i].width;
                    minhei = objs[i].height;
                }
            }
        }
    }
    // find the closest using prediction
    else{
        for(size_t i=0;i<objs.size();i++)
        {
            if(objs[i].x-obj.cx >= -15 || objs[i].y-obj.cy >= -15){
                if((pow(objs[i].x-obj.fx,2)+pow(objs[i].y-obj.fy,2))<minrad){
                    minx = objs[i].x;
                    miny = objs[i].y;
                    minrad = pow(objs[i].x-obj.fx,2)+pow(objs[i].y-obj.fy,2);
                    minwid = objs[i].width;
                    minhei = objs[i].height;
                }
            }
        }
    }
    Car c {minx,miny,minwid,minhei,obj.id};
    c.prex = obj.cx;
    c.prey = obj.cy;
    if(c.cy > 300 && c.prey < 300)
        carsNum++;
    if(objs.size()==0 || minrad > 2000){
        c.id = -1;
    }
    else if(obj.stat == 1){
        copy(begin(obj.xs),end(obj.xs),begin(c.xs));
        copy(begin(obj.ys),end(obj.ys),begin(c.ys));
        c.xs[obj.exe%5] = obj.cx;
        c.ys[obj.exe%5] = obj.cy;
        c.exe = obj.exe+1;
    }
    return c;
}
// del cars that have been match by findClosest
vector<Car> filter(Car obj,vector<Car> objs)
{
    vector<Car> objNew;
    for(size_t i = 0; i < objs.size(); i++)
    {
        if(obj.id!=objs[i].id){
            objNew.push_back(objs[i]);
        }
    }
    objNew.push_back(obj);
    return objNew;
}
// del cars that surpass the error limit (making too many errors)
vector<Car> getrid(vector<Car> objs)
{
    vector<Car> objNew;
    for(size_t i = 0; i < objs.size(); i++)
    {
        if(objs[i].stat<40){
            objNew.push_back(objs[i]);
        }
    }
    return objNew;
}

void detectAndDisplay( Mat frame )
{
    vector<Rect> bikes;
    vector<Rect> cars;
    Mat frame_gray;
    resize(frame,frame,Size(800,600));
    cvtColor( frame, frame_gray, CV_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    int height = frame.rows;
    if(minFaceSize == 0){
        if(round(height * 0.2f) > 0)
            minFaceSize = round(height * 0.1f);
    }
    maxFaceSize = 0.28f * height;
    bike_cascade.detectMultiScale( frame_gray, bikes, 1.2, 20, 0|CV_HAAR_SCALE_IMAGE, Size(minFaceSize,minFaceSize),Size(maxFaceSize,maxFaceSize) );
    car_cascade.detectMultiScale( frame_gray, cars, 1.2, 15, 0|CV_HAAR_SCALE_IMAGE, Size(minFaceSize,minFaceSize),Size(maxFaceSize,maxFaceSize) );
    for( size_t i = 0; i < bikes.size(); i++ )
    {
        printf("x:%d,y:%d, bike %d\n",bikes[i].x,bikes[i].y,i);
    }
    if(car.size() == 0){
        for( size_t i = 0; i < bikes.size(); i++ )
        {
            Car c {bikes[i].x,bikes[i].y,bikes[i].width,bikes[i].height};
            car.push_back(c);
        }
    }
    else
    {
        for(size_t i = 0; i < car.size(); i++){


            Car b = findClosest(car[i],bikes);
            if(b.id != -1){
                car = filter(b,car);
                i--;
                bikes = filterRectOne(bikes,b);
            }
            else{
                car[i].stat++;
                if(car[i].stat > 1)
                    car[i].exe = 0;
            }
        }
        car = getrid(car);
        car = updateDXY(car);
        car = updateFX(car);
        car = filterSame(car);
        bikes = filterRect(bikes,car);
        for( size_t i = 0; i < bikes.size(); i++ )
        {
            Car c {bikes[i].x,bikes[i].y,bikes[i].width,bikes[i].height};
            car.push_back(c);
        }
    }
    for(size_t i = 0; i < car.size(); i++){
        car[i].print();
        printf("end\n");
    }
    for( size_t i = 0; i < car.size(); i++ )
    {
        Point center( car[i].cx + car[i].width*0.5, car[i].cy + car[i].height*0.5 );
        ellipse( frame, center, Size( car[i].width*0.5, car[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
        putText(frame, to_string(car[i].id),center,FONT_HERSHEY_COMPLEX,0.5,Scalar(255,0,255));
    }
    for( size_t i = 0; i < cars.size(); i++ )
    {
        Point center( cars[i].x + cars[i].width*0.5, cars[i].y + cars[i].height*0.5 );
        ellipse( frame, center, Size( cars[i].width*0.5, cars[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 0 ), 4, 8, 0 );
    }
    Point center( 100,100 );
    putText(frame,to_string(carsNum),center,FONT_HERSHEY_COMPLEX,1,Scalar(255,0,255));
    line(frame,Point(100,300),Point(700,300),Scalar(255,0,0),2,8);
    imshow( window_name, frame );
    printf("frame %d:\n",frames);
}

