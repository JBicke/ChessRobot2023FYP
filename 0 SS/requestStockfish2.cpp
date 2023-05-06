#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    int infd[2], outfd[2];
    if (pipe(infd) == -1 || pipe(outfd) == -1) {
        std::cerr << "Failed to create pipes" << std::endl;
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Failed to fork" << std::endl;
        return 1;
    } else if (pid == 0) {
        // Child process: redirect stdin and stdout to the pipes
        dup2(infd[0], STDIN_FILENO);
        dup2(outfd[1], STDOUT_FILENO);
        close(infd[1]);
        close(outfd[0]);

        // Launch Stockfish
        execlp("/usr/games/stockfish", "stockfish", NULL);
        std::cerr << "Failed to launch Stockfish" << std::endl;
        return 1;
    }

    // Parent process: close the unused ends of the pipes
    close(infd[0]);
    close(outfd[1]);

    // Send commands to Stockfish
    std::string command = "position startpos moves e2e4 e7e5\n";
    write(infd[1], command.c_str(), command.length());
    command = "go depth 10\n";
    write(infd[1], command.c_str(), command.length());

    // Read Stockfish's output
    const int buf_size = 1024;
    char buf[buf_size];
    std::string output;
    while (true) {
        int n = read(outfd[0], buf, buf_size);
        if (n == -1) {
            std::cerr << "Failed to read from pipe" << std::endl;
            break;
        } else if (n == 0) {
            break;
        } else {
            output.append(buf, n);
        }
    }

    // Extract the best move from Stockfish's output
    std::string::size_type bestmove_pos = output.find("bestmove");
    if (bestmove_pos == std::string::npos) {
        std::cerr << "Failed to find best move in output" << std::endl;
    } else {
        std::string::size_type end_pos = output.find('\n', bestmove_pos);
        if (end_pos == std::string::npos) {
            end_pos = output.length() - 1;
        }
        std::string bestmove = output.substr(bestmove_pos + 9, end_pos - (bestmove_pos + 9));
        std::cout << "Best move: " << bestmove << std::endl;
    }

    // Close the pipes and wait for Stockfish to exit
    close(infd[1]);
    close(outfd[0]);
    int status;
    waitpid(pid, &status, 0);

    return 0;
}
