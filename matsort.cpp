#include "libfreenect.h"
#include <libfreenect_sync.h>
#include <pthread.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include "libfreenect.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"



using namespace cv;
using namespace std;

class Mutex {
public:
	Mutex() {
		pthread_mutex_init( &m_mutex, NULL );
	}
	void lock() {
		pthread_mutex_lock( &m_mutex );
	}
	void unlock() {
		pthread_mutex_unlock( &m_mutex );
	}
private:
	pthread_mutex_t m_mutex;
};

/**************Global Variables**************/
Mutex m_depth_mutex;
unsigned char *KinectDepth = 0;
unsigned char *data=0;
unsigned int timestamp;
unsigned int timestamp1;
Mat kinectDepthImage1(Size(640,480),CV_16UC1,Scalar(0));
Mat kinectvideoImage(Size(640,480),CV_8UC3,Scalar(0));
Mat image(Size(640,480),CV_8UC3,Scalar(0));
uint8_t *depth_mid;
int noofcups=0;
uint16_t t_gamma[10000];
pthread_t freenect_thread;
int wcup=0;int hcup=0;int r=0;int s=0;
Mat kinectvideoImagecrop(Size(590,440),CV_8UC3,Scalar(255,255,255));
int nolevels=0;
int cl=0,cr=0,rd=0,ru=0;
int ca[100];
int cb[100];
int cradius[100];
int flag[100];

int innerbandthreshold=2;
int outerbandthreshold=5;

int humangui=0;
int humanblue=0;
/*************************************** Frame *******************************************************************************/


		
/***************************Functions************************************************************************************/
void windowDraw_depth()
  {
	m_depth_mutex.lock();
	freenect_sync_get_depth((void**)&KinectDepth, &timestamp, 0, FREENECT_DEPTH_REGISTERED);
	kinectDepthImage1.data= KinectDepth;
	m_depth_mutex.unlock();
  }
/*FREENECT_DEPTH_11BIT = 0, /**< 11 bit depth information in one uint16_t/pixel 
FREENECT_DEPTH_10BIT = 1, /**< 10 bit depth information in one uint16_t/pixel 
FREENECT_DEPTH_11BIT_PACKED = 2, /**< 11 bit packed depth information 
FREENECT_DEPTH_10BIT_PACKED = 3, /**< 10 bit packed depth information 
FREENECT_DEPTH_REGISTERED = 4, /**< processed depth data in mm, aligned to 640x480 RGB 
FREENECT_DEPTH_MM = 5, /**< depth to each pixel in mm, but left unaligned to RGB image 
FREENECT_DEPTH_DUMMY = 2147483647, /**< Dummy value to force enum to be 32 bits wide */

/*******************Pixel Filtering******************************/

Mat filter(Mat kinectDepthImagefilt)
{

Mat kinectDepthImagenew(Size(640,480),CV_16UC1,Scalar(0));
int widthbound= 615;
int heightbound= 479;

	

for (int x=25;x<616;x++)
	{
		for(int y=40;y<480;y++)
		{
			if(kinectDepthImagefilt.at<uint16_t>(Point(x,y))==0)
			{
				int filtercollection[24][2];
				for(int g=0;g<24;g++)
				{
					for(int h=0;h<2;h++)
					{
						filtercollection[g][h]=0;
					}
				}

				int innerbandcount=0;
				int outerbandcount=0;
for(int yi=-2;yi<3;yi++)
{
	for(int xi=-2;xi<3;xi++)
	{
		if( (xi!=0) || (yi!=0) )
		{
			int xsearch = x + xi;
			int ysearch = y + yi;		
			if ( (xsearch >= 0) && (xsearch <= widthbound) && (ysearch >= 0) && (ysearch <= heightbound))
                        {
				if (kinectDepthImagefilt.at<uint16_t>(Point(xsearch,ysearch)) != 0)    
				{
					for (int i = 0; i < 24; i++)
                			{   
						if (filtercollection[i][0] == kinectDepthImagefilt.at<uint16_t>(Point(xsearch,ysearch)))
                  				{     
							filtercollection[i][1]++;
                    					break;
						}
						else if (filtercollection[i][0] == 0)
                  				{
							filtercollection[i][0] = kinectDepthImagefilt.at<uint16_t>(Point(xsearch,ysearch));
                    					filtercollection[i][1]++;
                    					break;
						}
					}
					if (yi != 2 && yi != -2 && xi != 2 && xi != -2)
                  			innerbandcount++;
                			else
                  			outerbandcount++;
				}
			}
		}
	}
}

				if ( (innerbandcount >= innerbandthreshold) || (outerbandcount >= outerbandthreshold) )
				{
					int f_frequency = 0;
        				int f_depth = 0;
					
					for (int i = 0; i < 24; i++)
        				{
						if (filtercollection[i][0] == 0)
            					break;
						if (filtercollection[i][1] > f_frequency)
          					{
            						f_depth = filtercollection[i][0];
            						f_frequency = filtercollection[i][1];
						}
					}
					kinectDepthImagenew.at<uint16_t>(Point(x,y)) = f_depth;
				}
			}
			else
    			{
      			      kinectDepthImagenew.at<uint16_t>(Point(x,y)) = kinectDepthImagefilt.at<uint16_t>(Point(x,y));
    			}
		}
}

return kinectDepthImagenew;
}


