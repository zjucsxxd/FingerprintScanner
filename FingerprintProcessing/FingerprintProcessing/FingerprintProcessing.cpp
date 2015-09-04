#include "FingerprintProcessing.h"

using namespace std;
using namespace cv;

Mat contoursImage(Mat image)
{
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(image, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	Mat drawing = Mat::zeros(image.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(255, 255, 255);
		drawContours(drawing, contours, i, color, -1, 8, hierarchy, 0, Point());
	}

	return drawing;
}

Mat closeContours(Mat image)
{
	Mat structuringElement = getStructuringElement(MORPH_CROSS, Size(2, 2));
	morphologyEx(image, image, MORPH_CLOSE, structuringElement);

	return image;
}

int main()
{
	Mat img = imread("test1.bmp", 0);

	// Creation of the mask
	Mat mask = Mat::zeros(img.rows, img.cols, img.type());
	Point polygon[1][11];
	polygon[0][0] = Point(17, 0);
	polygon[0][1] = Point(8, 14);
	polygon[0][2] = Point(4, 31);
	polygon[0][3] = Point(0, 42);
	polygon[0][4] = Point(0, 178);
	polygon[0][5] = Point(7, 200);
	polygon[0][6] = Point(13, 239);
	polygon[0][7] = Point(200, 239);
	polygon[0][8] = Point(215, 226);
	polygon[0][9] = Point(215, 7);
	polygon[0][10] = Point(207, 0);

	const Point* points[1] = { polygon[0] };
	int number_points[] = { 11 };

	fillPoly(mask, points, number_points, 1, Scalar(255, 255, 255), 8);

	// Canny edge detection
	Mat canny;
	Canny(img, canny, 50, 50 * 2, 3);

	// Merging
	Mat merged = canny & mask;
	imwrite("merged.bmp", merged);

	// Blurred image after canny and merging
	Mat blurred_normal;
	blur(merged, blurred_normal, Size(3, 3));
	normalize(blurred_normal, blurred_normal, 0, 1, NORM_MINMAX);
	imwrite("blurred_normal.bmp", blurred_normal * 10000);

	// Gaussian blurred image after canny and merging
	Mat gaussian_blurred, gaussian_blurred_normal, gaussian_blurred_hist;
	GaussianBlur(merged, gaussian_blurred, Size(3, 3), 0, 0, BORDER_DEFAULT);
	imwrite("gaussian_blurred.bmp", gaussian_blurred);
	normalize(gaussian_blurred, gaussian_blurred_normal, 0, 1, NORM_MINMAX);
	imwrite("gaussian_blurred_normal.bmp", gaussian_blurred_normal * 10000);
	equalizeHist(gaussian_blurred, gaussian_blurred_hist);
	imwrite("gaussian_blurred_hist.bmp", gaussian_blurred_hist);

	// Dilation after canny and merging
	Mat dilated;
	Mat element = getStructuringElement(MORPH_CROSS, Size(2, 2));
	dilate(merged, dilated, element);
	imwrite("dilated.bmp", dilated);

	// Closing of the contours and filling
	Mat closed = closeContours(merged);
	imwrite("closed.bmp", closed);
	Mat contoured_closed = contoursImage(closed);
	imwrite("contoured_closed.bmp", contoured_closed);

	// Gabor filter before merging and without canny
	Mat gabored;
	Scalar coeff = 1.5;
	double sigma = 10.0;
	double theta = 189.0;
	double lambda = 2.0;
	double gamma = 1.0;
	double psi = 8.0;
	Mat kernel = getGaborKernel(Size(2, 2), sigma, theta, lambda, gamma, psi, CV_32F);
	coeff = coeff*sum(kernel);
	divide(kernel, coeff, kernel);
	filter2D(img, gabored, CV_32F, kernel, Point(-1, -1), 0, BORDER_DEFAULT);
	threshold(gabored, gabored, 181, 255, 0);
	normalize(gabored, gabored, 0, 1, NORM_MINMAX);
	gabored.convertTo(gabored, mask.type());
	gabored = gabored & mask;
	imwrite("gabored.bmp", gabored * 10000);

	return(0);
}