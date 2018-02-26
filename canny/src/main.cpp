
#include "canny.h"


int main(int argc, const char * argv[])
{    
    Mat input;
    input = imread(argv[1], 0);
    int upper = atoi(argv[2]);
    int lower = atoi(argv[3]);
    canny image1(input,upper,lower);
    image1.show_edges();
    waitKey();
    Mat output=image1.get_edges();
    imshow("edges",output);
    waitKey();
    return 0;
}