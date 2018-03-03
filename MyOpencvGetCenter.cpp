#include<iostream>
using namespace std;

#include <cv.h>
#include <highgui.h>  
#include <opencv2/opencv.hpp>



CvPoint center,stem;
int Min=0;
int count_p=0;
CvPoint GetCenterPoint(IplImage *src);
CvPoint GetMassPoint(IplImage *src);
CvPoint GetStemPoint(IplImage *src,CvPoint center2);


void main()
{
	IplImage *src,*src_draw;
	src=cvLoadImage("canny2.jpg",-1);
	src_draw=cvLoadImage("kaki2.jpg");

	CvPoint CenterPoint,StemPoint;
	CenterPoint=GetCenterPoint(src);
	StemPoint=GetStemPoint(src,CenterPoint);
	cout<<"CenterPoint.x: " <<CenterPoint.x<<"\t"<<"CenterPoint.y:"<<CenterPoint.y<<endl;
	cout<<"Stempoint.x: " <<StemPoint.x<<"\t"<<"Stempoint.y:"<<StemPoint.y<<endl;
	CvPoint pt2,pt3,pt4,pt5,stm2,stm3,stm4,stm5;
	pt2.x=CenterPoint.x-20;pt2.y=CenterPoint.y;
	pt3.x=CenterPoint.x+20;pt3.y=CenterPoint.y;
	pt4.x=CenterPoint.x;pt4.y=CenterPoint.y+20;
	pt5.x=CenterPoint.x;pt5.y=CenterPoint.y-20;
	stm2.x=stem.x-20;stm2.y=stem.y;
	stm3.x=stem.x+20;stm3.y=stem.y;
	stm4.x=stem.x;stm4.y=stem.y+20;
	stm5.x=stem.x;stm5.y=stem.y-20;
	cvLine(src_draw,pt2,pt3,CV_RGB(255,0,0),2,8,0);//画红十字标记位置
	cvLine(src_draw,pt4,pt5,CV_RGB(255,0,0),2,8,0);
	cvLine(src_draw,stm2,stm3,CV_RGB(255,0,0),2,8,0);//画红十字标记位置
	cvLine(src_draw,stm4,stm5,CV_RGB(255,0,0),2,8,0);


	cvNamedWindow("Center",CV_WINDOW_AUTOSIZE);
	cvShowImage("Center",src);
	cvNamedWindow("Stem",CV_WINDOW_AUTOSIZE);
	cvShowImage("Stem",src_draw);
	
	cvWaitKey(-1);
	cvReleaseImage(&src);
	cvReleaseImage(&src_draw);


}
CvPoint GetCenterPoint(IplImage *src)
{
	int i,j;
	double x0=0,y0=0,sum2=0,sum3=0;
	CvSeqReader reader;
	CvPoint* edge;
	CvScalar pixel;
	for (i=0;i<src->width;i++)
		for(j=0;j<src->height;j++)
		{
			pixel=cvGet2D(src,j,i);
			if(pixel.val[0]==1)
			{
				x0=x0+i;
				y0=y0+j;
				sum2=sum2+1;
			}
		}
	center.x=x0/sum2;
	center.y=y0/sum2;
	
	return center;
}

CvPoint GetMassPoint(IplImage *src)
{
	double m00, m10, m01;
    CvMoments moment;
    cvMoments( src, &moment, 1);
    m00 = cvGetSpatialMoment( &moment, 0, 0 ); 
    m10 = cvGetSpatialMoment( &moment, 1, 0 );
    m01 = cvGetSpatialMoment( &moment, 0, 1 );
    center.x = (int) (m10/m00);
    center.y = (int) (m01/m00);

    return center;
}

CvPoint GetStemPoint(IplImage *src,CvPoint center2)
{
	CvSeqReader reader;
	CvPoint* edge;
	int i,j;
	int x0=0,y0=0,sum2=0;

	CvScalar pixel;
	for (i=0;i<src->width;i++)
		for(j=0;j<src->height;j++)
		{
			pixel=cvGet2D(src,j,i);
			if(pixel.val[0]==1)
			{
				x0=x0+i;
				y0=y0+j;
				sum2=sum2+1;
			}
		}
	edge=(CvPoint*)malloc(sum2*sizeof(CvPoint));
	CvPoint pt1;

	for (i=0;i<src->width;i++)
		for(j=0;j<src->height;j++)
		{
			pixel=cvGet2D(src,j,i);
			if(pixel.val[0]==1)
			{
				edge[count_p].x=i;
				edge[count_p].y=j;
				count_p++;
			}
		}

		double* p;
		double* r;
		p=(double*)malloc(count_p*sizeof(double));
		r=(double*)malloc(count_p/2*sizeof(double));

	for(i=0;i<count_p;i++)

	{
		p[i]=((edge[i].x-center2.x)*(edge[i].x-center2.x)+(edge[i].y-center2.y)*(edge[i].y-center2.y));
	}      
	//在半径序列中等间距的取偶数个组成新的序列 r

	for(i=1;i<count_p/2+1;i++)
	{
		r[i]=p[i*2];
	}
	double sum,g=100000000,g1;
	int h=0;
	for(i=1;i<count_p/4+1;i++)
	{
		sum=0;g1=0;
		for(j=1;j<count_p/4+1;j++)
		{
			if((i-j)<=0)
			{
				sum=fabs(r[i+j]-r[count_p/2+i-j]);
			}
			else
			{
				sum=fabs(r[i+j]-r[i-j]);
			}
			g1=g1+sum;
		}
		if(g1<g)
		{
			g=g1;
			h=i;
		}
	}
	double distance1,distance2;
	distance1=fabs(double(((edge[h*2].x-x0)*(edge[h*2].x-x0)+(edge[h*2].y-y0)*(edge[h*2].y-y0))));
	distance2=fabs(double((edge[count_p/2+h*2].x-x0)*(edge[count_p/2+h*2].x-x0)+(edge[count_p/2+h*2].y-y0)*(edge[count_p/2+h*2].y-y0)));
	if(distance1<distance2) //根据点与形心的距离判断花萼点
	{
		stem.x=edge[h*2].x;
		stem.y=edge[h*2].y;
	}
	else
	{
		stem.x=edge[count_p/2+h*2].x;
		stem.y=edge[count_p/2+h*2].y;
	}
	return stem;
}