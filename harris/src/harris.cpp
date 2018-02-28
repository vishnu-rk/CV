#include "harris.h"

Harris::Harris(Mat img, float k, int filterRange,float percentage, int suppressionRadius, int radius):img(img),k(k),filterRange(filterRange),percentage(percentage),suppressionRadius(suppressionRadius),radius(radius) {
    //Convert to greyscalescale image
    this->greyscaleImg = this->convertRgbToGrayscale();
    //Compute Derivatives
    this->derivatives = computeDerivatives();
    //Compute Harris Responses
    this->m_harrisResponses = computeHarrisResponses();
    this->points=this->getMaximaPoints();
}

vector<pointData> Harris::getMaximaPoints() {
    // Declare a max suppression matrix
    Mat maximaSuppressionMat(m_harrisResponses.rows, m_harrisResponses.cols, CV_32F, Scalar::all(0));
    // Create a vector of all Points
    std::vector<pointData> points;
    for (int r = 0; r < m_harrisResponses.rows; r++) {
        for (int c = 0; c < m_harrisResponses.cols; c++) {
            Point p(r,c); 
            pointData d;
            d.cornerResponse = m_harrisResponses.at<float>(r,c);
            d.point = p;
            points.push_back(d);
        }
    }
    //Sort points by corner Response
    sort(points.begin(), points.end(), by_cornerResponse());
    //Get top points
    int numberTopPoints = m_harrisResponses.cols * m_harrisResponses.rows * percentage;
    std::vector<pointData> topPoints;
    int i=0;
    while(topPoints.size() < numberTopPoints) {
        if(i == points.size())
            break;
        int supRows = maximaSuppressionMat.rows;
        int supCols = maximaSuppressionMat.cols;
        //Check if point marked in maximaSuppression matrix
        if(maximaSuppressionMat.at<int>(points[i].point.x,points[i].point.y) == 0) {
            for (int r = -suppressionRadius; r <= suppressionRadius; r++) {
                for (int c = -suppressionRadius; c <= suppressionRadius; c++) {
                    int sx = points[i].point.x+c;
                    int sy = points[i].point.y+r;
                    //Check bounds
                    if(sx > supRows)
                        sx = supRows;
                    if(sx < 0)
                        sx = 0;
                    if(sy > supCols)
                        sy = supCols;
                    if(sy < 0)
                        sy = 0;
                    maximaSuppressionMat.at<int>(sx, sy) = 1;
                }
            }
            topPoints.push_back(points[i]);
        }
        i++;
    }
    return topPoints;
}


Mat Harris::convertRgbToGrayscale() {
    Mat greyscaleImg;
    cvtColor(img, greyscaleImg, CV_BGR2GRAY);
    return greyscaleImg;
}



Derivatives Harris::computeDerivatives() {
    //Using Sobel for derivaties
    Mat Ix(greyscaleImg.rows-2, greyscaleImg.cols-2, CV_32F);
    Mat Iy(greyscaleImg.rows-2, greyscaleImg.cols-2, CV_32F);
    Mat Ixy(greyscaleImg.rows-2, greyscaleImg.cols-2, CV_32F);
    Sobel( greyscaleImg, Ix, CV_32F, 1, 0, 3, 1, 0, BORDER_DEFAULT );
    Sobel( greyscaleImg, Iy, CV_32F, 0, 1, 3, 1, 0, BORDER_DEFAULT );
    Ixy = Ix.mul( Iy);
    Derivatives d;
    d.Ix = Ix;
    d.Iy = Iy;
    d.Ixy = Iy;
    return d;
}


Mat Harris::computeHarrisResponses() {
    Derivatives d=this->derivatives;
    Mat M(d.Iy.rows, d.Ix.cols, CV_32F);
    for(int r=0; r<d.Iy.rows; r++) {  
        for(int c=0; c<d.Iy.cols; c++) {
            float   a11, a12, a21, a22;
            a11 = d.Ix.at<float>(r,c) * d.Ix.at<float>(r,c);
            a22 = d.Iy.at<float>(r,c) * d.Iy.at<float>(r,c);
            a21 = d.Ix.at<float>(r,c) * d.Iy.at<float>(r,c);
            a12 = d.Ix.at<float>(r,c) * d.Iy.at<float>(r,c);
            float det = a11*a22 - a12*a21;
            float trace = a11 + a22;
            M.at<float>(r,c) = abs(det - k * trace*trace);
        }
    }
    return M;
}


Mat Harris::MarkInImage() {
    Mat retImg;
    img.copyTo(retImg);
    for(vector<pointData>::iterator it = points.begin(); it != points.end(); ++it) {
        Point center = (*it).point;
        //down
        for(int r=-radius; r<radius; r++) {
            retImg.at<Vec3b>(Point(center.y+r,center.x+radius)) = Vec3b(0, 0, 255);
        }
        //up
        for(int r=-radius; r<radius; r++) {
            retImg.at<Vec3b>(Point(center.y+r,center.x-radius)) = Vec3b(0, 0, 255);
        }
        //left
        for(int c=-radius; c<radius; c++) {
            retImg.at<Vec3b>(Point(center.y-radius,center.x+c)) = Vec3b(0, 0, 255);
        }
        //right
        for(int c=-radius; c<radius; c++) {
            retImg.at<Vec3b>(Point(center.y+radius,center.x+c)) = Vec3b(0, 0, 255);
        }
        retImg.at<Vec3b>(Point(center.y,center.x)) = Vec3b(0, 255, 0);
    }
    return retImg;
}