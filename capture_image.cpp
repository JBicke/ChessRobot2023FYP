// Code to Capture an Image

#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {

	string filename = argv[1];
	string command = "fswebcam -r 1280x960 " + filename + ".jpg";

	system(command.c_str()); //Captures image and saves to file
	// cout << "Image Captured" << endl;
	return 0;
}
	
