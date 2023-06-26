#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>

/**
 * Creates two pipes, forks, and runns the given command.  One pipe is
 * connected between the given *out and the standard input stream of the child;
 * the other pipe is connected between the given *in and the standard output
 * stream of the child.
 *
 * Returns the pid of the child on success, -1 otherwise.  On error, errno
 * will be set accordingly.
 */
int bi_popen(const char* const command, FILE** const in, FILE** const out) // FILE** is a pointer to a pointer, which allows the function to modify the original pointer
{
    const int READ_END = 0;
    const int WRITE_END = 1;
    const int INVALID_FD = -1;

    int to_child[2] = { INVALID_FD, INVALID_FD }; // Integer Array with size 2. Creates a pipe to communicate from parent process to the child process
    int to_parent[2] = { INVALID_FD, INVALID_FD }; // Integer Array with size 2. Creates a pipe to communicate from child process to the parent process
    
    // Assign to NULL for error detection
    *in = NULL; //Assigns Null to the memory location pointed to by the pointer *in. Points to a FILE structure
    *out = NULL;
    
    /* Ensures correct inputs are provided
     * NOTE: *in = NULL assigned NULL to the memory location pointed to by the pointer *in.
     * This means that *in currently does not hold a valid FILE* pointer.
     * 
     * However, in == NULL checks if the pointer in itself is NULL. This checks if the in pointser itself points to a valid memory location. If in is NULL, it means that in itself does not hold a valid memory address and points to nowhere.
     * 
     * *in == NULL would check the memory location pointed to by in. This makes what IN points to as null.
     * in == NULL checks the pointer in itself (the value stored in in, which is the address of the memory location) This checks if in itself is null.
     * 
     */
    
    if (command == NULL || in == NULL || out == NULL) {
        errno = EINVAL;
        return 0;
    }

    
    // Create a pipe between the parent process and the child process
    // pipe() takes an array with to_child[0] as the reading from the pipe and to_child[1] for writing to the pipe
    // If pipe() < 0, then there was an error in creating the pipe
    
    if (pipe(to_child) < 0) {
        return 0;
    }

    // Create a pipe between the child process and the parent process

    if (pipe(to_parent) < 0) {
        return 0;
    }


    /** The fork() system call is used to create a new process by duplicating the existing process.
     * After the fork() call, two processes are created: the parent process and the child process.
     * The return value of fork() differs for each process:
     *  - In the parent process, the return value is the process ID (PID) of the newly created child process
     *  - In the child process, the return value is 0, indicating that it is the child process
     * 
     * The return value of fork() is stored as the pid variable of the type pid_t. 
     * It holds the PID of the child process in the parent process, or 0 in the child process
     * 
     * After a successful fork(), the parent process receives the process ID (PID) of the child process as the return vlaue of fork()
     * In this case, pid in the parent process will be greater than 0, the actual PID of the child process. 
     * The parent process can use this value to track and manage the child process, such as send signals or wait for termination.
     * 
     * After a successful fork(), the child process is created as a copy of the parent process, including code, data, file descriptors etc.
     * The chill process has a return value of fork() being 0, which allows the child process to differentiate itself form the parent process.
     * Hence, pid will be 0, indicating that it is the child process. 
     */
    const pid_t pid = fork();
    if (pid < 0) {
        return 0;
    }

    // Code for Child Process Only
    
    if (pid == 0) { 
        
        // dup2() duplicates file decriptor to_child[READ_END], which represents the read end of the pipe form parent to the child, to the standard input file descriptor ('STDIN_FILENO)
        // If < 0, indicates error in duplicating the file descriptor
        // Purpose of this is to redirect standard input of the child process to the read end of the pipe, allowing it to receive input from the parent process
        if (dup2(to_child[READ_END], STDIN_FILENO) < 0) {
            perror("dup2");
            exit(1);
        }
        
        // Necessary to close files after redirecting them to avoid resource leaks and ensure proper communication between the parent and child processes.
        close(to_child[READ_END]);
        close(to_child[WRITE_END]);

        // Purpose of this is to redirect standard output of the child process to the write end of the pipe, allowing it to send output from the parent process
        if (dup2(to_parent[WRITE_END], STDOUT_FILENO) < 0) {
            perror("dup2");
            exit(1);
        }
        
        // Necessary to close files after redirecting them to avoid resource leaks and ensure proper communication between the parent and child processes.
        close(to_parent[READ_END]);
        close(to_parent[WRITE_END]);

        /* Function used to replace the current process with a new process specified by the given command.
        * Command is "stockfish"
        * First argument is the path or name of the command to be executed, second argument is the command itself
        * NULL means end of argument list
        * If execlp is successful, the chilld process will be replaced by the Stockfish process, and the subsequent lines of code will not be executed
        */
        execlp(command, command, NULL);
        
        // If we are here, there was an error calling execlp
        perror("execlp");
        exit(1);
    }

    // Code for Parent Process Only, as if it was Child it would have run execlp()
    
    // Closes to_child[READ_END] in the parent process and set it to an invalid descriptor value.
    // This indicates that the parent process will not use the read end of the pipe from the parent to the child.
    
    close(to_child[READ_END]);
    to_child[READ_END] = INVALID_FD;

    // Closes to_parent[WRITE_END] in teh parent process, and set it to an invalid descriptor value.
    // This indicates that the parent process will not use the write end of the pipe from the child to the parent.
    
    close(to_parent[WRITE_END]);
    to_parent[WRITE_END] = INVALID_FD;

    
    // Resulting FILE* steam is assigned to pointer *in, which was an argument to bi_popen. 
    // This allows the parent process to read data from the child process through this stream.
    *in = fdopen(to_parent[READ_END], "r"); 
    
    //Checks if fdopen() worked
    if (*in == NULL) {
        return 0;
    }
    
    // to parent is no longer used as the steam is successfully associated with in
    to_parent[READ_END] = INVALID_FD;

    // Resulting FILE* steam is assigned to pointer *out, which was an argument to bi_popen. 
    // This allows the parent process to write data to the child process through this stream.
    *out = fdopen(to_child[WRITE_END], "w");
    if (*out == NULL) {
        return 0;
    }
    // to parent is no longer used as the steam is successfully associated with in

    to_child[WRITE_END] = INVALID_FD;
    
    // This configures the output steam *out to have no buffering (_IONBF)
    // No buffering means that data written to the steam will be immediately flushed and sent to the child process (stockfish) without beign stored in an intermediate buffer.
    setvbuf(*out, NULL, _IONBF, BUFSIZ);


    // Return the process ID of the child process to the parent process. This allows the parent process to track and manage the child process if necessary.
    return pid;

	// Would require review. This code is meant to be for covering errors, but is not reachable right now.
    const int old_errno = errno;

    if (*in != NULL) {
        fclose(*in);
    }

    if (*out != NULL) {
        fclose(*out);
    }

    for (int i = 0; i < 2; ++i) {
        if (to_child[i] != INVALID_FD) {
            close(to_child[i]);
        }
        if (to_parent[i] != INVALID_FD) {
            close(to_parent[i]);
        }
    }

    errno = old_errno;
    return -1;
}

