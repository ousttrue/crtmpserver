/*
 *  Copyright (c) 2010,
 *  Gavriloaie Eugen-Andrei (shiretu@gmail.com)
 *
 *  This file is part of crtmpserver.
 *  crtmpserver is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  crtmpserver is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with crtmpserver.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef WIN32

#include "platform/platform.h"
#include "utils/logging/logging.h"
#include <Strsafe.h>
#include <stdio.h>
#include <mswsock.h>

static map<uint32_t, SignalFnc> _signalHandlers;

string format(string fmt, ...) {
	string result = "";
	va_list arguments;
	va_start(arguments, fmt);
	result = vFormat(fmt, arguments);
	va_end(arguments);
	return result;
}

string vFormat(string fmt, va_list args) {
	char *pBuffer = NULL;
	if (vasprintf(&pBuffer, STR(fmt), args) == -1) {
		ASSERT("vasprintf failed");
		return "";
	}
	string result = pBuffer;
	free(pBuffer);
	return result;
}

int vasprintf(char **strp, const char *fmt, va_list ap, int size) {
	*strp = (char *) malloc(size);
	int result = 0;
	if ((result = vsnprintf(*strp, size, fmt, ap)) == -1) {
		free(*strp);
		return vasprintf(strp, fmt, ap, size + size / 2);
	} else {
		return result;
	}
}

bool fileExists(string path) {
#ifdef UNICODE
	//TODO: Add the unicode implementation here
	NYIR;
#else
	if (PathFileExists(STR(path)))
		return true;
	else
		return false;
#endif /* UNICODE */
}

string tagToString(uint64_t tag) {
	string result;
	for (uint32_t i = 0; i < 8; i++) {
		uint8_t v = (tag >> ((7 - i)*8)&0xff);
		if (v == 0)
			break;
		result += (char) v;
	}
	return result;
}

uint64_t getTagMask(uint64_t tag) {
	uint64_t result = 0xffffffffffffffffULL;
	for (int8_t i = 56; i >= 0; i -= 8) {
		if (((tag >> i)&0xff) == 0)
			break;
		result = result >> 8;
	}
	return ~result;
}

bool isNumeric(string value) {
	return value == format("%d", atoi(STR(value)));
}

string lowerCase(string value) {
	return changeCase(value, true);
}

string upperCase(string value) {
	return changeCase(value, false);
}

void lTrim(string &value) {
	string::size_type i = 0;
	for (i = 0; i < value.length(); i++) {
		if (value[i] != ' ' &&
				value[i] != '\t' &&
				value[i] != '\n' &&
				value[i] != '\r')
			break;
	}
	value = value.substr(i);
}

void rTrim(string &value) {
	int32_t i = 0;
	for (i = (int32_t) value.length() - 1; i >= 0; i--) {
		if (value[i] != ' ' &&
				value[i] != '\t' &&
				value[i] != '\n' &&
				value[i] != '\r')
			break;
	}
	value = value.substr(0, i + 1);
}

void trim(string &value) {
	lTrim(value);
	rTrim(value);
}

int8_t getCPUCount() {
	WARN("Windows doesn't support multiple instances");
	return 0;
}

void replace(string &target, string search, string replacement) {
	if (search == replacement)
		return;
	if (search == "")
		return;
	string::size_type i = string::npos;
	string::size_type lastPos = 0;
	while ((i = target.find(search, lastPos)) != string::npos) {
		target.replace(i, search.length(), replacement);
		lastPos = i + replacement.length();
	}
}

void split(string str, string separator, vector<string> &result) {
	result.clear();
	string::size_type position = str.find(separator);
	string::size_type lastPosition = 0;
	uint32_t separatorLength = (uint32_t) separator.length();

	while (position != str.npos) {
		ADD_VECTOR_END(result, str.substr(lastPosition, position - lastPosition));
		lastPosition = position + separatorLength;
		position = str.find(separator, lastPosition);
	}
	ADD_VECTOR_END(result, str.substr(lastPosition, string::npos));
}

