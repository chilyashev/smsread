/*
 *
 * Copyright (C) 2013 Mihail Chilyashev
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <termio.h>
#include <string.h>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <unistd.h> // nedded for write
#include <string>
#include <iostream>
#include <iconv.h>

#define DEVICE "/dev/ttyUSB2"

using namespace std;

FILE *fdr, *fdw;
int fd;
struct termio term_save;

void endItAll(int sig) {
    if (fdr) fclose(fdr);
    if (fdw) fclose(fdw);
    ioctl(fd, TCSETA, &term_save);
    close(fd);
    exit(sig);
}

int toASCII(char *&out, char * in) {
    int len = strlen(in),
            i = 0,
            j = 0,
            ind = 0,
            ok = 1;
    char ret[((int) len / 4) + 1];
    for (i = 0; i < len; i++) {
        ind = i;
        if (in[0] == '"') {
            ind = i - 1;
        }

        if (!(ind % 4)) {
            char tmp[3], *end;
            tmp[0] = in[i];
            tmp[1] = in[i + 1];
            tmp[2] = in[i + 2];
            tmp[3] = in[i + 3];
            tmp[4] = '\0';
            long int cnv = strtol(tmp, &end, 16);
            ret[j++] = cnv;
        }
    }
    ret[j] = '\0';
    out = ret;
    return ok;
}

int main(int argc, char *argv[]) {
    char buffer[2048];
    int num;
    struct termio term;
    if ((fd = open(DEVICE, O_RDWR | O_NDELAY)) < 0) {
        perror(DEVICE);
        exit(errno);
    }

    ioctl(fd, TCGETA, &term_save);
    signal(SIGHUP, endItAll);
    signal(SIGINT, endItAll);
    signal(SIGQUIT, endItAll);
    signal(SIGTERM, endItAll);


    ioctl(fd, TCGETA, &term);
    term.c_cflag |= CLOCAL | HUPCL;
    term.c_cflag &= ~CBAUD;
    term.c_cflag |= B19200;
    term.c_lflag &= ~(ICANON | ECHO); /* force raw mode */
    term.c_iflag &= ~ICRNL;
    term.c_cc[VMIN] = 0;
    term.c_cc[VTIME] = 10;
    ioctl(fd, TCSETA, &term);
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) & ~O_NDELAY);

    /*  open tty for reading... */
    if ((fdr = fopen(DEVICE, "r")) == NULL) {
        perror(DEVICE);
        exit(errno);
    }

    /* ... and writing*/
    if ((fdw = fopen(DEVICE, "w")) == NULL) {
        perror(DEVICE);
        exit(errno);
    }

    puts("Sending stuff...\n");

    while (1) {
        // Sending commands
        write(fileno(fdw), "AT+CMGF=1\r\n", 11);
        write(fileno(fdw), "AT+CSCS=\"UTF8\"\r\n", 16);
        write(fileno(fdw), "AT+CMGL=\"ALL\"\r\n", 15);

        string tmp;
        while ((num = read(fileno(fdr), buffer, 1)) > 0) {
            tmp.append(&buffer[0]);
        }
        char *pch;
        char *dup = strdup(tmp.c_str());
        char *lines[1000];
        pch = strtok(dup, "\n");
        int j = 0;
        while (pch != NULL) {
            lines[j++] = pch;
            pch = strtok(NULL, "\n");
        }

        int li = 0, infi = 0, msgNum = 0;

        for (li = 0; li < j; li++) {
            if (strstr(lines[li], "+CMGL")) {
                infi = 0;
                char *info = strdup(lines[li]);
                char *infoStuff[6];
                char *from = strtok(info, ",");
                while (from != NULL) {
                    infoStuff[infi++] = from;
                    from = strtok(NULL, ",");
                }
                char* phone, *msg;
                toASCII(phone, infoStuff[2]);
                printf("Message #%d From: %s on %s at %s\n", msgNum++, phone, infoStuff[3], infoStuff[4]);
                toASCII(msg, lines[li + 1]);
                puts(msg);
                puts("\n");
            }
        }
        free(dup);
        endItAll(0);

    }
} 