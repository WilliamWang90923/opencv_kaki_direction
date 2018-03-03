#include"foo.h"

Mat rotate_pic(Mat src,Point2f center,int angle,int SCALE=1)
{  
	Mat dst;
	//int angle = angle; //rotation degree
	int length;//output size equals input diagnal times SCALE
	 
	if(SCALE <= 1)  
		length = sqrt((src.cols*src.cols + src.rows*src.rows)*1.0);  
	else  
		length = sqrt((src.cols*src.cols + src.rows*src.rows)*1.0) * SCALE;  
	
	Mat tempImg(length,length,CV_8UC3,Scalar(0,0,0));//temp picture 
	int ROI_x = length/2 - src.cols/2;//ROI矩形左上角的x坐标   
	int ROI_y = length/2 - src.rows/2;//ROI矩形左上角的y坐标   
	Rect ROIRect(ROI_x,ROI_y,src.cols,src.rows);
	Mat tempImgROI2(tempImg,ROIRect);//tempImg middle area   
	src.copyTo(tempImgROI2);

	//Point2f center=center;  
	Mat M = getRotationMatrix2D(center,angle,SCALE);//Matrix for rotation 

	cout<<"Rotation Matrix："<<endl;  
	cout<<M.at<double>(0,0)<<","<<M.at<double>(0,1)<<","<<M.at<double>(0,2)<<","<<endl;  
	cout<<M.at<double>(1,0)<<","<<M.at<double>(1,1)<<","<<M.at<double>(1,2)<<","<<endl;  

	warpAffine(tempImg,dst,M,Size(length,length)); 

	return dst;
	//display 	
}

vector<Point2f> GetCenterPoint(Mat src)
{
	int i,j;
	double x0=0,y0=0,sum2=0,sum3=0;
	vector<vector<Point> > contours;      
    vector<Vec4i> hierarchy;  
	findContours(src, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//compute the moments of contours
	vector<Moments> mu(contours.size());       
    for( int i = 0; i < contours.size(); i++ )     
    {   
        mu[i] = moments( contours[i], false );   
    }     
    //compute mass point of contours     
    vector<Point2f> mc( contours.size() );      
    for( int i = 0; i < contours.size(); i++ )     
    {   
        mc[i] = Point2d( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );   
    }     
	return mc;
}



/*void DefectsRepair(IplImage *pBinary, float fDisThre)  
{  
    int x, y, i, j;  
    int x0, x1, x2;  
    int n, dx, dy, nSize;  
    unsigned char *pLine[3];  
    float fDistance;  
    Point2f ptPoint;  
    // 轮廓端点坐标  
    Point2f vecPoint;  
    // 执行条件  
    if (pBinary)  
    {  
        for (y = 1; y < pBinary->height - 1; y++)  
        {  
            pLine[0] = (unsigned char*) (pBinary->imageData + pBinary->widthStep * (y - 1));  
            pLine[1] = (unsigned char*) (pBinary->imageData + pBinary->widthStep * y);  
            pLine[2] = (unsigned char*) (pBinary->imageData + pBinary->widthStep * (y + 1));  
            for (x = 1; x < pBinary->width - 1; x++)  
            {  
                if (pLine[1][x] > 0)  
                {  
                    n = 0;  
                    x0 = x - 1;  
                    x1 = x;  
                    x2 = x + 1;  
                    if (pLine[0][x0] != pLine[0][x1]) n++;  
                    if (pLine[0][x1] != pLine[0][x2]) n++;  
                    if (pLine[0][x2] != pLine[1][x2]) n++;  
                    if (pLine[1][x2] != pLine[2][x2]) n++;  
                    if (pLine[2][x2] != pLine[2][x1]) n++;  
                    if (pLine[2][x1] != pLine[2][x0]) n++;  
                    if (pLine[2][x0] != pLine[1][x0]) n++;  
                    if (pLine[1][x0] != pLine[0][x0]) n++;  
                    // 孤立点  
                    if (n == 0)  
                    {  
                        if (pLine[1][x] != pLine[0][x0])  
                        {  
                            ptPoint.x = x;  
                            ptPoint.y = y;  
                            vecPoint.push_back(ptPoint);  
                        }  
                    }  
                    // 轮廓端点  
                    else if (n == 2)  
                    {  
                        ptPoint.x = x;  
                        ptPoint.y = y;  
                        vecPoint.push_back(ptPoint);  
                    }  
                }  
            }  
        }  
        // 缺陷修补  
        nSize = (int) vecPoint.size();  
        for (i = 0; i < nSize - 1; i++)  
        {  
            for (j = i + 1; j < nSize; j++)  
            {  
                dx = vecPoint[i].x - vecPoint[j].x;  
                dy = vecPoint[i].y - vecPoint[j].y;  
                fDistance = (float) (dx * dx + dy * dy);  
                if (fDistance <= fDisThre * fDisThre)  
                {  
                    cvLine(pBinary, vecPoint[i], vecPoint[j], CV_RGB(255, 255, 255), 1, 8, 0);  
                }  
            }  
        }  
    }  
}  */

int getDistance (const Point pointA,const Point pointB )
{
	int distance;    
	distance = powf((pointA.x - pointB.x),2) + powf((pointA.y - pointB.y),2);    
	distance = sqrtf(distance);    

	return distance;    
}

int num_gap(int x, int y,int N)
{
	int gap;
	gap = (abs(x-y)<=N/2) ? abs(x-y) : N-abs(x-y);
	return gap;
}