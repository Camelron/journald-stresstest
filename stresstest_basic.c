#include <systemd/sd-journal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>

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
}

// should give a value in ms between 0 and 1 seconds 
int randomvalms() {
	float n = (float)rand()/RAND_MAX;
	n *= 1000;
	printf("generated: %d\n", (int)n);
	return (int)n;
}

int main(int argc, char *argv[]) {
	while(true) {
	pid_t pid = fork();
        if (pid == 0) {
            // child
            entry0();
            return 0;
        } else {
	    // main process
	    // attempt to interrupt the child process to invalidate the pid
	    usleep(randomvalms());
	    kill(pid, SIGKILL);
	}
        //sleep(5);
    }
    return 0;
}