/**************Crop RGB to Depth image**************************/
void cropRGBtoregdepthimage()
{
	
	for(int p=25;p<616;p++)
	{
		for(int q=40;q<480;q++)
		{
		kinectvideoImagecrop.at<Vec3b>(Point(p-25,q-40))[0]= kinectvideoImage.at<Vec3b>(Point(p,q))[0];
		kinectvideoImagecrop.at<Vec3b>(Point(p-25,q-40))[1]= kinectvideoImage.at<Vec3b>(Point(p,q))[1];
		kinectvideoImagecrop.at<Vec3b>(Point(p-25,q-40))[2]= kinectvideoImage.at<Vec3b>(Point(p,q))[2];
		}
	}

}
/***************************************************************/
void windowDraw_video()
{
m_depth_mutex.lock();
freenect_sync_get_video((void**)(&data), &timestamp, 0, FREENECT_VIDEO_RGB); //FREENECT_VIDEO_RGB
kinectvideoImage.data= data;
cvtColor(kinectvideoImage,kinectvideoImage,CV_RGB2BGR);

cropRGBtoregdepthimage();


int param1=50;
int param2=50;
Mat blurred;
GaussianBlur(kinectvideoImage, blurred, Size(param2/10*2+1,param2/10*2+1), 0, 0);
kinectvideoImage = kinectvideoImage*(1+param1/10) + blurred*(-param1/10);

//namedWindow( "Sharpened image",CV_WINDOW_AUTOSIZE);
//	imshow("Sharpened image", kinectvideoImage);
m_depth_mutex.unlock();
 
}




/*****************************************GUI RGB******************************************/
Mat GUIRGB(Mat guiimage,int noofcups)
{
putText(guiimage, "RGB IMAGE", cvPoint(900,50), FONT_HERSHEY_DUPLEX, 0.8, cvScalar(0,0,0), 2, CV_AA);


/**RGB image cropped according to registered depth image on GUI image**/	

	for(int p=650;p<1240;p++)
	{
		for(int q=(90);q<530;q++)
		{
		guiimage.at<Vec3b>(Point(p,q))[0]= kinectvideoImagecrop.at<Vec3b>(Point(p-650,(q-90)))[0];
		guiimage.at<Vec3b>(Point(p,q))[1]= kinectvideoImagecrop.at<Vec3b>(Point(p-650,(q-90)))[1];
		guiimage.at<Vec3b>(Point(p,q))[2]= kinectvideoImagecrop.at<Vec3b>(Point(p-650,(q-90)))[2];
		}
	}

return guiimage;
}