std::string runStock(std::vector<std::string> moves)
{
    FILE* in = NULL; // Pointer to FILE Structure to read data from the pipe
    FILE* out = NULL; // Pointer to FILE Structure to write data to the pipe
    char* line = NULL; // Pointer to character array to store output from Stockfish 
    size_t size = 0;
    std::string previousLine;
    std::string outputMove;

    const int pid = bi_popen("stockfish", &in, &out); //And operator obtains memory address of variables in and out
    /* This allows the functon bi_popen to access and modify the original variable rather than working with a copy of it
     * Also, in and out are FILE structures, which is why they need to be modifyable.
     * If used just in (or out) with no &, we would send the value of in which is NULL.
     */
    
    if (pid < 0) {
        perror("bi_popen");
        return "Error";
    }
    
    std::string moveInput;
    for (const auto& move : moves) {
        moveInput += move + " ";
    }
    
    
    fprintf(out, "position startpos moves %s\n", moveInput.c_str());
    fprintf(out, "go depth 10 \n");
    
    
    
    for(int i = 1; i<20;i++) {
        
        std::string currentLine;
        		
		getline(&line, &size, in); // reads the output from the in file
        currentLine = line;
        
		if (strncmp(line, "bestmove", 8) == 0) {
            std::string str(currentLine);
            std::string strPrev(previousLine);
            
            std::string output = str.substr(9,4);
            outputMove = output;
            
            size_t pos = strPrev.find("cp");
            
            if (pos != std::string::npos) {
                outputMove = outputMove + " cp " + strPrev.substr(pos+3,3);
                 
            }
            
            // printf("%s\n", output.c_str());
            break;
        }
        
        previousLine = currentLine;
	}
    
    // Since in this case we can tell the child to terminate, we'll do so
    // and wait for it to terminate before we close down.
    fprintf(out, "quit\n");
    waitpid(pid, NULL, 0);

    fclose(in);
    fclose(out);

    return outputMove;
}
