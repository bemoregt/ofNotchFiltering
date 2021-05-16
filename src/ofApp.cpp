#include "ofApp.h"
using namespace ofxCv;

using namespace cv;
using namespace std;

// functions ------------------------------------------
void dft_shift(Mat& ds)
{
    int cx = ds.cols / 2;
    int cy = ds.rows / 2;
    Mat q0 = ds(cv::Rect(0, 0, cx, cy));
    Mat q1 = ds(cv::Rect(cx, 0, cx, cy));
    Mat q2 = ds(cv::Rect(0, cy, cx, cy));
    Mat q3 = ds(cv::Rect(cx, cy, cx, cy));
    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

void magnitude(Mat &input_img, Mat &magnitude_img)
{
    Mat planes[2];
    split(input_img, planes);
    magnitude(planes[0], planes[1], magnitude_img);
}

ofImage show_spectrum(Mat& img_spectrum, String title, bool inverse)
{
    Mat img;
    magnitude(img_spectrum, img);
    if (!inverse)
    {
        img += Scalar::all(1);
        log(img, img);
    }
    normalize(img, img, 0, 255, NORM_MINMAX);
    img.convertTo(img, CV_8UC1);
    
    ofImage of1;
    toOf(img, of1);
    //of1.update();
    return of1;
    
}

void notch_filter(Mat &spectrum, int x, int y, int value)
{
    Vec2d* pf = spectrum.ptr<Vec2d>(y);
    pf[x][0] = value;
}

//--------------------------------------------------------------
void ofApp::setup(){

    //Mat src_img = cv::imread("lena1.jpg", 0);
    filePath = "/Users/irix/Desktop/lena.jpg";
    before.load(filePath);
    cv::Mat src_img = toCv(before);
    
    if (!src_img.data) {
        return 0;
    }

    //imshow("src img", src_img);
    src_img.convertTo(src_img, CV_64FC1);
    Mat img_spectrum(src_img.size(), CV_64FC2);

    dft(src_img, img_spectrum, DFT_COMPLEX_OUTPUT, 0);
    dft_shift(img_spectrum);

    spec1= show_spectrum(img_spectrum, "src spectrum", false);
    spec1.update();
    
    notch_filter(img_spectrum, 277, 312, 0);
    notch_filter(img_spectrum, 263, 312, 0);
    notch_filter(img_spectrum, 249, 200, 0);
    notch_filter(img_spectrum, 235, 200, 0);

    spec2= show_spectrum(img_spectrum, "filtered spectrum", false);
    spec2.update();
    
    //========================
    Mat dtft_mat;
    dft(img_spectrum, dtft_mat, DFT_INVERSE);
    
    after= show_spectrum(dtft_mat, "filtered spectrum", false);
    after.update();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor(255,255,255);
    before.draw(0, 0, 512, 512);
    after.draw(512, 0, 512, 512);
    spec1.draw(0, 512, 512, 512);
    spec2.draw(512, 512, 512, 512);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
