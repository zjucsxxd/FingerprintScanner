#include <opencv2/highgui/highgui.hpp> // allow to read and write files
#include <opencv2/imgproc/imgproc.hpp> // load the main image processing functions

using namespace cv;

Mat contoursImage(Mat image); // draw and fill the contours
Mat closeContours(Mat image); // close the contours to better fill them