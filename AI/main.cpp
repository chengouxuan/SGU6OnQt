// main.cpp : ��������̨Ӧ�ó��������ڵ㡣
//

#include "Connect6.h"
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cassert>
#include <cstdio>
#include "defines.h"
#include "MoveSearcher.h"
#include "utilities.h"
#include "Frame.h"

#pragma comment(lib,"WS2_32.lib")

extern MoveSearcher searcher;

void ReInitParams(const char *);
void ReInitShapes(const char *);

int main(int argc, char* argv[])
{
    if(argc != 4) {
        MessageBox(NULL, TEXT("argc != 4"), NULL, MB_OK);
        exit(1);
    }
    ReInitShapes(argv[1]);
    ReInitParams(argv[2]);
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(1, 1);
    if(WSAStartup(wVersionRequested, &wsaData) != 0) {
        printf("socket startup error.\n");
        return 1;
    }
    if(LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
        printf("socket version error.\n");
        WSACleanup();
        return 1;
    }
    printf("port = %d\n", atoi(argv[3]));
    SOCKET sockSrv;
    while((sockSrv = socket(AF_INET, SOCK_STREAM, 0)) < 0) {}
    SOCKADDR_IN addrSrv;
    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons((u_short)atoi(argv[3]));
    while(bind(sockSrv, (SOCKADDR *)(&addrSrv), sizeof(SOCKADDR)) < 0) {}
    while(listen(sockSrv, 1) < 0) {}
    SOCKADDR_IN addrClient;
    while(true) {
        RequestDataStruct r;
        int len = sizeof(addrClient);
        SOCKET sockConn;
        while((sockConn = accept(sockSrv, (SOCKADDR *)(&addrClient), &len)) < 0) {}
        int recvLen = 0;
        do {
            recvLen += recv(sockConn, (char *)(&r) + recvLen, sizeof(RequestDataStruct) - recvLen, 0);
        } while(recvLen < sizeof(RequestDataStruct));
        if(r._type == RequestDataStruct::TYPE_EXIT) {
            closesocket(sockConn);
            printf("Exit\n");
            break;
        }
        else if(r._type == RequestDataStruct::TYPE_PARAM) {
            printf("Parameter, not implemented\n");
            //FOR_EACH(i, evaluator._shapeSize) {
            //    printf("%2d: %d\n", i, r._data._param[i]);
            //    evaluator._shape[SHAPE_TABLE_NO_BLACK][i]._value = r._data._param[i];
            //    evaluator._shape[SHAPE_TABLE_NO_WHITE][i]._value = r._data._param[i];
            //}
        } else { // r._type == RequestData::TYPE_SEARCH
            printf("Search\n");
            clock_t cl = clock();
            searcher.SearchGoodMoves(r._data._searchData._board, r._data._searchData._isBlack, r._data._searchData._moves);
            PrintBoard(r._data._searchData._board);
            ResponseDataStruct m = ResponseDataStruct(
                searcher.GetDMove()._r1, searcher.GetDMove()._c1,
                searcher.GetDMove()._r2, searcher.GetDMove()._c2);
            if(r._data._searchData._moves == 2) {
                printf("(%02d, %c), (%02d, %c)\n", m._r1, m._c1 + 'A', m._r2, m._c2 + 'A');
            } else {
                printf("(%02d, %c)\n", m._r1, m._c1 + 'A');
            }
            int d = clock() - cl;
            printf("%d.%03d seconds\n", d / 1000, d % 1000);
            //printf("%d evaluated nodes\n", evaluatedNodes);
            //printf("%d DTSS nodes\n", dtssNodes);
            printf("%d evaluations\n", evaluator._evaluations);
            printf("%d evaluator hits\n", evaluator._transTable._hits);
            printf("%d nodes\n", frame._nodes);
            printf("%d hits\n", frame._transTableBlack._hits + frame._transTableWinte._hits);
            printf("%d dtss nodes\n", dtsser._nodes);
            printf("%d dtss hits\n", dtsser._transTableBlack._hits + dtsser._transTableWhite._hits);
            printf("%d dtss dropped searches\n", dtsser._dropedSearches);
            printf("%d idtss successes\n", dtsser._idtssSuccesses);
            send(sockConn, (char *)(&m), sizeof(ResponseDataStruct), 0);
        }
        closesocket(sockConn);
    }
    closesocket(sockSrv);
    WSACleanup();
    return 0;
}
