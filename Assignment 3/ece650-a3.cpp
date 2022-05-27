#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
using namespace std;
int main (int argc, char **argv) {

    int pipe_one[2];
    int pipe_two[2];

    pipe(pipe_one);
    pipe(pipe_two);
    pid_t proc_t;
    proc_t = fork ();

    if (proc_t == 0) {
	dup2(pipe_one[1], STDOUT_FILENO);
	close(pipe_one[1]);
	execv ("rgen", argv);
    }

	pid_t proc_t1 = fork();
	if(proc_t1 == 0){
		dup2(pipe_one[0], STDIN_FILENO);
        close(pipe_one[0]);
		dup2(pipe_two[1], STDOUT_FILENO);
		close(pipe_two[1]);
		char *arg_py[2];
		arg_py[0] = (char *)"python3";
		arg_py[1] = (char *)"ece650-a1.py";
		execvp("python3",arg_py);
	}

	pid_t proc_t2 = fork();
	if(proc_t2 == 0){
		dup2(pipe_two[0], STDIN_FILENO);
        	close(pipe_two[0]);
	        execv("ece650-a2",argv);
	}

	dup2(pipe_two[1], STDOUT_FILENO);
	close(pipe_two[1]);
	string line;
	while(!cin.eof()){
		getline(cin, line);
		if(line.size() > 0) cout << line << endl;
	}

	int t1,t2,t3;
  	kill (proc_t, SIGTERM);
  	waitpid(proc_t, &t1, 0);
	kill (proc_t1, SIGTERM);
  	waitpid(proc_t1, &t2, 0);
	kill (proc_t2, SIGTERM);
  	waitpid(proc_t2, &t3, 0);

	return 0;
}
