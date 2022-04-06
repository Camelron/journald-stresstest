#include <systemd/sd-journal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <pthread.h>

#define MESSAGE_SIZE 75

void entry0() {
    printf("entry0 ran on pid: %d\n", getpid());
    sd_journal_send("MESSAGE=clubpenguino ;)",
                        "MESSAGE_ID=52fb62f99e2c49d89cfbf9d6de5e3555",
                        "PRIORITY=5",
			"CODE_FILE=stresstest_basic.c",
			"CODE_LINE=600",
			"CODE_FUNC=entry0",
			"ERRNO=2",
			"SYSLOG_FACILITY=garbage",
			"SYSLOG_IDENTIFIER=cloggingcomponent",
			"SYSLOG_PID=foobar",
			"MY_FIELD=cameron",
			"MY_FIELD1=cameron",
			"MY_FIELD2=cameron",
			"MY_FIELD3=cameron",
			"MY_FIELD4=cameron",
			"MY_FIELD5=cameron",
			"MY_FIELD6=cameron",
                        "HOME=%s", getenv("HOME"),
                        "TERM=%s", getenv("TERM"),
                        "PAGE_SIZE=%li", sysconf(_SC_PAGESIZE),
                        "N_CPUS=%li", sysconf(_SC_NPROCESSORS_ONLN),
                        NULL);
	sleep(5);
}

// should give a value in us between 0 and 1 seconds 
int randomvalus() {
	float n = (float)rand()/RAND_MAX;
	n *= 1000000;
	// printf("generated: %d\n", (int)n);
	return (int)n;
}

void * killChild(void *args) {
	usleep(randomvalus());
	kill(*((pid_t *)args), SIGKILL);
	return NULL;
}

int main(int argc, char *argv[]) {
	uid_t uid = getuid();
	printf("User id for main is: %d\n", uid);
	while(true) {
	pid_t pid = fork();
        if (pid == 0) {
            // child
            entry0();
            return 0;
        } else {
	    // main process
	    // attempt to interrupt the child process to invalidate the pid
	    // do this by creating a race condition; 
	    // pthread will kill the child process with pid: pid after 0-1 second delay
	    // in the meantime, main thread will try to log about the child thread using OBJECT_PID
	    // this will, in theory, expose a state where main thread is logging about a process which is killed. 
	    pthread_t thread_id;
	    pthread_create(&thread_id, NULL, killChild, (void *)&pid);
	    char *message_str = (char*)malloc(MESSAGE_SIZE * sizeof(char));
	    snprintf(message_str, MESSAGE_SIZE, "MESSAGE=loggingmessage about process with pid: %d", pid);
	    char *objpid_str = (char*)malloc(MESSAGE_SIZE * sizeof(char));
	    snprintf(objpid_str, MESSAGE_SIZE, "OBJECT_PID=%d", pid);
	    sd_journal_send(message_str,
                        	"MESSAGE_ID=52fb62f99e2c49d89cfbf9d6de5e3555",
                        	"PRIORITY=5",
				"CODE_FILE=stresstest_basic.c",
				"CODE_LINE=600",
				"CODE_FUNC=entry0",
				"ERRNO=2",
				"SYSLOG_FACILITY=garbage",
				"SYSLOG_IDENTIFIER=cloggingcomponent",
				"SYSLOG_PID=foobar",
				objpid_str,
                        	NULL);
	    free(message_str);
	    free(objpid_str);
	    pthread_join(thread_id, NULL);
	}
        //sleep(5);
    }
    return 0;
}
