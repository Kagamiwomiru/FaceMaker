#pragma once
#pragma warning(disable:4819)
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<time.h>

#define WIDTH 500
#define HEIGHT 500
#define NAME_SIZE 10
#define SYSCMD 100

using namespace cv;
using namespace std;


void cap_image(Mat &in, string path, int width, int height,int num_images,VideoCapture cap);