Mat cupdrawred(Mat guiimage,int indexrow[],int ca[],int cb[],int noofcups1,int noofcups)
{	
	if(noofcups%2 ==0)
	{
		int k=0;
		for(int p=0;p<noofcups1;p++)
		{	
			cl=23+(p*r);
			cr=((p+1)*r)-3;
			for(int q=0;q<2;q++)
			{
				rd=((q+1)*s)-3;
				ru=(q*s)+indexrow[k]+23;
				k++;
				
				for (int g=cl;g<cr;g++)
				{
					for( int h=rd;h>ru;h--)
					{
						
		guiimage.at<Vec3b>(Point(g,h))[0]=0;guiimage.at<Vec3b>(Point(g,h))[1]=0;guiimage.at<Vec3b>(Point(g,h))[2]=255;
			
					}
	
				}
					if(q==0)
					{
					stringstream naming;
					naming<<(k);
			putText(guiimage, naming.str(), cvPoint((28+(p*r)),50), FONT_HERSHEY_DUPLEX, 0.8, cvScalar(0,0,0), 2, CV_AA);
					}
					if(q==1)
					{
					stringstream naming;
					naming<<(k);
			putText(guiimage, naming.str(), cvPoint((28+(p*r)),(s+50)), FONT_HERSHEY_DUPLEX, 0.8, cvScalar(0,0,0), 2, CV_AA);
					}
			}
		}
	}
	else
	{
		int k=0;
		for(int p=0;p<noofcups1;p++)
		{	
			
			cl=23+(p*r);
			cr=((p+1)*r)-3;
			for(int q=0;q<2;q++)
			{	
				
				rd=((q+1)*s)-3;
				ru=(q*s)+indexrow[k]+23;
				k++;
					
				for (int g=cl;g<cr;g++)
				{	
					for( int h=rd;h>ru;h--)
					{
				
		guiimage.at<Vec3b>(Point(g,h))[0]=0;guiimage.at<Vec3b>(Point(g,h))[1]=0;guiimage.at<Vec3b>(Point(g,h))[2]=255;
			
					}
				}
					if(q==0)
					{
					stringstream naming;
					naming<<(k);
			putText(guiimage, naming.str(), cvPoint((28+(p*r)),50), FONT_HERSHEY_DUPLEX, 0.8, cvScalar(0,0,0), 2, CV_AA);
					}
					if(q==1)
					{
					stringstream naming;
					naming<<(k);
			putText(guiimage, naming.str(), cvPoint((28+(p*r)),(s+50)), FONT_HERSHEY_DUPLEX, 0.8, cvScalar(0,0,0), 2, CV_AA);
					}
			if(p==(noofcups1-1)) {break;}
			}
		}
	


	}

	
return guiimage;
}







/*******************************box filling function****************/
void boxfill(Mat guiimage,int nolevels,int noofcups,int flag[],int guilevelvalue[],int ca[],int cb[],int noofcups1)
{

	int indexrow[100];
	for(int n=0;n<100;n++)
	{
		indexrow[n]=0;
	}


for(int i=0;i<noofcups;i++)
{
indexrow[i]=guilevelvalue[nolevels-(flag[i])];
	
}




	
	cupdrawred(guiimage,indexrow,ca,cb,noofcups1,noofcups);


}

/*****************************************GUI image****************************************/



