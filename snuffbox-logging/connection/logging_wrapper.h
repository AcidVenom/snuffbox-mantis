#pragma once

#include <assert.h>

#ifdef SNUFF_WIN32
#define set_blocking_socket(sock, x) \
{\
	u_long mode = x ? 0 : 1;\
	ioctlsocket(sock, FIONBIO, &mode);\
}

#undef errno
#define errno WSAGetLastError()

#define SNUFF_IS_CONNECTED WSAEISCONN
#define SNUFF_WOULD_BLOCK WSAEWOULDBLOCK
#endif

#ifdef SNUFF_LINUX
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define set_blocking_socket(sock, x) \
if (x == true) \
{ \
	fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) & (~O_NONBLOCK)); \
} \
if (x == false) \
{ \
	fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | (O_NONBLOCK)); \
}
#define closesocket(x) close(x)
#define SNUFF_IS_CONNECTED EISCONN
#define SNUFF_WOULD_BLOCK EWOULDBLOCK
#endif