map<string, string> mapping(string str, string separator1, string separator2, bool trimStrings) {
	map<string, string> result;

	vector<string> pairs;
	split(str, separator1, pairs);

	FOR_VECTOR_ITERATOR(string, pairs, i) {
		if (VECTOR_VAL(i) != "") {
			if (VECTOR_VAL(i).find(separator2) != string::npos) {
				string key = VECTOR_VAL(i).substr(0, VECTOR_VAL(i).find(separator2));
				string value = VECTOR_VAL(i).substr(VECTOR_VAL(i).find(separator2) + 1);
				if (trimStrings) {
					trim(key);
					trim(value);
				}
				result[key] = value;
			} else {
				if (trimStrings) {
					trim(VECTOR_VAL(i));
				}
				result[VECTOR_VAL(i)] = "";
			}
		}
	}
	return result;
}

string changeCase(string &value, bool lowerCase) {
	//int32_t len = (int32_t)value.length();
	string newvalue(value);
	for (string::size_type i = 0, l = newvalue.length(); i < l; ++i)
		newvalue[i] = (char) (lowerCase ? tolower(newvalue[i]) : toupper(newvalue[i]));
	return newvalue;
}

int gettimeofday(struct timeval *tv, void* tz) {
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	uint64_t value = ((uint64_t) ft.dwHighDateTime << 32) | ft.dwLowDateTime;
	tv->tv_usec = (long) ((value / 10LL) % 1000000LL);
	tv->tv_sec = (long) ((value - 116444736000000000LL) / 10000000LL);
	return (0);
}

bool HandlerRoutine(uint32_t dwCtrlType) {
	if (MAP_HAS1(_signalHandlers, dwCtrlType)) {
		_signalHandlers[dwCtrlType]();
		return true;
	}
	return false;
}

void installConfRereadSignal(SignalFnc pConfRereadSignalFnc) {
	_signalHandlers[CTRL_BREAK_EVENT] = pConfRereadSignalFnc;
	SetConsoleCtrlHandler((PHANDLER_ROUTINE) HandlerRoutine, TRUE);
}

void installQuitSignal(SignalFnc pQuitSignalFnc) {
	_signalHandlers[CTRL_C_EVENT] = pQuitSignalFnc;
	SetConsoleCtrlHandler((PHANDLER_ROUTINE) HandlerRoutine, TRUE);
}

double getFileModificationDate(string path) {
	struct _stat64 s;
	if (_stat64(STR(path), &s) != 0) {
		FATAL("Unable to stat file %s", STR(path));
		return 0;
	}
	return (double) s.st_mtime;
}

static LPFN_WSASENDMSG Load_WSASendMsg() {
	HMODULE hWSockLib = LoadLibraryA("ws2_32.dll");
	if (hWSockLib == NULL) {
		//   fprintf(stdout, "Unable to load ws2_32.dll\n");
		return NULL;
	}
	LPFN_WSASENDMSG pProc = (LPFN_WSASENDMSG)GetProcAddress(hWSockLib, "WSASendMsg");
	if (pProc == NULL) {
		//   fprintf(stdout, "Unable to find WSASendMsg in ws2_32.dll - using emulation\n");
		return NULL;
	}
	// fprintf(stdout, "WSASendMsg loaded from ws2_32.dll\n");
	return pProc;
}

LPFN_WSASENDMSG WSASendMsg_Real = NULL;

void InitNetworking() {
	SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
	WSASendMsg_Real = Load_WSASendMsg();
	WSADATA wsa;
	memset(&wsa, 0, sizeof (wsa));
	WSAStartup(0, &wsa);
	WSAStartup(wsa.wHighVersion, &wsa);
}

HMODULE UnicodeLoadLibrary(string fileName) {
	return LoadLibrary(STR(fileName));
}

int inet_aton(const char *pStr, struct in_addr *pRes) {
	pRes->S_un.S_addr = inet_addr(pStr);
	return true;
}

