#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

struct pointData { 
    float cornerResponse;
    Point point;
};

struct by_cornerResponse { 
    bool operator()(pointData const &left, pointData const &right) { 
        return left.cornerResponse > right.cornerResponse;
    }
};

struct Derivatives {
	Mat Ix;
	Mat Iy;
	Mat Ixy;
};

class Harris {
public:
    Harris(Mat img, float k, int filterRange);
	vector<pointData> getMaximaPoints(float percentage, int filterRange, int suppressionRadius);
	Mat MarkInImage(Mat& img,vector<pointData> points, int radius);

private:
	Mat convertRgbToGrayscale(Mat& img);
	Derivatives computeDerivatives(Mat& greyscaleImg);	
	Mat computeHarrisResponses(float k, Derivatives& intMats);
	Mat m_harrisResponses;
};
