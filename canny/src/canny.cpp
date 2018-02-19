#include "canny.h"

void canny::followEdges() {
    
    edges.at<float>(y, x) = 255;
    
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            //make sure we don't go out of bounds and check that we do not look at the same pixel twice
            if((i != 0) && (j != 0) && (x + i >= 0) && (y + j >= 0) && (x + i <= magnitude.cols) && (y + j <= magnitude.rows)){
                
                if((magnitude.at<float>(y + j, x + i) > lower) && (edges.at<float>(y + j, x + i) != 255)) {
                    x=x+i;
                    y=y+j;
                    followEdges();
                }
            }
        }
    }
}

void canny::edgeDetect() {
    
    int rows = magnitude.rows;
    int cols = magnitude.cols;
    
    edges = Mat(magnitude.size(), CV_32F, 0.0);
    
    for (x = 0; x < cols; x++) {
        for (y = 0; y < rows; y++) {
            if (magnitude.at<float>(y, x) >= upper){
                followEdges();
            }
        }
    }
    
}

void canny::nonMaximumSuppression() {
    
    //used to mirror positions in magnitude Mat
    Mat checkImage = Mat(magnitude.rows, magnitude.cols, CV_8U);
    
    //using MatIterator to iterate through the Mats
    MatIterator_<float>itMag = magnitude.begin<float>();
    MatIterator_<float>itDirection = slopes.begin<float>();
    
    MatIterator_<unsigned char>itRet = checkImage.begin<unsigned char>();
    
    MatIterator_<float>itEnd = magnitude.end<float>();
    
    //Calculates the direction in this loop
    for (; itMag != itEnd; ++itDirection, ++itRet, ++itMag) {
        const Point pos = itRet.pos();
        
        float currentDirection = atan(*itDirection) * (180 / 3.142);
        
        while(currentDirection < 0) currentDirection += 180;
        
        *itDirection = currentDirection;
        
        //checks to make sure we don't go out of bounds
        if (currentDirection > 22.5 && currentDirection <= 67.5) {
            if(pos.y > 0 && pos.x > 0 && *itMag <= magnitude.at<float>(pos.y - 1, pos.x - 1)) {
                magnitude.at<float>(pos.y, pos.x) = 0;
            }
            if(pos.y < magnitude.rows-1 && pos.x < magnitude.cols-1 && *itMag <= magnitude.at<float>(pos.y + 1, pos.x + 1)) {
                magnitude.at<float>(pos.y, pos.x) = 0;
            }
        }
        else if(currentDirection > 67.5 && currentDirection <= 112.5) {
            if(pos.y > 0 && *itMag <= magnitude.at<float>(pos.y-1, pos.x)) {
                magnitude.at<float>(pos.y, pos.x) = 0;
            }
            if(pos.y<magnitude.rows-1 && *itMag<=magnitude.at<float>(pos.y+1, pos.x)) {
                magnitude.at<float>(pos.y, pos.x) = 0;
            }
        
        }
        else if(currentDirection > 112.5 && currentDirection <= 157.5) {
            if(pos.y>0 && pos.x<magnitude.cols-1 && *itMag<=magnitude.at<float>(pos.y-1, pos.x+1)) {
                magnitude.at<float>(pos.y, pos.x) = 0;;
            }
            if(pos.y < magnitude.rows-1 && pos.x>0 && *itMag<=magnitude.at<float>(pos.y+1, pos.x-1)) {
                magnitude.at<float>(pos.y, pos.x) = 0;
            }
        }
        else {
            if(pos.x > 0 && *itMag <= magnitude.at<float>(pos.y, pos.x-1)) {
                magnitude.at<float>(pos.y, pos.x) = 0;
            }
            if(pos.x < magnitude.cols-1 && *itMag <= magnitude.at<float>(pos.y, pos.x+1)) {
                magnitude.at<float>(pos.y, pos.x) = 0;
            }
        }        
        
    }   
    
}


void canny::MyCanny() {

    image = src.clone();

    //Noise reduction
    GaussianBlur(src, image, Size(3, 3), 1.5);

    //calculate magnitudes and direction using Sobel
    Mat magX = Mat(src.rows, src.cols, CV_32F);
    Mat magY = Mat(src.rows, src.cols, CV_32F);
    Sobel(image, magX, CV_32F, 1, 0, 3);
    Sobel(image, magY, CV_32F, 0, 1, 3);
    
    //calculate slope
    slopes = Mat(image.rows, image.cols, CV_32F); 
    divide(magY, magX, slopes);

    //caclulate magnitude of the gradient at each pixel
    Mat sum = Mat(image.rows, image.cols, CV_64F);
    Mat prodX = Mat(image.rows, image.cols, CV_64F);
    Mat prodY = Mat(image.rows, image.cols, CV_64F);
    multiply(magX, magX, prodX);
    multiply(magY, magY, prodY);
    sum = prodX + prodY;
    sqrt(sum, sum);    
    magnitude = sum.clone();
    
    //Non Maximum Suppression
    nonMaximumSuppression();    
    //Hysterisis
    edgeDetect();   

}

void canny::show_edges(){
    imshow("edges", edges);
}

Mat canny::get_edges(){
    return edges;
}
