
//	Elena Rangelov
//	P03
//	17.3.2022

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

string ifilename;

int main(int argc, char* argv[]) {

    ofstream results;
    results.open("results.txt", ios::out | ios::trunc | ios::binary);

    int lower = 110;
    int upper = 170;

    int low = 0;
    int high = 0;
    int d = 0;
    int p = 0;
    int n = 0;
    int q = 0;
    int sd = 0;
    int min_dist = 40;
    int max_radius = 125;

    if (argc > 0) {

        int i = 1;
        while (i < argc) {

            string temp;
            string t = argv[i];

            if (t == "-U") {
                i++;
                temp += argv[i];
                low = stoi(temp);
            }
            else if (t == "-A") {
                i++;
                temp += argv[i];
                high = stoi(temp);
            }
            else if (t == "-P") {
                i++;
                temp += argv[i];
                p = stoi(temp);
            }
            else if (t == "-N") {
                i++;
                temp += argv[i];
                n = stoi(temp);
            }
            else if (t == "-Q") {
                i++;
                temp += argv[i];
                q = stoi(temp);
            }
            else if (t == "-D") {
                i++;
                temp += argv[i];
                d = stoi(temp);
            }
            else if (t == "-SD") {
                i++;
                temp += argv[i];
                sd = stoi(temp);
            }
            else if (t == "-MINDIST") {
                i++;
                temp += argv[i];
                min_dist = stoi(temp);
            }
            else if (t == "-MAXRADIUS") {
                i++;
                temp += argv[i];
                max_radius = stoi(temp);
            }
            else if (t == "-F") {
                i++;
                temp += argv[i];
                ifilename = temp;
            }
            i++;

        }
    }

    Mat img;
    img = imread(ifilename, 1);

    if (!img.data)
    {
        printf("No image data \n");
        return -1;
    }
    
    //	grayscale
    
    imwrite("./copy.jpg", img);

    Mat gray;

    gray = imread(ifilename, IMREAD_GRAYSCALE);
    imwrite("./gray.jpg", gray);

    Mat blur;
    medianBlur(gray, blur, 5);

    Mat canny;
    Canny(blur, canny, lower, upper);
    imwrite("./imagef.jpg", canny);

    vector<Vec3f> circles;
               //                               
               //image circles     method      dp      

    HoughCircles(blur, circles, HOUGH_GRADIENT, 1, min_dist, low, high, 75, max_radius);

    int pennies = 0;
    int nickels = 0;
    int dimes = 0;
    int quarters = 0;
    int dollars = 0;


    for (int i = 0; i < circles.size(); i++) {

        int x = cvRound(circles[i][0]);
        int y = cvRound(circles[i][1]);
        int r = cvRound(circles[i][2]);

        Point center(x, y);
        Scalar color;


        if (r <= d) {
            color = Scalar(255, 0, 0);
            dimes++;
        }
        else if (r <= p) {
            color = Scalar(0, 0, 255);
            pennies++;
        }
        else if (r <= n) {
            color = Scalar(255, 255, 0);
            nickels++;
        }
        else if (r <= q) {
            color = Scalar(0, 255, 0);
            quarters++;
        }
        else if(r <= sd) {
            color = Scalar(0, 255, 255);
            dollars++;
        }



        circle(img, center, r, color, 4);

    }

    imwrite("./circles.jpg", img);

    results << "pennies: " << pennies << endl;
    results << "nickels: " << nickels << endl;
    results << "dimes: " << dimes << endl;
    results << "quarters: " << quarters << endl;
    results << "dollars: " << dollars << endl;
    results << endl;

    double total = pennies * 0.01 + nickels * 0.05 + dimes * 0.1 + quarters * .25 + dollars;

    results << "total: $" << total << endl;

    // iterate through circles.size
    // circle[i] -- 0 x, 1 y, 2 r
    // center point
    // average color
    // circle(img, center, radius, Scalar(0, 0, 255, 4, 8, 0);
    // imwrite("./circles.jpg", img)

    


}
