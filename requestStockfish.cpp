#include <iostream>
#include <cstdio>
#include <cstring>
#include <stdio.h>

using namespace std;

int main() {
    FILE* pipe_in = popen("/usr/games/stockfish", "w");
    if (!pipe_in) {
        cout << "Failed to open stockfish process\n";
        return 1;
    }
    
    FILE* pipe_out = popen("/usr/games/stockfish", "r");
    if (!pipe_out) {
        cout << "Failed to open stockfish process for reading\n";
        return 1;
    }
    
    fprintf(pipe_in, "position startpos makemoves e2e4 e7e5\n");
    cout << "Step1" << endl;

    fprintf(pipe_in, "go depth 10\n");
    //fprintf(pipe_in, "quit\n");
    cout << "Step2" << endl;

    fflush(pipe_in);
    cout << "Step3" << endl;

    char buffer[4096];
    cout << "Step4" << endl;
    
    
    while(fgets(buffer, 4096, pipe_out)) {
        cout << "Received from Stockfish: " << buffer << endl;
        if (strncmp(buffer, "bestmove", 8) == 0) {
            cout << "Stockfish response: " << buffer << endl;
            break;
        }
    }
    cout << "Step5" << endl;

    cout << "It Worked" << endl;
    fprintf(pipe_in, "quit\n"); // send the "quit" command to Stockfish
    //fprintf(pipe_out, "quit\n");
    pclose(pipe_in);
    
    pclose(pipe_out);
    return 0;
}

//int main() {
    //FILE* pipe = popen("/usr/games/stockfish", "w");
    //if (!pipe) {
        //cerr << "Failed to open Stockfish process" << endl;
        //return 1;
    //}
    
    
    
    //// Send the "position" and "go" commands to Stockfish
    //fprintf(pipe, "position startpos moves e2e4 e7e5\n");
    //fprintf(pipe, "go depth 15\n");
    //fflush(pipe);
    //// Read the engine output until it sends the "bestmove" command
    //char buffer[256];
    //while (fgets(buffer, 256, pipe)) {
        //if (strncmp(buffer, "bestmove", 8) == 0) {
            //cout << "Stockfish response: " << buffer << endl;
            //break;
        //}
    //}
    //pclose(pipe);
    //return 0;
//}
