#include <bits/stdc++.h>
using namespace std;
int main(){
	FILE* pt = fopen("HistoryBMode.txt", "w");
	FILE* ptt = fopen("HistoryBTTime.txt", "w"); 
	FILE* pttt = fopen("HistoryMode.txt", "w");
	FILE* ptttt = fopen("HistoryTime.txt", "w");
	for (int i = 1; i <= 100; i++){
		fprintf(pt, " %lf", 10000000000.00000);
		fprintf(ptt, " %d", 0);
	}
	fprintf(pttt, "\n%d", 0);
	fprintf(ptttt, "\n%d", 0);
	fclose(pt);
}

