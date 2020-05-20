#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAXLINE 80 /* The maximum length of a command line */

int main(void) {
	char *args[MAXLINE/2+ 1]; /* command line with max 40 arguments */
	int should_run = 1; /* flag to determine when to exit program */
	while (should_run) {
		printf("Kenny-610> ");/* prompt-replace FirstName and L3SID */
		fflush(stdout);

		// Read cmd from user and parse it. Make second token to chop off \n that fgets includes
		char cmd[MAXLINE/2 + 1];
		fgets(cmd, MAXLINE/2 + 1, stdin);
		char *token = strtok(cmd, "\n");
		char *token2 = strtok(token, " ");

		// Add the tokens to args
		int i = 0;
		bool run_concurrently = false;
		while (token2 != NULL) {
			// If there's an &, parent doesn't wait for child. & should not be stored in args
			if (strcmp(token2, "&") == 0)
				run_concurrently = true;
			else
				args[i++] = token2;

			token2 = strtok(NULL, " ");
		}

		// If cmd entered is 0, exit the shell
		if (strcmp(args[0], "exit") == 0) {
			should_run = 0;
			continue;
		}

		args[i] = NULL;	// Last entry of args should be NULL

		pid_t pid = fork();

		if (pid < 0) {	// Fork fails, print out prompt and await next cmd
			printf("Fork failed!\n");
		}
		else if (pid == 0) { // Child, execute the program
			execvp(args[0], args);
			printf("Exec failed. Invalid command\n");
		}
		else if (!run_concurrently) { // Parent and there's no &
			wait(NULL);
		}

	}
	return 0;
}