void GUI(int noofcups,int nolevels,int flag[],int ca[],int cb[],int human)
{

Mat guiimage(Size(1300,676),CV_8UC3,Scalar(255,255,255));
/******Number of Black boxes outline for Cup level**********/
int x=77;
if(noofcups==0)
{
	noofcups=1;
}
int noofcups1=noofcups;
int oddcups=0;
hcup= (676-((2+1)*20))/(2);


		if(noofcups1%2 != 0)
		{
			oddcups=1;
			noofcups1+=1;
		}
		noofcups1/=2;
		wcup= (648-((noofcups1+1)*20))/(noofcups1);
		if(wcup>137)
		{
			wcup=137;
		}
		 r=wcup+20;
		 s=hcup+20;


		for(int p=0;p<noofcups1;p++)
		{
			rectangle(guiimage, Point((20+(p*r)),20), Point((r+(p*r)),(s)), Scalar(0,0,0), 2, 8, 0);
			
			for(int y=0;y<5;y++)
			{
				putText(guiimage,"_", cvPoint((20+(p*r)),(20+(y*x))), FONT_HERSHEY_DUPLEX, 0.8, cvScalar(0,0,0), 1, CV_AA);
			}		
			
		}
		if(oddcups==1)
		{
		
			for(int p=0;p<(noofcups1-1);p++)
			{
				rectangle(guiimage, Point((20+(p*r)),(20+s)), Point((r+(p*r)),(2*s)), Scalar(0,0,0), 2, 8, 0);
				for(int y=0;y<5;y++)
				{
				putText(guiimage,"_", cvPoint((20+(p*r)),(348+(y*x))), FONT_HERSHEY_DUPLEX, 0.8, cvScalar(0,0,0), 1, CV_AA);
				}
			}
		}
		else
		{
			for(int p=0;p<(noofcups1);p++)
			{
				rectangle(guiimage, Point((20+(p*r)),(20+s)), Point((r+(p*r)),(2*s)), Scalar(0,0,0), 2, 8, 0);
				for(int y=0;y<5;y++)
				{
				putText(guiimage,"_", cvPoint((20+(p*r)),(348+(y*x))), FONT_HERSHEY_DUPLEX, 0.8, cvScalar(0,0,0), 1, CV_AA);
				}
			}
		}



int guilevelvalue[nolevels];
	for(int p1=0;p1<nolevels;p1++)
	{
		guilevelvalue[p1]=0;
		
	}

	for(int p1=0;p1<nolevels;p1++)
	{
		guilevelvalue[p1]=(p1 * hcup/nolevels);

	}


GUIRGB(guiimage,noofcups);	
boxfill(guiimage,nolevels,noofcups,flag,guilevelvalue,ca,cb,noofcups1);

	if(human==1)
	{
	
		human=0;
		if(humanblue==1){
			cvDestroyWindow("COFFEE LEVEL DETECTION1");
			//humanblue=0;
				}
		namedWindow( "COFFEE LEVEL DETECTION",CV_WINDOW_AUTOSIZE);
		imshow("COFFEE LEVEL DETECTION", guiimage);
		humangui=1;
	}


	else
	{

		if(humangui==1){
			cvDestroyWindow("COFFEE LEVEL DETECTION");
			//humangui=0;
			}
		//Mat bluescreen(Size(1300,676),CV_8UC3,Scalar(255,0,0));
		Mat bluescreen=imread("here.jpg",1);
	putText(bluescreen,"Where Did The Humans Go??!!", cvPoint(150,600), FONT_HERSHEY_DUPLEX, 2, cvScalar(255,255,255), 2, CV_AA);
		namedWindow( "COFFEE LEVEL DETECTION1",CV_WINDOW_AUTOSIZE);
		imshow("COFFEE LEVEL DETECTION1", bluescreen);
		humanblue=1;
	
	}
 
}




/*********************************Main Program**************************************************************************/

