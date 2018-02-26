#include "canny.h"

void canny::followEdges() {    
    edges.at<float>(y, x) = 255; 
    //for  all neighbouring  pixels   
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            //checking bounding conditions
            if((i != 0) && (j != 0) && (x + i >= 0) && (y + j >= 0) && (x + i <= magnitude.cols) && (y + j <= magnitude.rows)){ 
                //check if magnitude of neighbouring pixel  is above lower  threshold.               
                if((magnitude.at<float>(y + j, x + i) > lower) && (edges.at<float>(y + j, x + i) != 255)) {
                    x=x+i;
                    y=y+j;
                    followEdges();
                }
            }
        }
    }
}

void canny::hysteresis() {    
    int rows = magnitude.rows;
    int cols = magnitude.cols;    
    edges = Mat(magnitude.size(), CV_32F, 0.0);
    //for all pixels    
    for (x = 0; x < cols; x++) {
        for (y = 0; y < rows; y++) {
            //check if  magnitude  of pixel  is greater than upper threshold
            if (magnitude.at<float>(y, x) >= upper){
                followEdges();
            }
        }
    }
    
}

void canny::nonMaximumSuppression() {    
    Mat checkImage = Mat(magnitude.rows, magnitude.cols, CV_8U);    
    MatIterator_<float>itMag = magnitude.begin<float>();
    MatIterator_<float>itDirection = slopes.begin<float>();    
    MatIterator_<unsigned char>itRet = checkImage.begin<unsigned char>();    
    MatIterator_<float>itEnd = magnitude.end<float>();    
    for (; itMag != itEnd; ++itDirection, ++itRet, ++itMag) {
        const Point pos = itRet.pos();        
        float currentDirection = atan(*itDirection) * (180 / 3.142);        
        while(currentDirection < 0) currentDirection += 180;        
        *itDirection = currentDirection;        
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
    GaussianBlur(src, image, Size(3, 3), 1.5);
    Mat magX = Mat(src.rows, src.cols, CV_32F);
    Mat magY = Mat(src.rows, src.cols, CV_32F);
    //find image gradients  along horizontal and  vertical  directions
    Sobel(image, magX, CV_32F, 1, 0, 3);
    Sobel(image, magY, CV_32F, 0, 1, 3);
    //find slope image    
    slopes = Mat(image.rows, image.cols, CV_32F); 
    divide(magY, magX, slopes);
    Mat sum = Mat(image.rows, image.cols, CV_64F);
    Mat prodX = Mat(image.rows, image.cols, CV_64F);
    Mat prodY = Mat(image.rows, image.cols, CV_64F);
    //find  magnitude image
    multiply(magX, magX, prodX);
    multiply(magY, magY, prodY);
    sum = prodX + prodY;
    sqrt(sum, sum);    
    magnitude = sum.clone(); 
    // apply non-maxima supression   
    nonMaximumSuppression(); 
    //hysteresis   
    hysteresis();   
}

void canny::show_edges(){
    imshow("edges", edges);
}

Mat canny::get_edges(){
    return edges;
}
