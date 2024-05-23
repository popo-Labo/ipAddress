#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment( lib, "ws2_32.lib"	)
#pragma comment( lib, "iphlpapi.lib"	)

#include <stdio.h>
#include <winsock2.h>
#include <iphlpapi.h>

int main()
{
	DWORD i;
	PMIB_IPADDRTABLE pIpAddrTable = nullptr;
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	/* GetIpAddrTable()で必要になるサイズを取得 */
	if ( GetIpAddrTable( NULL, &dwSize, 0 ) == ERROR_INSUFFICIENT_BUFFER ) {
		pIpAddrTable = (MIB_IPADDRTABLE*) malloc( dwSize );
	}

	/* 実際にGetIpAddrTable()を使う */
	if ( ( dwRetVal = GetIpAddrTable( pIpAddrTable, &dwSize, 0 ) ) == NO_ERROR ) {
		if ( pIpAddrTable->dwNumEntries > 0 ) {
			for ( i = 0; i < pIpAddrTable->dwNumEntries; i++ ) {

				printf( "Address: %s\n", inet_ntoa( *(struct in_addr*) &pIpAddrTable->table[i].dwAddr ) );
				printf( "Mask:    %s\n", inet_ntoa( *(struct in_addr*) &pIpAddrTable->table[i].dwMask ) );
				printf( "Index:   %ld\n", pIpAddrTable->table[i].dwIndex );
				printf( "BCast:   %ld\n", pIpAddrTable->table[i].dwBCastAddr );
				printf( "Reasm:   %ld\n", pIpAddrTable->table[i].dwReasmSize );
				printf( "\n" );
			}
		}
	}
	else {
		printf( "GetIpAddrTable failed.\n" );
		LPVOID lpMsgBuf;

		if ( FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwRetVal,
			MAKELANGID( LANG_NEUTRAL,
			SUBLANG_DEFAULT ), //Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL ) ) {
			printf( "\tError: %s", lpMsgBuf );
		}
		LocalFree( lpMsgBuf );
	}
	return 0;
}
