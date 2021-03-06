/**
 * @file rectangle.cpp
 * @brief mex interface for cv::rectangle
 * @ingroup imgproc
 * @author Kota Yamaguchi
 * @date 2012
 */
#include "mexopencv.hpp"
using namespace std;
using namespace cv;

/**
 * Main entry called from Matlab
 * @param nlhs number of left-hand-side arguments
 * @param plhs pointers to mxArrays in the left-hand-side
 * @param nrhs number of right-hand-side arguments
 * @param prhs pointers to mxArrays in the right-hand-side
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // Check the number of arguments
    nargchk(nrhs>=2 && nlhs<=1);

    // Argument vector
    vector<MxArray> rhs(prhs, prhs+nrhs);

    // cv::rectangle has two overloaded variants
    bool rect_variant = (rhs[1].numel() == 4);
    nargchk(rect_variant ? ((nrhs%2)==0) : (nrhs>=3 && (nrhs%2)==1));

    // Option processing
    Scalar color;
    int thickness = 1;
    int lineType = cv::LINE_8;
    int shift = 0;
    for (int i=(rect_variant ? 2 : 3); i<nrhs; i+=2) {
        string key(rhs[i].toString());
        if (key=="Color")
            color = rhs[i+1].toScalar();
        else if (key=="Thickness")
            thickness = (rhs[i+1].isChar()) ?
                ThicknessType[rhs[i+1].toString()] : rhs[i+1].toInt();
        else if (key=="LineType")
            lineType = (rhs[i+1].isChar()) ?
                LineType[rhs[i+1].toString()] : rhs[i+1].toInt();
        else if (key=="Shift")
            shift = rhs[i+1].toInt();
        else
            mexErrMsgIdAndTxt("mexopencv:error",
                "Unrecognized option %s", key.c_str());
    }

    // Process
    Mat img(rhs[0].toMat());
    if (!rect_variant) {
        Point pt1(rhs[1].toPoint()), pt2(rhs[2].toPoint());
        rectangle(img, pt1, pt2, color, thickness, lineType, shift);
    }
    else {
        Rect r(rhs[1].toRect());
        rectangle(img, r, color, thickness, lineType, shift);
    }
    plhs[0] = MxArray(img);
}