bool setFdJoinMulticast(SOCKET sock, string bindIp, uint16_t bindPort, string ssmIp) {
	if (ssmIp == "") {
		struct ip_mreq group;
		group.imr_multiaddr.s_addr = inet_addr(STR(bindIp));
		group.imr_interface.s_addr = INADDR_ANY;
		if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
				(char *) &group, sizeof (group)) < 0) {
			int err = LASTSOCKETERROR;
			FATAL("Adding multicast failed. Error was: %d", err);
			return false;
		}
		return true;
	} else {
		struct group_source_req multicast;
		struct sockaddr_in *pGroup = (struct sockaddr_in*) &multicast.gsr_group;
		struct sockaddr_in *pSource = (struct sockaddr_in*) &multicast.gsr_source;

		memset(&multicast, 0, sizeof (multicast));

		//Setup the group we want to join
		pGroup->sin_family = AF_INET;
		pGroup->sin_addr.s_addr = inet_addr(STR(bindIp));
		pGroup->sin_port = EHTONS(bindPort);

		//setup the source we want to listen
		pSource->sin_family = AF_INET;
		pSource->sin_addr.s_addr = inet_addr(STR(ssmIp));
		if (pSource->sin_addr.s_addr == INADDR_NONE) {
			FATAL("Unable to SSM on address %s", STR(ssmIp));
			return false;
		}
		pSource->sin_port = 0;

		INFO("Try to SSM on ip %s", STR(ssmIp));

		if (setsockopt(sock, IPPROTO_IP, MCAST_JOIN_SOURCE_GROUP, (char *)&multicast,
				sizeof (multicast)) < 0) {
			int err = LASTSOCKETERROR;
			FATAL("Adding multicast failed. Error was: (%d)", err);
			return false;
		}

		return true;
	}
}

bool setFdCloseOnExec(int fd) {
	return true;
}

bool setFdNonBlock(SOCKET fd) {
	u_long iMode = 1; // 0 for blocking, anything else for nonblocking

	if (ioctlsocket(fd, FIONBIO, &iMode) < 0) {
		int err = LASTSOCKETERROR;
		FATAL("Unable to set fd flags: %d", err);
		return false;
	}
	return true;
}

bool setFdNoSIGPIPE(SOCKET fd) {
	return true;
}

bool setFdKeepAlive(SOCKET fd, bool isUdp) {
	if (isUdp)
		return true;
	int value = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *) &value, sizeof (int)) != 0) {
		DWORD err = WSAGetLastError();
		/*FATAL("setsockopt failed with error %"PRIu32, err);*/
		WARN/*FATAL*/("Error #%u", WSAGetLastError());
		return false;
	}
	return true;
}

bool setFdBuffers(intptr_t fd) {
	int rbs = 128 * 1024;
	if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (char *)&rbs, sizeof(rbs)) == SOCKET_ERROR) {
		FATAL("Error #%u", WSAGetLastError());
		return false;
	}
	int sbs = 128 * 1024;
	if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char *)&sbs, sizeof(sbs)) == SOCKET_ERROR) {
		FATAL("Error #%u", WSAGetLastError());
		return false;
	}

	return true;
}

bool setFdNoNagle(SOCKET fd, bool isUdp) {
	if (isUdp)
		return true;
	BOOL value = TRUE;
	if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *) &value, sizeof (BOOL)) == SOCKET_ERROR) {
		DWORD err = WSAGetLastError();
		FATAL("Unable to disable Nagle algorithm. Error was: %"PRIu32, err);
		return false;
	}

	return true;
}

bool setFdReuseAddress(SOCKET fd) {
	BOOL value = TRUE;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) &value, sizeof (BOOL)) == SOCKET_ERROR) {
		FATAL("Error #%u", WSAGetLastError());
		return false;
	}
	return true;
}

bool setFdTTL(SOCKET fd, uint8_t ttl) {
	NYI;
	return true;
}

bool setFdMulticastTTL(SOCKET fd, uint8_t ttl) {
	NYI
	return true;
}

bool setFdTOS(SOCKET fd, uint8_t tos) {
	NYI
	return true;
}

bool setFdOptions(SOCKET fd, bool isUdp) {
	if (!isUdp) {
		if (!setFdNonBlock(fd)) {
			FATAL("Unable to set non block");
			return false;
		}
	}

	if (!setFdNoSIGPIPE(fd)) {
		FATAL("Unable to set no SIGPIPE");
		return false;
	}

	if (!setFdKeepAlive(fd, isUdp)) {
		//FATAL("Unable to set keep alive");
		//return false;
		WARN/*FATAL*/("Unable to set keep alive");
	}

	if (!setFdNoNagle(fd, isUdp)) {
		WARN("Unable to disable Nagle algorithm");
	}

	if (!setFdBuffers(fd)) {
		FATAL("Unable to set socket buffers");
		return false;
	}

	if (!setFdReuseAddress(fd)) {
		FATAL("Unable to enable reuse address");
		return false;
	}

	return true;
}

string alowedCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

string generateRandomString(uint32_t length) {
	string result = "";
	for (uint32_t i = 0; i < length; i++)
		result += alowedCharacters[rand() % alowedCharacters.length()];
	return result;
}

string getHostByName(string name) {
	struct hostent *pHostEnt = gethostbyname(STR(name));
	if (pHostEnt == NULL)
		return "";
	if (pHostEnt->h_length <= 0)
		return "";
	string result = format("%hhu.%hhu.%hhu.%hhu",
			(uint8_t) pHostEnt->h_addr_list[0][0],
			(uint8_t) pHostEnt->h_addr_list[0][1],
			(uint8_t) pHostEnt->h_addr_list[0][2],
			(uint8_t) pHostEnt->h_addr_list[0][3]);
	return result;
}

void splitFileName(string fileName, string &name, string & extension, char separator) {
	size_t dotPosition = fileName.find_last_of(separator);
	if (dotPosition == string::npos) {
		name = fileName;
		extension = "";
		return;
	}
	name = fileName.substr(0, dotPosition);
	extension = fileName.substr(dotPosition + 1);
}

string normalizePath(string base, string file) {
	char dummy1[MAX_PATH ];
	char dummy2[MAX_PATH ];
	if (GetFullPathName(STR(base), MAX_PATH, dummy1, NULL) == 0)
		return "";
	if (GetFullPathName(STR(base + file), MAX_PATH, dummy2, NULL) == 0)
		return "";

	base = dummy1;
	file = dummy2;

	if (file == "" || base == "") {
		return "";
	}

	if (file.find(base) != 0) {
		return "";
	} else {
		if (!fileExists(file)) {
			return "";
		} else {
			return file;
		}
	}
}

bool listFolder(string path, vector<string> &result, bool normalizeAllPaths,
		bool includeFolders, bool recursive) {
	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	// Check that the input path plus 3 is not longer than MAX_PATH.
	// Three characters are for the "\*" plus NULL appended below.
	if (path.size() > (MAX_PATH - 3)) {
		WARN("Directory path is too long: %s.", STR(path));
		return false;
	}

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.

	StringCchCopy(szDir, MAX_PATH, STR(path));
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	// Find the first file in the directory.

	hFind = FindFirstFile(szDir, &ffd);
	if (hFind == INVALID_HANDLE_VALUE) {
		FATAL("Unable to open folder %s", STR(path));
		return false;
	}

	do {
		string entry = ffd.cFileName;
		if ((entry == ".") || (entry == "..")) {
			continue;
		}

		if (normalizeAllPaths) {
			entry = normalizePath(path, entry);
		} else {
			entry = path + entry;
		}
		if (entry == "")
			continue;

		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (includeFolders) {
				ADD_VECTOR_END(result, entry);
			}
			if (recursive) {
				if (!listFolder(entry, result, normalizeAllPaths, includeFolders, recursive)) {
					FATAL("Unable to list folder");
					FindClose(hFind);
					return false;
				}
			}
		} else {
			ADD_VECTOR_END(result, entry);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) {
		WARN("Unable to find first file");
		FindClose(hFind);
		return false;
	}
	FindClose(hFind);
	return true;
}

bool deleteFile(string path) {
	if (remove(STR(path)) != 0) {
		FATAL("Unable to delete file `%s`", STR(path));
		return false;
	}
	return true;
}

