
#include <stdio.h>
#include <iostream>

int main() {
    FILE *pipe_out;
    char buffer[4096];
    
    // Open a pipe to execute the "ls" command and read the output
    pipe_out = popen("ls", "r");
    
    // Read each line of output and print it
    while (fgets(buffer, 4096, pipe_out)) {
        std::cout << buffer << "Lmao" << std::endl;
    }
    
    // Close the pipe and exit
    pclose(pipe_out);
    return 0;
}