int main(int argc, char **argv) {


for (int i=0; i<10000; i++) {
		float v = i/2048.0;
		v = powf(v, 3)* 6;
		t_gamma[i] = v*6*256;
	
	}

	
	bool die(false);

	
        Mat src_gray;
        vector<Vec3f> circles;
        
	
	

	int framecount=0;
	int fmax2[100][10];
	for(int g=0;g<100;g++)
	{
		for(int h=0;h<10;h++)
		{
			fmax2[g][h]=0;	
		}
	}

	int fSubcupcentre[100][10];
	for(int g=0;g<100;g++)
	{	
		for(int h=0;h<10;h++)
		{
			fSubcupcentre[g][h]=0;	
		}
	}
	
	

     while (!die) {
    	
	

	windowDraw_depth();
/************************************************ 16 bit depth data to 11 bit depth data **********************************/
	
	Mat kinectDepthImage(Size(640,480),CV_16UC1,Scalar(0));
	/*uint16_t *pvldepth = (uint16_t*)malloc(640*480);

	for (int i = 0; i < 640*480; i++) 
	{
	uint16_t *depthh= (uint16_t*)kinectDepthImage1.data;
	uint16_t *pvldepth= (uint16_t*)kinectDepthImage.data;
	pvldepth[i] = t_gamma[depthh[i]];
	}
/****************************************************************************************************************************/
	
	
	//for (int p=0;p<640;p++)
	//{
		//for(int q=0;q<480;q++)
		//{
		//	kinectDepthImage.at<uint16_t>(Point(p,q))=kinectDepthImage1.at<uint16_t>(Point(p,q));
		//}
	//}
////////////////////////////////////////////Depth Pixel Filtering////////////////////////////////////////////////////////////////////////

Mat kinectDepthImagen= filter(kinectDepthImage1);	
kinectDepthImage= filter(kinectDepthImagen);

/******************************************************Human Detection********************************************************/
int human=0;
for(int hr=0;hr<640;hr++)
{
	for(int hc=0;hc<480;hc++)
	{	

		if((kinectDepthImage.at<uint16_t>(Point(hr,hc)) >0)&&(kinectDepthImage.at<uint16_t>(Point(hr,hc)) < 1730))
		{
			human=1;break;
		}
		else
		{
			human=0;
		}
	}
	if(human==1){
		break;
		}
}



/*****************************************************************************************************************************/	
	
	windowDraw_video();
	
       			
	cvtColor( kinectvideoImage,src_gray,CV_RGB2GRAY );	
        HoughCircles(  src_gray, circles, CV_HOUGH_GRADIENT, 1, 5 ,25,30, 9, 17 );


/*************************************************************************************************************************/

	noofcups=circles.size();
	int max2[100];
	for(int n=0;n<100;n++)
	{
		max2[n]=0;
	}	
	int Subcupcentre[100];
	for(int n=0;n<100;n++)
	{
		Subcupcentre[n]=0;
	}


	for(int n=0;n<100;n++)
	{
		ca[n]=0;
		cb[n]=0;
		cradius[n]=0;
	}


	
	
	
	
       	for( size_t i = 0; i < circles.size(); i++ )
	{
         		ca[i]=cvRound(circles[i][0]);
         		cb[i]=cvRound(circles[i][1]);       
			cradius[i] =cvRound(circles[i][2]);
	}

int tempa=0;
int tempb=0;
int tempr=0;

for(int i=0;i<noofcups;i++)
{
	for(int j=i+1;j<noofcups;j++)
	{
		if(ca[i]>ca[j])
		{
			tempa=ca[i];
			ca[i]=ca[j];
			ca[j]=tempa;
			
			tempb=cb[i];
			cb[i]=cb[j];
			cb[j]=tempb;

			tempr=cradius[i];
			cradius[i]=cradius[j];
			cradius[j]=tempr;
			
		}
	}
}


int a=0;
int b=0;
int radius=0;
for(size_t i=0; i<noofcups; i++)
{
	a=ca[i];
	b=cb[i];
	radius=cradius[i];
	Point center(a,b); 
	//cout<<a<<"::"<<b<<endl;
	//cout<<kinectDepthImage.at<uint16_t>(Point(a,b))<<endl; 
/*********************************************************************************************************************/
			
int Z=(2*radius);
stringstream naming;
naming<<(i+1);
//naming<<"("<<a<<","<<b<<")";
putText(kinectvideoImagecrop, naming.str() , cvPoint((a-Z-5),(b-Z)), FONT_HERSHEY_DUPLEX, 0.8, cvScalar(0,0,255), 2, CV_AA);
//cout<<a<<":"<<b<<endl;
/********************Average of pixels around the cup***************************************/

			Mat whitecircle(Size(640,480),CV_16UC1,Scalar(0));
			for(int p=0;p<kinectDepthImage.cols;p++)
			{
				for(int q=0;q<kinectDepthImage.rows;q++)
				{
					(  whitecircle.at<uint16_t>(Point(p,q))  )= (  kinectDepthImage.at<uint16_t>(Point(p,q))  );
				}
			}



	 		circle( whitecircle, center, (Z), Scalar(65535), 4, 8, 0 );




/****************************************/



int avg=0; 
int count=0;
for (int x=0;x<whitecircle.cols;x++)
{	
	for(int y=0;y<whitecircle.rows;y++)
	{
				

		if( ( whitecircle.at<uint16_t>(Point(x,y)) ) == 65535)
		{
			count++;			
			avg+=kinectDepthImage.at<uint16_t>(Point(x,y));
			
		}

	}
}

avg/=count;

/*******************************************Rim (min) value by circle of radius*********************/

			Mat whitecirclerim(Size(640,480),CV_16UC1,Scalar(0));
			for(int p=0;p<kinectDepthImage.cols;p++)
			{
				for(int q=0;q<kinectDepthImage.rows;q++)
				{
					(  whitecirclerim.at<uint16_t>(Point(p,q))  ) = (  kinectDepthImage.at<uint16_t>(Point(p,q))  );
				}
			}


	 		circle( whitecirclerim, center, (radius), Scalar(65535), 4, 8, 0 );//thickness,linetype

int rimmin=0;
int countrim=0;
for (int x=0;x<whitecirclerim.cols;x++)
{	
	for(int y=0;y<whitecirclerim.rows;y++)
	{
				

		if( ( whitecirclerim.at<uint16_t>(Point(x,y)) ) == 65535)
		{
			countrim++;			
		}

	}
}

int rimminarray[countrim];
for (int x=0;x<countrim;x++)
{	
	rimminarray[x]=0;				
}

int arr=0;
for (int x=0;x<whitecirclerim.cols;x++)
{	
	for(int y=0;y<whitecirclerim.rows;y++)
	{
				

		if( ( whitecirclerim.at<uint16_t>(Point(x,y)) ) == 65535)
		{
			rimminarray[arr] = kinectDepthImage.at<uint16_t>(Point(x,y));
			arr++;			
		}

	}
}

rimmin=rimminarray[0];

for (int x=0;x<countrim;x++)
{	if(rimminarray[x]==0) {continue;}	
	if(rimminarray[x]<rimmin)
	{
		rimmin=rimminarray[x];
	}
}	


/************************Dynamic thresholding****************************************/



max2[i]=avg-rimmin;
max2[i]=abs(max2[i]);

//cout<<"Cup height"<<"::"<<max2<<endl;
/************************Cup level calculation*******************************************************/

       		
         		
int cupcentreavg=0;
int X[7]={(a-3),(a-2),(a-1),a,(a+1),(a+2),(a+3)};
int Y[7]={(b-3),(b-2),(b-1),b,(b+1),(b+2),(b+3)};

for (int g=0;g<7;g++)
{
	for(int h=0;h<7;h++)
	{
		cupcentreavg+=	(kinectDepthImage.at<uint16_t>(Point((X[g]),(Y[h]))));
	}
}
   
cupcentreavg/=49;



		
			
		Subcupcentre[i]=cupcentreavg-avg;
		Subcupcentre[i]=abs(Subcupcentre[i]);
	
}//circle for loop	

/****************************************Filtering**********************************************************************************/

for( int i=0;i<noofcups;i++)
{
fmax2[i][framecount]=max2[i];
fSubcupcentre[i][framecount]=Subcupcentre[i];
}
framecount++;

if(framecount==9)
{
	
	for( int i=0;i<noofcups;i++)
	{
	
	fmax2[i][framecount]=max2[i];
	fSubcupcentre[i][framecount]=Subcupcentre[i];
	
			int tempmax2=0;
			for(int im=0;im<10;im++)
			{
				for(int jm=im+1;jm<10;jm++)
				{
					if(fmax2[i][im]>fmax2[i][jm])
					{
						tempmax2=fmax2[i][im];
						fmax2[i][im]=fmax2[i][jm];
						fmax2[i][jm]=tempmax2;
					}
				}
			}
	/*for(int te=0;te<10;te++){
			cout<<fmax2[i][te]<<endl;}cout<<"#"<<endl;*/
			int avgmax2=0;
			for(int g=3;g<7;g++)
			{
				avgmax2+=fmax2[i][g];
			}
			max2[i]=avgmax2/4;
			
			int tempfSubcupcentre=0;
			for(int is=0;is<10;is++)
			{
				for(int js=is+1;js<10;js++)
				{
					if(fSubcupcentre[i][is]>fSubcupcentre[i][js])
					{
						tempfSubcupcentre=fSubcupcentre[i][is];
						fSubcupcentre[i][is]=fSubcupcentre[i][js];
						fSubcupcentre[i][js]=tempfSubcupcentre;
					}
				}
			}
	
	
			int avgSubcupcentre=0;
			for(int g=3;g<7;g++)
			{
				avgSubcupcentre+=fSubcupcentre[i][g];
			}
			Subcupcentre[i]=avgSubcupcentre/4;
		//cout<<Subcupcentre[i]<<"::"<<max2[i]<<endl;
	}//for

framecount=0;


/*************************************** Levels *****************************************/
for(int i=0;i<noofcups;i++)
{
nolevels=6; //Number of levels
int valofeachlevel=(max2[i])/(nolevels);

int levelvalue[nolevels];

	for(int p=0;p< nolevels;p++)
	{
		levelvalue[p]=0;
		
	}
	
	for(int p=0;p< nolevels;p++)
	{
		levelvalue[p]=(p * valofeachlevel);
			
	}
	
/************************************ Cup liquid level *********************************/


for(int p=0;p< nolevels;p++)
	{	
		if( (Subcupcentre[i]>(levelvalue[p])) && (Subcupcentre[i]<(levelvalue[(p+1)])) )
		{	
			
				if(p>(nolevels/2)){p=p+1;}
				else{p=p;}
				flag[i]=p;
				
		}	 
	}


}

GUI(noofcups, nolevels,flag,ca,cb,human);
}




/*************************************************************************************************************************************/





   if(cvWaitKey(10)==27)
	break;


 }// while main  


return 0;
}//main