bool deleteFolder(string path, bool force) {
	string fileFound;
	WIN32_FIND_DATA info;
	HANDLE hp;
	fileFound = format("%s\\*.*", STR(path));
	hp = FindFirstFile(STR(fileFound), &info);

	// Check first if we have a valid handle!
	if (hp == INVALID_HANDLE_VALUE) {
		WARN("Files to be deleted were already removed: %s", STR(fileFound));
		return true;
	}

	do {
		if (!((strcmp(info.cFileName, ".") == 0) ||
				(strcmp(info.cFileName, "..") == 0))) {
			if ((info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ==
					FILE_ATTRIBUTE_DIRECTORY) {
				string subFolder = path;
				subFolder.append("\\");
				subFolder.append(info.cFileName);
				if (!deleteFolder(subFolder, true)) {
					FATAL("Unable to delete subfolder %s", STR(subFolder));
					return false;
				}
				if (!RemoveDirectory(STR(subFolder))) {
					FATAL("Unable to delete subfolder %s", STR(subFolder));
					return false;
				}
			} else {
				fileFound = format("%s\\%s", STR(path), info.cFileName);
				if (!DeleteFile(STR(fileFound))) {
					FATAL("Unable to delete file %s", STR(fileFound));
					return false;
				}
			}
		}
	} while (FindNextFile(hp, &info));
	FindClose(hp);
	return true;
}

bool createFolder(string path, bool recursive) {
	char DirName[256];
	const char* p = path.c_str();
	char* q = DirName;
	while (*p) {
		if (('\\' == *p) || ('/' == *p)) {
			if (':' != *(p - 1)) {
				CreateDirectory(DirName, NULL);
			}
		}
		*q++ = *p++;
		*q = '\0';
	}
	CreateDirectory(DirName, NULL);
	return true;
}

bool moveFile(string src, string dst) {
	if (rename(STR(src), STR(dst)) != 0) {
		FATAL("Unable to move file from `%s` to `%s`",
				STR(src), STR(dst));
		return false;
	}
	return true;
}

bool isAbsolutePath(string &path) {
	if (path.size() < 4)
		return false;
	return (((path[0] >= 'A')&&(path[0] <= 'Z')) || ((path[0] >= 'a')&&(path[0] <= 'z')))
			&&(path[1] == ':')
			&&(path[2] == '\\');
}

static time_t _gUTCOffset = -1;

void computeUTCOffset() {
	//time_t now = time(NULL);
	//struct tm *pTemp = localtime(&now);
	//_gUTCOffset = pTemp->tm_gmtoff;
	NYIA;
}

time_t getlocaltime() {
	if (_gUTCOffset == -1)
		computeUTCOffset();
	return getutctime() + _gUTCOffset;
}

time_t gettimeoffset() {
	if (_gUTCOffset == -1)
		computeUTCOffset();
	return _gUTCOffset;
}

INT WSASendMsg_WRAPPER(SOCKET s, LPWSAMSG lpMsg, DWORD dwFlags,
	LPDWORD lpNumberOfBytesSent,
	LPWSAOVERLAPPED lpOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) {
		if (WSASendMsg_Real != NULL)
			return WSASendMsg_Real(s, lpMsg, dwFlags,
			lpNumberOfBytesSent,
			lpOverlapped,
			lpCompletionRoutine);
		if (lpMsg == NULL) {
			FATAL("Error: no data in WSASendMsg_WRAPPER");
			return 0;
		}
		if (lpOverlapped != NULL) {
			FATAL("Error: overlapped in WSASendMsg_WRAPPER");
			return 0;
		}
		if (lpCompletionRoutine != NULL) {
			FATAL("Error: completion routine in WSASendMsg_WRAPPER");
			return 0;
		}
		char tmpbuf[65536];
		uint32_t tmplen = 0;
		for (DWORD i = 0; i < lpMsg->dwBufferCount; i++) {
			WSABUF & wsaBuf = lpMsg->lpBuffers[i];
			if ((tmplen + wsaBuf.len) > sizeof(tmpbuf)) {
				FATAL("Error: can not send data WSASendMsg_WRAPPER - data too long");
				return 0;
			}
			memcpy(tmpbuf + tmplen, wsaBuf.buf, wsaBuf.len);
			tmplen += wsaBuf.len;
		}
		int res = sendto(s, tmpbuf, tmplen, dwFlags, lpMsg->name, lpMsg->namelen);
		if (res == SOCKET_ERROR) {
			return res;
		}
		if (lpNumberOfBytesSent != NULL) {
			*lpNumberOfBytesSent = res;
		}
		return 0;
}
#endif /* WIN32 */
