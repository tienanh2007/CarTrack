#ifndef CAR_H
#define CAR_H
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <cmath>
#include <algorithm>
using namespace std;
class Car
{
protected:
    static int nextID;
public:
    int width;
    int height;
    int cx ;
    int cy;
    int id;
    int prex;
    int prey;
    int stat;
    double dx;
    double dy;
    int fx;
    int fy;
    int xs[5] = {0,0,0,0,0};
    int ys[5] = {0,0,0,0,0};
    int exe;
    Car(int x,int y,int wid,int hei);
    Car(int x,int y,int wid,int hei,int i);
    void print();
};

#endif // CAR_H
