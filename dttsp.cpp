/*

The code contained in this file has been excerpted from DttSP port-clients.c
and adapted to c++ by A.Montefusco - IW0HDV

Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009 by Frank Brickle, AB2KT and Bob McGwier, N4HY


This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>  
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/socket.h>  
#include <netdb.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>

#include "dttsp.h"



DttSP :: DttSP (bool verbose, int port, int inbound):
    port(port)
{
    pSa = new struct sockaddr_in;
	address = NULL;

    if (pSa) {
        // create socket 
        if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
          perror("Couldn't create dttsp_port_client socket");
          exit(1);
        }

        flags = 0;

        // are we the listener here?
        if (inbound) {
          clen = sizeof(*pSa);
          memset((char *) pSa, 0, clen);
          pSa->sin_family = AF_INET;
          pSa->sin_addr.s_addr = htonl(INADDR_ANY);
          pSa->sin_port = htons((unsigned short) port);

          if (bind(sock, (struct sockaddr *) pSa, clen) < 0) {
            perror("Failed to bind socket");
            exit(1);
          }
        } // else no, so sockaddr gets filled in at point of use, not bound by us

                if(verbose) fprintf( stderr, "DttSP port %d\n", port);

        // one size fits all
        size = DTTSP_PORT_CLIENT_BUFSIZE;
        memset(buff, 0, size);
    }
}

DttSP :: DttSP (bool verbose, int port, int inbound, char *host):
        port(port)
{
    struct addrinfo		hints;
    int					rc;
    char				porttxt[32];

    if (inbound) {
        sethostent(0);
        memset(&hints, 0, sizeof(struct addrinfo));
        snprintf(porttxt, 32, "%d", port);
        hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = IPPROTO_UDP;	//PF_INET;
        if ( (rc = getaddrinfo(NULL, porttxt, &hints, &address)) != 0 ) {
            fprintf( stderr, "getaddrinfo for inbound %s %d failed %s\n", host, port, gai_strerror(rc));
            return;
        }

        // create socket 
        if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
            perror("Couldn't create dttsp_port_client socket");
            exit(1);
        }

        flags = 0;
        if (bind(sock, address->ai_addr, address->ai_addrlen) < 0) {
            perror("Failed to bind socket");
            exit(1);
        }
        if (verbose) fprintf( stderr, "DttSP inbound host %s port %d addrlen %d\n", host, port, address->ai_addrlen);
    } else {
        sethostent(0);
        memset(&hints, 0, sizeof(struct addrinfo));
        snprintf(porttxt, 32, "%d", port);
        //hints.ai_flags = AI_PASSIVE | ;
        hints.ai_flags = AI_NUMERICSERV;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = IPPROTO_UDP;	//PF_INET;
        if ( (rc = getaddrinfo(host, porttxt, &hints, &address)) != 0 ) {
            fprintf( stderr, "getaddrinfo for outbound %s %d failed %s\n", host, port, gai_strerror(rc));
            return;
        }

        // create socket 
        if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
            perror("Couldn't create dttsp_port_client socket");
            exit(1);
        }

        flags = 0;

        fprintf( stderr, "DttSP outbound host %s port %d\n", host, port);

    }
    // one size fits all
    size = DTTSP_PORT_CLIENT_BUFSIZE;
    memset(buff, 0, size);
}

DttSP :: ~DttSP ()
{
    close (sock);
    if (address == NULL)
    	delete pSa;
    else
        delete address;
}


int DttSP :: send_command ( char *cmdstr ) 
{
    fd_set fds;
        struct timeval tv;

        char *saveptr;
        char *token;

    // are we pointing at the moon?
    if ((!pSa && !address) || sock == -1 || !cmdstr)
//    if (!pSa  || sock == -1 || !cmdstr)
        return -1;

    // make local, properly terminated copy of command
    // Needs better error checking
    // TBD
    strcpy (buff, cmdstr);
    strcat (buff, "\n");
    used = strlen (buff);

    // blast it
    if (address == NULL) {
        clen = sizeof(*pSa);
    	memset((char *) pSa, 0, clen);
    	pSa->sin_family = AF_INET;
    	pSa->sin_addr.s_addr = htonl(INADDR_ANY);
    	pSa->sin_port = htons((unsigned short) port);
    	if (sendto(sock, buff, used, flags, (struct sockaddr *) pSa, clen) != used) {
            fprintf (stderr, "%s: error in sendto\n", __FUNCTION__);
            return -3;
        }
    } else {

    	if (sendto(sock, buff, used, flags, address->ai_addr, address->ai_addrlen) != used) {
            fprintf (stderr, "%s: error in sendto\n", __FUNCTION__);
            return -3;
    	}
    }

    // wait a little for ack
    FD_ZERO(&fds);
    FD_SET(sock, &fds);
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    if (!select(sock + 1, &fds, 0, 0, &tv)) {
        fprintf (stderr, "%s: error from select, disabling port\n", __FUNCTION__);
        fprintf (stderr, "Perhaps sdr-core isn't started, or usbsoftrock isn't running (when selected in CFG).\n");
        close (sock);
        sock = -1;
        delete pSa;
        pSa = NULL;
       	return -4;
    }
    if (address == NULL) {
        if (recvfrom(sock, buff, size, flags, (struct sockaddr *) pSa, (socklen_t *)(&clen)) <= 0) {
            fprintf (stderr, "%s: error in recvfrom\n", __FUNCTION__);
            return -5;
        }
    } else {
        if (recvfrom(sock, buff, size, flags, address->ai_addr, &address->ai_addrlen) <= 0) {
            fprintf (stderr, "%s: error in recvfrom\n", __FUNCTION__);
            return -5;
        }
    }

    if (buff[0] != 'o' || buff[1] != 'k') return -6;

	param = 0.0;
        token = strtok_r(buff, " ", &saveptr);		// parse the buffer
        if (token != NULL) {  
          token = strtok_r(NULL, " ", &saveptr);	// parse the next
	  if (token != NULL) param = atof(token);	// param is double number
        }


    return 0;
}

int DttSPcmd :: sendCommand ( const char *format, ... )
{
    va_list ap;
    char    szBuf [BUFSIZ];
    int     rc;

    va_start(ap, format);

    vsprintf ( szBuf, format, ap );
    rc = send_command (szBuf);

    va_end(ap);

    return rc;

}


int DttSPmeter :: fetch (int *label, float *data, int npts)
{
  fd_set fds;
  struct timeval tv;

  // wait a bit for data to appear
  FD_ZERO(&fds);
  FD_SET(sock, &fds);
  tv.tv_sec = 1;
  tv.tv_usec = 0;
  if (!select(sock + 1, &fds, 0, 0, &tv))
    return -1;
  if (recvfrom(sock, buff, size, flags,
	       (struct sockaddr *) pSa, (socklen_t*)&clen) <= 0)
    return -2;

  // copy payload back to client space
  memcpy((char *) label, buff,               sizeof(int)          );
  memcpy((char *) data,  buff + sizeof(int), npts * sizeof(float) );
  return 0;
}

int  DttSPspectrum :: fetch ( int *tick, int *label, float *data, int npts) 
{
  fd_set fds;
  struct timeval tv;

  // wait a bit for data to appear
  FD_ZERO(&fds);
  FD_SET(sock, &fds);
  tv.tv_sec = 1;
  tv.tv_usec = 0;
  if (!select(sock + 1, &fds, 0, 0, &tv))
    return -1;
  if (recvfrom(sock, buff, size, flags,
	       (struct sockaddr *) pSa, (socklen_t*)&clen) <= 0)
    return -2;

  // copy payload back to client space
  memcpy((char *) tick,  buff,                   sizeof(int)          );
  memcpy((char *) label, buff + sizeof(int),     sizeof(int)          );
  memcpy((char *) data,  buff + 2 * sizeof(int), npts * sizeof(float) );
  return 0;
}

double USBSoftrockCmd :: getParam()
{
	return param;
}

int USBSoftrockCmd :: sendCommand ( const char *format, ... )
{
	va_list ap;
	char    szBuf [BUFSIZ];
	int     rc;

	va_start(ap, format);

	vsprintf ( szBuf, format, ap );
	rc = send_command (szBuf);

	va_end(ap);

	return rc;

}

int DttSPTXcmd :: sendCommand ( const char *format, ... )
{
    va_list ap;
    char    szBuf [BUFSIZ];
    int     rc;

    va_start(ap, format);

    vsprintf ( szBuf, format, ap );
    rc = send_command (szBuf);

    va_end(ap);

    return rc;

}

void DttSPTXcmd :: off ( )
{
    printf("DttSPTXcmd = off\n");
    close (sock);
    sock = -1;
    delete pSa;
    pSa = NULL;
}

void DttSPTXcmd :: on ( )
{
    printf("DttSPTXcmd = on\n");
    pSa = new struct sockaddr_in;

    if (pSa) {
        // create socket 
        if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		  perror("Couldn't create dttsp_port_client socket for DttSPTXcmd");
          exit(1);
        }

        flags = 0;

        if(verbose) fprintf( stderr, "DttSP port %d\n", port);

        // one size fits all
        size = DTTSP_PORT_CLIENT_BUFSIZE;
        memset(buff, 0, size);
    }
}

void DttSPTXcmd :: setPort ( const int newport )
{
    printf("DttSPTXcmd: old port %d new port %d\n", port, newport);
    port = newport;
}

