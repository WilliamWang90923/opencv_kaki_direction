#include<iostream>   
#include<opencv2/imgproc/imgproc.hpp>   
#include<opencv2/highgui/highgui.hpp>   
#include <cmath>

using namespace cv; 
using namespace std; 

Mat rotate_pic(Mat src,Point2f center,int angle,int SCALE);
vector<Point2f> GetCenterPoint(Mat src);
int getDistance (Point pointA,Point pointB );
int num_gap(int x,int y,int N);
void Kmeans(vector<vector<int>>,int N);