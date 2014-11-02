/*
Oj742
StackStacks Interpreter
*/

#include <fstream>
#include <iostream>
#include <cstring>
#include <cctype>

typedef unsigned long long qword;

const int NUMCMD = 72;
typedef void* scode;
const char cmd[NUMCMD] = {};

#define SC_PUSH     ((scode)(cmd+0))
#define SC_SIZE     ((scode)(cmd+1))
#define SC_LEVEL    ((scode)(cmd+2))
#define SC_POP      ((scode)(cmd+3))
#define SC_NIP      ((scode)(cmd+4))
#define SC_TUCK     ((scode)(cmd+5))
#define SC_CLEAR    ((scode)(cmd+6))
#define SC_DUP      ((scode)(cmd+7))
#define SC_CDUP     ((scode)(cmd+8))
#define SC_SWAP     ((scode)(cmd+9))
#define SC_INC      ((scode)(cmd+10))
#define SC_DEC      ((scode)(cmd+11))
#define SC_PACK     ((scode)(cmd+12))
#define SC_UNPACK   ((scode)(cmd+13))
#define SC_GETFAIL  ((scode)(cmd+14))
#define SC_GETTEST  ((scode)(cmd+15))
#define SC_OUTI     ((scode)(cmd+16))
#define SC_OUTC     ((scode)(cmd+17))
#define SC_OUTA     ((scode)(cmd+18))
#define SC_OUTS     ((scode)(cmd+19))
#define SC_GETS     ((scode)(cmd+20))
#define SC_GETA     ((scode)(cmd+21))
#define SC_DUP2     ((scode)(cmd+22))
#define SC_SWAP2    ((scode)(cmd+23))
#define SC_OVER     ((scode)(cmd+24))
#define SC_ROVER    ((scode)(cmd+25))
#define SC_COVER    ((scode)(cmd+26))
#define SC_RCOVER   ((scode)(cmd+27))
#define SC_OVER2    ((scode)(cmd+28))
#define SC_ROVER2   ((scode)(cmd+29))
#define SC_ROT      ((scode)(cmd+30))
#define SC_RROT     ((scode)(cmd+31))
#define SC_CYCLE    ((scode)(cmd+32))
#define SC_RCYCLE   ((scode)(cmd+33))
#define SC_ADD      ((scode)(cmd+34))
#define SC_TAKE     ((scode)(cmd+35))
#define SC_CAT      ((scode)(cmd+36))
#define SC_MUP      ((scode)(cmd+37))
#define SC_MDOWN    ((scode)(cmd+38))
#define SC_MROOT    ((scode)(cmd+39))
#define SC_MGOTO    ((scode)(cmd+40))
#define SC_MLEAF    ((scode)(cmd+41))
#define SC_NOT      ((scode)(cmd+42))
#define SC_EQ       ((scode)(cmd+43))
#define SC_LS       ((scode)(cmd+44))
#define SC_GRT      ((scode)(cmd+45))
#define SC_NEQ      ((scode)(cmd+46))
#define SC_LSEQ     ((scode)(cmd+47))
#define SC_GRTEQ    ((scode)(cmd+48))
#define SC_OR       ((scode)(cmd+49))
#define SC_AND      ((scode)(cmd+50))
#define SC_XOR      ((scode)(cmd+51))
#define SC_TEST     ((scode)(cmd+52))
#define SC_SKIP     ((scode)(cmd+53))
#define SC_DO       ((scode)(cmd+54))
#define SC_EXIT     ((scode)(cmd+55))
#define SC_LOOP     ((scode)(cmd+56))
#define SC_RPACK    ((scode)(cmd+57))
#define SC_GETLEAF  ((scode)(cmd+58))
#define SC_END      ((scode)(cmd+59))
#define SC_POP2     ((scode)(cmd+60))
#define SC_XCHG     ((scode)(cmd+61))
#define SC_SHFTL    ((scode)(cmd+62))
#define SC_SHFTR    ((scode)(cmd+63))
#define SC_RESIZE   ((scode)(cmd+64))
#define SC_ENDL     ((scode)(cmd+65))
#define SC_DEBUG    ((scode)(cmd+66))
#define SC_DEBUGE   ((scode)(cmd+67))
#define SC_DEBUGA   ((scode)(cmd+68))
#define SC_DEBUGC   ((scode)(cmd+69))
#define SC_DEBUGX   ((scode)(cmd+70))

#define SC_IDLAST   ((scode)(cmd+NUMCMD-1))


#define SC_NUM      0
#define SC_STRING   1
#define SC_CALL     2
#define SC_BLOCK    3
#define SC_STRINGX  4
#define SC_NOP      5

using namespace std;

const unsigned BSIZE = 257;





struct xcode
{
    char i;
    union{
        struct block * blk;
        qword num;
        char * str;
    };
};

struct call_stack
{
    struct block * current;
    unsigned long i; //code index
    call_stack * prev;
};


struct block
{
    char * name;
    scode * c;
    long size;
    block * next;
    ~block();
};

class strtree //a tree for all the strings, so we don't waste memory and can easily compare strings
{
public:
    strtree();
    strtree(char *);
    ~strtree();
    char* add(char *);
private:
    char * str;
    strtree * left;
    strtree * right;
};

class memstore //for getting the code into nice, compact arrays
{
public:
    memstore();
    ~memstore();
    void add(scode to_add);
    scode* getarray();
    qword getsize();
private:
    struct node{
        scode data;
        node * next;
    }*head, *tail;
    qword size;
};

char* getword(ifstream & file);
qword strtonum(char* source);
char* compilestr(char* source);
char* copystr(char* source);
bool isnum(char * source);
char* anonfunc(char * parent);
//void decode(scode c);
