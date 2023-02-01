#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>

// let's define a do_something function we can loop with
static void msg(const char *msg) {
    fprintf(stderr, "%s\n", msg);
}

static void die(const char *msg) {
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}
static void do_something(int connfd) {
    char rbuf[64] = {};
    ssize_t n = read(connfd, rbuf, sizeof(rbuf) -1);
    if (n<0) {
        msg("read() error");
        return;
    }
    printf("client says: %s\n", rbuf);

    char wbuf[] = "world";
    write(connfd, wbuf, strlen(wbuf));
}

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET for ipv4, SOCK_STREAM for TCP
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    // setsockopt() call is to configure various aspects of a socket, this call enables SO_REUSEADDR, which allows a socket to bind to a port in use by another socket
    //let's bind to 0.0.0.0:1234:!
    //but first let's write some code to deal w/ ipv4 addresses
    struct sockaddr_in addr = {}; //struct groups variables in one place
    addr.sin_family = AF_INET; // address family 
    addr.sin_port = ntohs(1234); //port number, ntohs returns # in host byte order
    addr.sin_addr.s_addr = ntohl(0); //wildcard address, access as one 4-byte integer with s_addr, ntohl same as above but 32-bit instead of 16 bit
    // ok now we can bind!
    int rv = bind(fd, (const struct sockaddr *) &addr, sizeof(addr));
    if (rv) {
        die("bind()"); //die = dies the thing and displays STDERR
    }
    // let's listen...
    rv = listen(fd, SOMAXCONN); //SOMAXCONN defines the max number of connection requests you're allowed to pass to listen()
    if (rv) {
        die("listen()");
    }


    while (free) {
            // accept
            struct sockaddr_in client_addr = {};
            socklen_t socklen = sizeof(client_addr);
            int connfd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
            if (connfd < 0) {
                continue;   // error
            }

            do_something(connfd);
            close(connfd);
        }

        return 0;
}