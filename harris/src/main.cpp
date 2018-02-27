#include "harris.h"

float k = 0.25;
int boxFilterSize = 3;
int maximaSuppressionDimension = 10;
int markDimension = 5;
float percentage=0.00041;
Mat m_img;

void doHarris(Mat m_img) {
    Harris harris(m_img, k, boxFilterSize);
    vector<pointData> resPts = harris.getMaximaPoints(percentage, boxFilterSize, maximaSuppressionDimension);
    Mat _img = harris.MarkInImage(m_img,resPts, markDimension);
    imshow("HarrisCornerDetector", _img);
}

int main(int argc, char** argv) {
    // read image from file + error handling
    Mat img;
    if (argc == 1) {
        cout << "No image provided! Usage: ./Ex1 [path to image]" << endl << "Using default image: haus.jpg" << endl;
        img = imread("haus.jpg");
    } else {
        img = imread(argv[1]);
    }
    img.copyTo(m_img);
    namedWindow("HarrisCornerDetector", 1);
    doHarris(m_img);
    waitKey(0);
    return 0;
}
