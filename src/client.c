#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DST_ADDR "8.130.53.155"
#define DST_PORT 34567

static int sock_fd;
static char buf[1024];

static void init(void)
{
  int ttl;

  sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock_fd < 0) {
    perror("socket");
    exit(-1);
  }

  ttl = 64;
  if (setsockopt(sock_fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0) {
    perror("setsockopt");
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
  struct sockaddr_in dst_addr;
  int t;

  dst_addr.sin_family = AF_INET;
  dst_addr.sin_addr.s_addr = inet_addr(DST_ADDR);
  dst_addr.sin_port = htons(DST_PORT);
  buf[0] = 0;
  buf[1] = 0;
  buf[2] = 0;
  size_t len = 2;
  for (t = 0; t < 600000; ++t)
  {
    //size_t len = 512 + (t & 511);
    //for (size_t j = 0; j < len; ++j)
    //    buf[j] = rand();
    buf[0] = (buf[0] + (buf[1] + 1) / 126) % 126;
    buf[1] = (buf[1] + 1) % 126;
    ssize_t ret = sendto(sock_fd, buf, len, 0,
                         (struct sockaddr *) &dst_addr,
                         sizeof(dst_addr));
    if (ret != len) {
      perror("sendto");
      exit(-1);
    }
    printf("send %ith packet (content=%d %d, timestamp=%llu)...\n",
            t, (int) buf[0],(int)buf[1], (unsigned long long) timestamp());

    usleep(1000 * 40 + rand() % (1000 * 20));
  }
}

int main(void)
{
  init();
  work();

  return 0;
}
