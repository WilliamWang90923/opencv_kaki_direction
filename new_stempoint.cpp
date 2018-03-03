#include "foo.h"

int main()  
{  
	Mat src = imread("004-11.png");
	Mat src_gray;
	Mat blurimage,dst;
	Mat canny_output;
	int imax = 0; //number of biggest contour
    int imaxcontour = -1; //size of of biggest contour

	cvtColor( src, src_gray, CV_BGR2GRAY );  
	//Mat ele = getStructuringElement(MORPH_RECT, Size(3, 3));
	//dilate(src_gray, src_gray, ele);
	blur( src_gray, blurimage, Size(3,3) );  
	threshold(blurimage,dst,200,255,0);
	//cvtColor( src, src_gray, CV_BGR2GRAY );//灰度化      
    //GaussianBlur( src, src, Size(3,3), 0.1, 0, BORDER_DEFAULT );   
    //blur( src_gray, src_gray, Size(3,3) ); //滤波  

	vector<vector<Point> > contours;      
    vector<Vec4i> hierarchy;    
    //canny   
    Canny( dst, canny_output,40, 40*3, 3 );  
	findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL ,CV_CHAIN_APPROX_NONE, Point(0, 0) ); 

	//vector<Point2f> center=GetCenterPoint(src);
	//Mat dst = rotate_pic(src,center,60,1);
	
	vector<Moments> mu(contours.size() );       
    for( int i = 0; i < contours.size(); i++ )     
    {   
        mu[i] = moments( contours[i], false );   
    }     
    //计算轮廓的质心     
    vector<Point2f> mc( contours.size() );      
    for( int i = 0; i < contours.size(); i++ )     
    {   
        mc[i] = Point2d( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );   
    }     
    //画轮廓及其质心并显示      
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );         
    for( int i = 0; i< contours.size(); i++ )      
    {    
		int itmp =  contourArea(contours[i]);
		if (imaxcontour < itmp )
		{
            imax = i;
            imaxcontour = itmp;
        }
        Scalar color = Scalar( 255, 0, 0);        
        drawContours( drawing, contours, i, color, 1, 8, hierarchy);     //0, Point()    
        circle( drawing, mc[i], 5, Scalar( 0, 0, 255), -1, 8, 0 );                
        rectangle(drawing, boundingRect(contours.at(i)), cvScalar(0,255,0));              
        char tam[100];   
        sprintf(tam, "(%0.0f,%0.0f)",mc[i].x,mc[i].y);   
        putText(drawing, tam, Point(mc[i].x, mc[i].y), FONT_HERSHEY_SIMPLEX, 0.4, cvScalar(255,0,255),1);     
    }     
	
	Mat drawing_max = Mat::zeros( canny_output.size(), CV_8UC3 );
	drawContours( drawing_max, contours, imax, Scalar( 255, 0, 0), CV_FILLED, 8, hierarchy);

	RotatedRect rect=minAreaRect(contours[imax]);  
	Point2f P[4];  
	rect.points(P);  
	for(int j=0;j<=3;j++)  
	{  
		line(drawing_max,P[j],P[(j+1)%4],Scalar(0,0,0),2);  
	}  

	int count=0;
	const int Size = 1000;
    int maxdist = 0;
	int mindist = 5;
    Point center_round=(0,0),temp,tanpointA=(0,0),tanpointB=(0,0),tanpointC=(0,0);
	int tangentA = 0,tangentB = 0;
	int *allpoints=new int[Size];

	/* if (maxdist<=0)
	{
		center_round = Point(int(P[1].x+P[2].x+P[3].x+P[0].x)/4,int(P[1].y+P[2].y+P[3].y+P[0].y)/4);
	} */
	//get center point
    for(int i=(boundingRect(contours.at(imax)).x+boundingRect(contours.at(imax)).width/4);i<(boundingRect(contours.at(imax)).x+boundingRect(contours.at(imax)).width*3/4);i++)
    {
        for(int j=(boundingRect(contours.at(imax)).y+boundingRect(contours.at(imax)).height/4);j<(boundingRect(contours.at(imax)).y+boundingRect(contours.at(imax)).height*3/4);j++)
        {
           int dist = pointPolygonTest(contours[imax],Point(i,j),true);
            if(dist>=maxdist)
            {
                maxdist=dist;
                center_round=Point(i,j);
            }
        }
    }
	
	cout<<"center_round="<<center_round.x<<" "<<center_round.y<<endl;
	
	//get all the tangent point
	for (int i=0;i<contours[imax].size();i++)
	{
		
		int dist =  getDistance (center_round,contours[imax][i]);  
		circle(drawing_max,contours[imax][i],1,Scalar(0,255,255),1, 8, 0);
		if((maxdist-2<dist) && (dist<=maxdist+2))
		{
			
			allpoints[count] =i;
			count++;
		}
	}

	

	for (int i=0;i<count;i++)
	{
		cout<<allpoints[i]<<endl;
		circle(drawing_max,contours[imax][allpoints[i]],3,Scalar(0,255,0),1, 8, 0);
	}

	
	int group_n=0;
	int sub_a = 0,sub_b=0;
	vector<vector<int> > vec;
	vector< int > line;   
	auto itr = vec.begin();
	/*	for(int i = 0 ; i < 10 ; i ++)
		{ 
			vec.push_back(line); 
		} */
	//vec[group_n][0] = allpoints[0];
	vec.push_back(line);
	for (int i=1;i<count;i++)
	{
		
		if (num_gap(allpoints[i-1],allpoints[i],contours[imax].size())<=5)
		{vec[sub_b].push_back(allpoints[i]);}
		else
        {sub_b++;vec.push_back(line);vec[sub_b].push_back(allpoints[i]);sub_a++;}
	}

	for (int i=0;i<vec.size();i++)	
	{
		for (int j=0;j<vec[i].size();j++)    //for testing contour points 
		{
			cout<<vec[i][j]<<" ";
		}
	   cout<<endl;
	} 
	int index=0,index2=0;
	if(vec[0].size()>=vec[1].size())
	{tangentA=vec[0].size();tangentB=vec[1].size();}
	else 
    {tangentA=vec[1].size();tangentB=vec[0].size();}

	for (int i=1;i<vec.size();i++)
	{
		if((vec[i-1].size()<=vec[i].size())&&(vec[i].size()>=tangentA))
		{
			index=i;tangentA=vec[i].size();
		}
	}
	tanpointA = contours[imax][vec[index][tangentA/2]];
	cout<<tanpointA.x<<" "<<tanpointA.y<<endl;
	itr = vec.erase(vec.begin()+index);
	tangentB = vec[0].size();

	if(vec.size()>1)
	{for (int i=1;i<vec.size();i++)
	{
		if ((vec[i-1].size()<=vec[i].size())&&(vec[i].size()>=tangentB))
		{
			tangentB = vec[i].size();
			index2 = i;
		}
	}}
	else {tangentB = vec[0].size();index2=0;}
	tanpointB = contours[imax][vec[index2][tangentB/2]];
	cout<<tanpointB.x<<" "<<tanpointB.y<<endl;

	delete [] allpoints;

	//calculate the wanted point
	float a1 = float(tanpointA.x-center_round.x)/float(tanpointA.y-center_round.y);
	float b1 = 1.0;
	float a2 = float(tanpointB.x-center_round.x)/float(tanpointB.y-center_round.y);
	float b2 = 1.0;
	float c1 = float(tanpointA.y*(tanpointA.y-center_round.y)+tanpointA.x*(tanpointA.x - center_round.x));
	c1=c1/(tanpointA.y-center_round.y);
	float c2 = float(tanpointB.y*(tanpointB.y-center_round.y)+tanpointB.x*(tanpointB.x - center_round.x));
	c2=c2/(tanpointB.y-center_round.y);
	tanpointC.x = int((c1*b2-c2*b1)/(a1*b2-a2*b1));
	tanpointC.y = int((a1*c2-a2*c1)/(a1*b2-a2*b1));


    //绘制结果
	//dist = pointPolygonTest(contours[imax],Point(950,500),true);
	cout<<"size of contour ="<<contours[imax].size()<<endl;
	cout<<"dist="<<maxdist<<endl;
	cout<<"center_round="<<center_round.x<<" "<<center_round.y<<endl;
	circle(drawing_max,center_round,maxdist,Scalar(255,255,255),1, 8, 0);
	circle(drawing_max,tanpointA,8,Scalar(0,20,255),1, 8, 0);
	circle(drawing_max,tanpointB,8,Scalar(100,25,255),1, 8, 0);
	//circle(drawing_max,tanpointC,8,Scalar(100,180,50),1, 8, 0);
	circle(drawing_max,center_round,2,Scalar(0,255,255),1, 8, 0);
	cv::line(drawing_max,tanpointC,center_round,Scalar(0,255,0),1);
	cv::line(src,tanpointC,center_round,Scalar(0,255,0),1);
	cv::line(drawing_max,tanpointA,center_round,Scalar(0,255,0),1);
	cv::line(drawing_max,tanpointB,center_round,Scalar(0,255,0),1);
	//cv::line(drawing_max,tanpointA,tanpointC,Scalar(0,255,0),1);
	//cv::line(drawing_max,tanpointB,tanpointC,Scalar(0,255,0),1);
	circle(drawing,center_round,maxdist,Scalar(255,255,255),1, 8, 0);
	imshow("src",src);  
	//imshow("dst",dst);  
	namedWindow( "Contours", CV_WINDOW_AUTOSIZE );  
	namedWindow( "Contours_max", CV_WINDOW_AUTOSIZE );
    imshow( "Contours", drawing );  
	imshow( "Contours_max", drawing_max );
    
    moveWindow("Contours",1100,20); 
	moveWindow("Contours_max",1100,20); 
    waitKey(0);       
    src.release();    
    src_gray.release();       
    return 0;   
	
}  