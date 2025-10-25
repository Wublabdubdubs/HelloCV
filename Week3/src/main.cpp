#include<iostream>
#include<vector>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void getContours(Mat input,Mat output,int tpye)
{
    vector<vector<Point> >contours;
    vector<Vec4i> hierarchy;
    
    findContours(input,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    vector<vector<Point> > Poly(contours.size());
    vector<Rect> bdRect(contours.size());
    // cout<<"-----------------------------\n";
    for(int i=0;i<contours.size();i++)
    {
        int S=contourArea(contours[i]);
        Mat tmp(input.rows,input.cols,CV_8UC3,Scalar(255,255,255));
        drawContours(tmp,contours,i,Scalar(255,0,255),FILLED);
        // imshow("IDP",tmp);
        // imshow("tmp",output);
        // waitKey(0);
        // cout<<S<<"\n";
        if(S>30000)
        {

            double peri =arcLength(contours[i],1);
            approxPolyDP(contours[i],Poly[i],0.02*peri,1);
            // drawContours(output,Poly,i,Scalar(255,0,255),2);
            bdRect[i]=boundingRect(Poly[i]);
            if(tpye==1)
            {
                putText(output,"Green",Point(10,50),FONT_HERSHEY_COMPLEX,1.5,Scalar(0,255,0),3);
                rectangle(output,bdRect[i].tl(),bdRect[i].br(),Scalar(0,0,255),3);
            }
            else 
            {
                putText(output,"Red",Point(10,50),FONT_HERSHEY_COMPLEX,1.5,Scalar(0,0,255),3);
                rectangle(output,bdRect[i].tl(),bdRect[i].br(),Scalar(0,255,0),3);

            }
        }
    }
    return ;
}

// int H_min,S_min,V_min;
// int H_max,S_max,V_max;

int main()
{
    // namedWindow("Control",(720,720));
    // createTrackbar("H_min","Control",&H_min,360);
    // createTrackbar("H_max","Control",&H_max,360);
    // createTrackbar("S_min","Control",&S_min,255);
    // createTrackbar("S_max","Control",&S_max,255);
    // createTrackbar("V_min","Control",&V_min,255);
    // createTrackbar("V_max","Control",&V_max,255);
    VideoCapture cap("../data/trafficLight.mp4");
    VideoWriter wt("../output/ans.mp4",VideoWriter::fourcc('m','p','4','v'),30,Size(1920,1080));
    while(1)
    {
        // if(!cap.isOpened()) break;
        Mat ori,img_green,img_red;
        cap.read(ori);
        if(ori.empty()) break;
        cout<<ori.rows<<" "<<ori.cols<<"\n";
        inRange(ori,Scalar(100,129,0),Scalar(207,247,80),img_green);
        inRange(ori,Scalar(13,14,131),Scalar(116,120,255),img_red);

        GaussianBlur(img_green,img_green,Size(5,5),0,0);
        GaussianBlur(img_red,img_red,Size(5,5),0,0);
        Canny(img_green,img_green,100,200);
        Canny(img_red,img_red,100,200);
        Mat K=getStructuringElement(MORPH_RECT,Size(5,5));
        dilate(img_green,img_green,K);
        erode(img_green,img_green,K);
        dilate(img_red,img_red,K);
        erode(img_red,img_red,K);
        
        getContours(img_green,ori,1);
        getContours(img_red,ori,2);
        imshow("Ori",ori);
        wt.write(ori);
        if(waitKey(1)>=0) break;
    }
    cap.release();
    return 0;
}
