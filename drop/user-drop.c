#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <arpa/inet.h>

#define MTU_SIZE 1024

static uint64_t pkts = 0;
static uint64_t bytes = 0;

void counter()
{
    printf("packets=%lu bytes=%lu\n", pkts, bytes);
    pkts = 0;
    bytes = 0;
}

int main()
{
    struct sockaddr_in in = {0};
    struct sigaction sa = {0};
    struct itimerval t = {0};
    char buf[MTU_SIZE];
    int fd, res;

    sa.sa_handler = &counter;
    sigaction(SIGALRM, &sa, NULL);

    t.it_value.tv_sec = 1;
    t.it_interval.tv_sec = 1;
    setitimer(ITIMER_REAL, &t, NULL);

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
        exit(EXIT_FAILURE);

    in.sin_family = AF_INET;
    in.sin_addr.s_addr = INADDR_ANY;
    in.sin_port = htons(1234);

    if (bind(fd, (struct sockaddr*)&in, sizeof(in)) < 0)
        exit(EXIT_FAILURE);

    while (1) {
        int res = read(fd, buf, MTU_SIZE);

	if (res <= 0) {
	    if (errno == EINTR) {
		continue;
	    }
            break;
	}

        pkts += 1;
        bytes += res;
    }

    close(fd);
    return 0;
}
