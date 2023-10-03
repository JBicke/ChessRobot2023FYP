// Code to Capture an Image

#include <cstdlib>
#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[]) {
	
	std::string message;

	std::getline(std::cin, message);

	string filename = message;
	
	string folderPath = "Pictures/";
	chdir(folderPath.c_str());
	
	string command = "fswebcam -r 1280x960 " + filename + ".jpg";

	system(command.c_str()); //Captures image and saves to file
	// cout << "Image Captured" << endl;
	return 0;
}
	
