void clearTable(myTable *tp);
void realocTable(myTable* tp, int newSize);
void addToTable(myTable* tp, char* newElem);
void addToStr(char** str, char ch, int* len, int* cap);
void deleteSpaces(char** str, int len);
char* getFuncInter(FILE* fp, char c);
myTable* getTableFromFile(FILE* fp);
void printTable(myTable *tp);
void generateHeader(myTable *tp, FILE *fp);
