#include "concer_traffic_api.h"
using namespace cv;
using namespace std;
/************************************************************************
* function: this function: extract concer from every box or two point ,
*           only appiy fit approx Poly,sort order of points,with whole picture;
* parameter:
*     输入       srcImage:		原始图像，
*     输入       maskImage:		掩膜图像,原始图像经过语义分割二值化图像
*     输出       roadlinepoints:	提取道路线的角点像素坐标
* return: void
*********************************************************************/
void findroadconcers(Mat srcImage,Mat maskImage, vector<vector<Point> > & roadlinepoints)//int method
{
    float k=0.6;
    Mat  grayImage;
    //    srcImage=srcImage(Rect(0,srcImage.rows/2, srcImage.cols, srcImage.rows/2));
    //    maskImage=maskImage(Rect(0,maskImage.rows/2, maskImage.cols, maskImage.rows/2));
    cvtColor( srcImage, grayImage, CV_BGR2GRAY );
    blur(grayImage, grayImage, Size(3,3));

    //  erode operation
    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
    //  Mat element = getStructuringElement(MORPH_CROSS, Size(5, 5));
    Mat out;
    erode(maskImage, out, element);
    //dilate operation
    Mat element2 = getStructuringElement(MORPH_RECT, Size(5, 5));
    //   Mat element2 = getStructuringElement(MORPH_CROSS, Size(7, 7));
    Mat out2;
    dilate(out,out2, element2);
    Mat element3 = getStructuringElement(MORPH_RECT, Size(5, 5));//7.7
    //    Mat element3 = getStructuringElement(MORPH_CROSS, Size(5, 5));
    Mat out3;
    dilate(out2,out3, element3);
    //    imshow("dilate3", out3);

    //floodFillborder  ++++++++++++++++++++++++++++++++++++++++
    Mat edge;
    //????Canny????
    Size ksize(5,5);
    GaussianBlur(grayImage,grayImage,ksize,1);//0,1,2
    Canny(grayImage, edge,30, 90,3,true);//20, 60;30, 90

    //findContours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    /// Find contours
    //findContours(out3, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    findContours(out3, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0) );
    /// Approximate contours to polygons + get bounding rects and circles
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );


    for( int i = 0; i < contours.size(); i++ )
    {
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true);//
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );
    }

    Mat mask_edge;
    //vector<vector<Point> > contours_poly3;//roadlinepoints;
    vector<Vec4i> hierarchy3;


    for( int i = 0; i< contours.size(); i++ )
    {
        double contours_area=contourArea(contours[i]);
        float heigh_roi=srcImage.rows*k;
        float k1=-0.56,b1=720.0,k2=0.56,b2=-0.0;//float k1=-1.125,b1=720.0,k2=1.125,b2=-720.0;
        if(contours_area>5000||contours_area<100||(boundRect[i].tl().y<heigh_roi)||(boundRect[i].br().y>680)||boundRect[i].tl().y<(k1*boundRect[i].tl().x+b1)||boundRect[i].tl().y<(k2*boundRect[i].tl().x+b2))//2000,5000
            continue;
        Mat mask_image_roi=Mat::zeros(srcImage.size( ), CV_8UC1);
        Mat mm=mask_image_roi(Rect(boundRect[i]));//?????????????boundRect??С??ROI
        mm={Scalar(1)};//change color of roi

        //mask with out to edge
        mask_edge=edge.mul(mask_image_roi);

        //imshow("mask_edge_show",mask_edge);
        vector<vector<Point> > contours2;
        vector<Vec4i> hierarchy2;
        /// Find contours
        findContours(mask_edge, contours2, hierarchy2, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0) );
        /// Approximate contours to polygons _ get bounding rects and circles
        vector<vector<Point> > contours_poly2( contours2.size() );

        //                cout<<"contours2.size()=1"<<contours2.size()<<endl;
        for( int i = 0; i < contours2.size(); i++ )
        {
            approxPolyDP( Mat(contours2[i]), contours_poly2[i], 3, true );//3  5  7

            if(4==contours_poly2[i].size())//get four points
            {
                int ty=(contours_poly2[i][0].y+contours_poly2[i][2].y)/2;
                //                if(contours_poly2[i][1].y>ty && contours_poly2[i][3].y<ty)
                if(contours_poly2[i][1].y<ty && contours_poly2[i][3].y>ty)
                {
                    Point temp;
                    temp=contours_poly2[i][0];
                    contours_poly2[i][0]=contours_poly2[i][1];
                    contours_poly2[i][1]=contours_poly2[i][2];
                    contours_poly2[i][2]=contours_poly2[i][3];
                    contours_poly2[i][3]=temp;

                }
                roadlinepoints.push_back(contours_poly2[i]);
                hierarchy3.push_back(hierarchy2[i]);
            }
        }

    }
}

