#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * Creates two pipes, forks, and runns the given command.  One pipe is
 * connected between the given *out and the standard input stream of the child;
 * the other pipe is connected between the given *in and the standard output
 * stream of the child.
 *
 * Returns the pid of the child on success, -1 otherwise.  On error, errno
 * will be set accordingly.
 */
int bi_popen(const char* const command, FILE** const in, FILE** const out)
{
    const int READ_END = 0;
    const int WRITE_END = 1;
    const int INVALID_FD = -1;

    int to_child[2] = { INVALID_FD, INVALID_FD };
    int to_parent[2] = { INVALID_FD, INVALID_FD };

    *in = NULL;
    *out = NULL;

    if (command == NULL || in == NULL || out == NULL) {
        errno = EINVAL;
        goto bail;
    }

    if (pipe(to_child) < 0) {
        goto bail;
    }

    if (pipe(to_parent) < 0) {
        goto bail;
    }

    const pid_t pid = fork();
    if (pid < 0) {
        goto bail;
    }

    if (pid == 0) { // Child
        if (dup2(to_child[READ_END], STDIN_FILENO) < 0) {
            perror("dup2");
            exit(1);
        }
        close(to_child[READ_END]);
        close(to_child[WRITE_END]);

        if (dup2(to_parent[WRITE_END], STDOUT_FILENO) < 0) {
            perror("dup2");
            exit(1);
        }
        close(to_parent[READ_END]);
        close(to_parent[WRITE_END]);

        execlp(command, command, NULL);
        perror("execlp");
        exit(1);
    }

    // Parent
    close(to_child[READ_END]);
    to_child[READ_END] = INVALID_FD;

    close(to_parent[WRITE_END]);
    to_parent[WRITE_END] = INVALID_FD;

    *in = fdopen(to_parent[READ_END], "r");
    if (*in == NULL) {
        goto bail;
    }
    to_parent[READ_END] = INVALID_FD;

    *out = fdopen(to_child[WRITE_END], "w");
    if (*out == NULL) {
        goto bail;
    }
    to_child[WRITE_END] = INVALID_FD;

    setvbuf(*out, NULL, _IONBF, BUFSIZ);

    return pid;

bail:
    ; // Goto label must be a statement, this is an empty statement
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

int main(void)
{
    FILE* in = NULL;
    FILE* out = NULL;
    char* line = NULL;
    size_t size = 0;

    const int pid = bi_popen("/bin/bash", &in, &out);
    if (pid < 0) {
        perror("bi_popen");
        return 1;
    }

    fprintf(out, "ls -l a.out\n");
    getline(&line, &size, in);
    printf("-> %s", line);

    fprintf(out, "pwd\n");
    getline(&line, &size, in);
    printf("-> %s", line);

    fprintf(out, "date\n");
    getline(&line, &size, in);
    printf("-> %s", line);

    // Since in this case we can tell the child to terminate, we'll do so
    // and wait for it to terminate before we close down.
    fprintf(out, "exit\n");
    waitpid(pid, NULL, 0);

    fclose(in);
    fclose(out);

    return 0;
}