//Derrick Chuong
//SURF staands for speed up robust features that is used for object recognization.
//It is faster than SIFT (SIFT uses hash table)
#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
//Namepaces
using namespace cv;
using namespace std;


int main()
{
	//timing variable
	double myTime; 
	//load the image as greyscale - 1 matrix compare to many
	Mat object = imread("C:/images/photo9.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!object.data){
		cout << "Can't open image";
		return -1;
	}
	//create the window
	namedWindow("Obj Recognization", CV_WINDOW_AUTOSIZE);
	//setting up variables for matching objects
	vector<KeyPoint> keyPointObject, keyPointImage;
	Mat descriptorObject;
	//SURF Detector, and descriptor, matching objects
	//Hessian is the determinant of the matrix - higher the value = the fewer the keypoints ( less noise )
	int minHessian = 2000;
	SurfFeatureDetector detector(minHessian);
	detector.detect(object, keyPointObject);
	//Extractor is needed to compute the required calculation and detect matches
	SurfDescriptorExtractor extractor;
	extractor.compute(object, keyPointObject, descriptorObject);
	// one of the descriptor matcher
	FlannBasedMatcher matcher;
	//Initialize video and display window
	VideoCapture capatureMyCamera(0); //camera 0 is my webcam
	if (!capatureMyCamera.isOpened()) {
		return -1;
	}
	//Object corner points for plotting box
	//Opencv coordinate system is base on columns and then rows so (y, x)
	vector<Point2f> object_corners(4);
	object_corners[0] = cvPoint(0, 0);
	object_corners[1] = cvPoint(object.cols, 0);
	object_corners[2] = cvPoint(object.cols, object.rows);
	object_corners[3] = cvPoint(0, object.rows);
	//startng the video loop
	char escapeKey = 'q';
	double frameCount = 0;
	// the higher the value the more drawing of the keypoint matching will draw
	float thresholdMatchingValue = 0.8;
	// was thinking to take screenshot of several goodmatches
	unsigned int thresholdGoodMatches = 5;
	unsigned int thresholdGoodMatchesV[] = { 5, 6, 7, 8, 9, 10, 11 };
	for (int threshold = 0; threshold < 7; threshold++){
		thresholdGoodMatches = thresholdGoodMatchesV[threshold];
		//thresholdGoodMatches=8;
		cout << thresholdGoodMatches << endl;
		myTime = (double)getTickCount();
		while (escapeKey != 's')
		{
			frameCount++;
			Mat frame;
			Mat image;
			capatureMyCamera >> frame;
			// convert to grayscale
			cvtColor(frame, image, CV_RGB2GRAY);
			Mat descriptor_image, imgage_matches, homography;
			vector<KeyPoint> keyPoint_image;
			//Dmatch tells which one is more similar -> match the 2 nearest feature
			vector<vector<DMatch>> matches;
			vector<DMatch> good_matches;
			vector<Point2f> obj;
			vector<Point2f> scene;
			vector<Point2f> scene_corners(4);
			//detect the keypoints then compute math for both images
			detector.detect(image, keyPoint_image);
			extractor.compute(image, keyPoint_image, descriptor_image);
			// find the k best matches for each descriptor
			matcher.knnMatch(descriptorObject, descriptor_image, matches, 2);

			// for each good descriptor, find the good matches and push it into a vector
			for (int i = 0; i < min(descriptor_image.rows - 1, (int)matches.size()); i++) 
			{
				if ((matches[i][0].distance < thresholdMatchingValue*(matches[i][1].distance)) && ((int)matches[i].size() <= 2 && (int)matches[i].size()>0))
				{
					good_matches.push_back(matches[i][0]);
				}
			}
	
			//draw only "good" matches
			//parameters (img1, kp1, img2, kp2, matches1to2, outimg, white color line, white color kp, circles do no match, mataches, flags
			drawMatches(object, keyPointObject, image, keyPoint_image, good_matches, imgage_matches, Scalar::all(255), Scalar::all(255), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
			if (good_matches.size() >= thresholdGoodMatches)
			{
				// display that the object is found
				putText(imgage_matches, "Object Found", cvPoint(10, 50), FONT_HERSHEY_COMPLEX_SMALL, 2, cvScalar(0, 0, 250), 1, CV_AA);
				for (int i = 0; i < good_matches.size(); i++)
				{
					//get the keypoints from the good matches
					// query image = left image, training image = right image
					obj.push_back(keyPointObject[good_matches[i].queryIdx].pt);
					scene.push_back(keyPoint_image[good_matches[i].trainIdx].pt);
				}
				//perspective transformation between the two planes
				homography = findHomography(obj, scene, CV_RANSAC);
				perspectiveTransform(object_corners, scene_corners, homography);
				//Draw lines between the corners (the mapped object in the scene image )
				line(imgage_matches, scene_corners[0] + Point2f(object.cols, 0), scene_corners[1] + Point2f(object.cols, 0), Scalar(0, 255, 0), 4);
				line(imgage_matches, scene_corners[1] + Point2f(object.cols, 0), scene_corners[2] + Point2f(object.cols, 0), Scalar(0, 255, 0), 4);
				line(imgage_matches, scene_corners[2] + Point2f(object.cols, 0), scene_corners[3] + Point2f(object.cols, 0), Scalar(0, 255, 0), 4);
				line(imgage_matches, scene_corners[3] + Point2f(object.cols, 0), scene_corners[0] + Point2f(object.cols, 0), Scalar(0, 255, 0), 4);
			}
			else
			{
				putText(imgage_matches, "", cvPoint(10, 25), FONT_HERSHEY_COMPLEX_SMALL, 3, cvScalar(0, 0, 250), 1, CV_AA);
			}
			//Show detected matches	
			imshow("Obj Recognization", imgage_matches);

			// to stop and take a "Screen Shot" change escapekey != s to = s;
			escapeKey = cvWaitKey(10);
			if (frameCount>15)
				escapeKey != 's';
		}
		//average frames per second			
		myTime = ((double)getTickCount() - myTime) / getTickFrequency();
		cout << myTime << " " << frameCount / myTime << endl;
		cvWaitKey(0);
		
		frameCount = 0;
		escapeKey = 'a';
	}
	//Release camera and exit
	capatureMyCamera.release();
	return 0;
}