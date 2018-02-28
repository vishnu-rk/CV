#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
// struct for holding cornerness and cv::point
struct pointData { 
    float cornerResponse;
    Point point;
};
//function for sorting
struct by_cornerResponse { 
    bool operator()(pointData const &left, pointData const &right) { 
        return left.cornerResponse > right.cornerResponse;
    }
};
//struct for derivaties
struct Derivatives {
	Mat Ix;
	Mat Iy;
	Mat Ixy;
};
class Harris {
private:
	int filterRange;					//filter size
	int suppressionRadius;				//supression radius
	int radius;							//size of box around key point
	float percentage;					//percentage of best keypoints to be detected
	float k;							//multiplication factor in cornerness function
	vector<pointData> points;			//keypoints
	Mat img;							//source image
	Mat greyscaleImg;					//grayscale image
	Mat m_harrisResponses;				//harris corner responses
	Derivatives derivatives;			//derivative images struct
	Mat convertRgbToGrayscale();
	Derivatives computeDerivatives();	
	Mat computeHarrisResponses();
public:
    Harris(Mat img, float k, int filterRange,float percentage,int suppressionRadius,int radius);
	vector<pointData> getMaximaPoints();
	Mat MarkInImage();	
};
