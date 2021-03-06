/*
* New system call Demo
* syscall 'pinfo'
*/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

/* To call pinfo without libc wrapper */
#ifndef SYSCALL_THROUGH_LIBC
int pinfo(struct prcs_info *pif, pid_t pid)
{
	int err;	
	__asm(
	    "MOV x8, #285;" //x8 holds syscall no
	    " SVC #0;"      // supervisor call
	    "MOV %[result], x0" : [result] "=r"(err) // copy return code to err variable
	);
	return err; 
}
#endif

int main(int argc, char **argv)
{
	int ret;
	pid_t pid;

	printf("System Call Demo \n");
	printf("syscall pinfo() \n");

	struct prcs_info pif =
	{
		.prio = -1,
		.state = -1,	/* -1 unrunnable, 0 runnable, >0 stopped */
		.cpu = 0, 	 	
		.nvcsw = 0,
		.start_time = 0,
	};

//	pid = getpid(); // current process
//	pid = 1; // for init process
//	pid = -1; // to test errno

	if (argc > 1)
		pid = atoi(argv[1]);
	else
		pid = getpid();

	ret = pinfo(&pif, pid);
	if (ret < 0) {
		printf("System call error : %s\n", strerror(-ret));
		return -1;
	}

	printf("Process info : PID %d \n", pid);
	printf("Process priority : %d\n", pif.prio);
	printf("Process state : %ld\n", pif.state);
	printf("Current CPU : %d\n", pif.cpu);
	printf("Context switch count : %lu\n", pif.nvcsw);
	printf("Process start time (ns) : %llu\n", pif.start_time);

	printf("================= \n\n"); 
    return 0;
}

