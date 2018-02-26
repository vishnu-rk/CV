#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class canny{
private:
    int upper;                      //upper limit  for thresholding
    int lower;                      //lower limit for thresholding-used for hysteresis
    int x,y;                        //image pixel coordinates
    Mat image;                      //image
    Mat src;                        //source image
    Mat edges;                      //edge  image
    Mat magnitude;                  //maginitude image
    Mat slopes;                     //slope  image
    void followEdges();             //hysteresis function
    void hysteresis();              //upper  thresholding function
    void nonMaximumSuppression();   //non maxima supression
    void MyCanny();                 //canny pipeline function
public:
    //constructor
    canny(Mat src,int upper,int lower):src(src),upper(upper),lower(lower){
        MyCanny();
    }; 
    ~canny(){};    
    void show_edges();              //function to display edges
    Mat get_edges();                //function to return edges
};