#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BIND_ADDR "0.0.0.0"
#define BIND_PORT 34567

static int sock_fd;
static char buf[2048];

static void init(void)
{
  struct sockaddr_in bind_addr;

  sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock_fd < 0) {
    perror("socket");
    exit(-1);
  }

  bind_addr.sin_family = AF_INET;
  bind_addr.sin_addr.s_addr = inet_addr(BIND_ADDR);
  bind_addr.sin_port = htons(BIND_PORT);

  if (bind(sock_fd, (struct sockaddr *) &bind_addr,
           sizeof(bind_addr)) < 0) {
    perror("bind");
    exit(-1);
  }
}

static uint64_t timestamp(void)
{
  struct timespec tms;

  if (clock_gettime(CLOCK_REALTIME, &tms) < 0) {
    perror("clock_gettime");
    exit(-1);
  }

  return tms.tv_sec * 1000000000ull + tms.tv_nsec;
}

static void work(void)
{
  struct sockaddr src_addr;
  socklen_t src_addrlen;
  int t;

  t = 0;
  while (true)
  {
    src_addrlen = sizeof(src_addr);

    ssize_t len = recvfrom(sock_fd, buf, sizeof(buf), 0,
                           &src_addr, &src_addrlen);
    if (len < 0) {
      perror("recvfrom");
      exit(-1);
    }

    ssize_t ret = sendto(sock_fd, buf, len, 0,
                         &src_addr, src_addrlen);
    if (len != ret) {
      perror("sendto");
      exit(-1);
    }

    printf("recv %ith packet (len=%lu, timestamp=%llu)...\n",
            t, (unsigned long) len, (unsigned long long) timestamp());

    ++t;
  }
}

int main(void)
{
  init();
  work();

  return 0;
}
