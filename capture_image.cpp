// Code to Capture an Image

#include <cstdlib>
#include <iostream>

using namespace std;

int main() {
	system("fswebcam -r 1280x960 1001.jpg"); //Captures image and saves to file
	cout << "Image Captured" << endl;
	return 0;
}
	
