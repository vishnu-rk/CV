#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class canny{
private:
    int upper;
    int lower;
    int x,y;
    Mat image;
    Mat src;
    Mat edges;
    Mat magnitude;
    Mat slopes;    
    void followEdges();
    void edgeDetect();
    void nonMaximumSuppression();
    void MyCanny();
public:
    canny(Mat src,int upper,int lower):src(src),upper(upper),lower(lower){
        //cout<<"created object"<<endl;
        MyCanny();
    }; 
    ~canny(){};    
    void show_edges();
    Mat get_edges();
};