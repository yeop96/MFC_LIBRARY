// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

 //#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"

// MySQL 헤더
#include <mysql.h>
#include <WinSock2.h>
#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"ws2_32.lib")

// DB연결 초기화 헤더
#define MY_IP "localhost"
#define DB_USER "root"
#define DB_PASS "0000"
#define DB_NAME "javadb"

extern MYSQL_ROW Sql_Row; // mysql 의 각행의 인덱스값들 불러온다.
extern MYSQL_RES *Sql_Result; // mysql의 결과
extern MYSQL Connect; // mysql의 연결을 담당
extern CString id_static;//로그인 아이디값

#endif //PCH_H
