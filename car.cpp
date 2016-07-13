#include "car.h"
#include <iostream>
#include <stdio.h>

using namespace std;
int Car::nextID = 100;
Car::Car(int x, int y,int wid,int hei)
{
    width = wid;
    height = hei;
    id = ++nextID;
    cx = x;
    cy = y;
    dx = 1;
    dy = 4;
    prex = 0;
    prey = 0;
    fx = x;
    fy = y;
    exe = 0;
}
Car::Car(int x, int y,int wid,int hei,int i)
{
    width = wid;
    height = hei;
    id = i;
    cx = x;
    cy = y;
    dx = 1;
    dy = 4;
    prex = 0;
    prey = 0;
    fx = x;
    fy = y;
    exe = 0;
}
void Car::print()
{
    cout << "car id: "<< id << ", car x: " << cx << ", car y: " << cy << ", car prex: "
         << prex << ", car prey:" << prey << ", car stat:" << stat << ", dx: " << dx
         << ", dy: "<< dy << ", fx:" << fx << ", fy:" << fy << ", exe: " << exe <<"\n" << "x1: " << xs[0]
         << ", x2: " << xs[1] << ", x3: " << xs[2] << ", x4: " << xs[3] << ", x5: " << xs[4]
         << "\n" << ", y1: " << ys[0] << ", y2: " << ys[1] << ", y3: " << ys[2] << ", y4: " << ys[3] << ", y5: " << ys[4] << "\n";
}
