#include "harris.h"
int main(int argc, char** argv) {
    float k = 0.25;                         //multiplication factor in cornerness function
    int boxFilterSize = 3;                  //filter size
    int maximaSuppressionDimension = 10;    //supression radius
    int markDimension = 5;                  //size of box around key point
    float percentage=0.00041;               //percentage of best keypoints to be detected
    //read image from file
    Mat img;
    if (argc == 1) {
        cout << "No image provided!"<< endl;
        return 0;
    } else {
        img = imread(argv[1]);
    }
    namedWindow("HarrisCornerDetector", 1);
    //call harris object
    Harris harris(img, k, boxFilterSize,percentage,maximaSuppressionDimension,markDimension);
    //mark harris corners on image
    Mat _img=harris.MarkInImage();
    //display image
    imshow("HarrisCornerDetector", _img);
    waitKey(0);
    return 0;
}
