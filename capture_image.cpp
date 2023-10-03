// Code to Capture an Image

#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {

	string filename = argv[1];

	system("fswebcam -r 1280x960 " + filename + ".jpg"); //Captures image and saves to file
	// cout << "Image Captured" << endl;
	return 0;
}
	
