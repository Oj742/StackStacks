/*
Oj742
StackStacks Interpreter

Okay, this is a mess.
There's hardly any comments and the declarations and implementations are haphazzard at best.
The code is crude, and generally based on trial and error.
I'll clean it up eventually.
*/
#include "Code.h"


char* getword(ifstream & file)
{
    static char buffer[BSIZE];
    char c;
    int i = 0;
    while(isspace(c = file.get()) && !file.eof());
    *buffer = c;
    if(file.eof()) *buffer = '\0';
    else if(c == '/' && file.peek() == '/'){
        *buffer = '\0';
        while(c != '\n') c = file.get();
    } else if(c == '/' && file.peek() == '*'){
        *buffer = '\0';
        file.ignore();
        while(c != '*' && file.peek() != '/') c = file.get();
        file.ignore(); //skip past that last '/'
    }else if(c == '\"'){
       do{
            ++i;
            buffer[i] = file.get();
            if(buffer[i] == '\\') buffer[++i] = file.get();
        }while(buffer[i] != '\"');
        ++i;
    }else if(c == '\''){
       do{
            ++i;
            buffer[i] = file.get();
            if(buffer[i] == '\\') buffer[++i] = file.get();
        }while(buffer[i] != '\'');
        ++i;
    }else if(c == '{'){//treat these as operators; they always stand alone
        i = 1;
    }else if(c == '}'){
        i = 1;
    }else{
        do{
            ++i;
            buffer[i] = file.get();
        }while(!isspace(buffer[i]));
    }
    buffer[i] = '\0';
    //cout << "Word: " << buffer << endl;
    return buffer;
}

qword strtonum(char * source)
{
    qword num = 0;
    while(*source){
        num *= 10;
        num += *source - '0';
        ++source;
    }
    return num;
}

char * copystr(char * source)
{
    char * copy = new char [strlen(source)+1];

    strcpy(copy, source);
    return copy;
}

bool isnum(char * source)
{
    while(*source){
        if(!isdigit(*source)) return false;
        ++source;
    }
    return true;
}


char* compilestr(char * source)
{
    static char out[BSIZE];
    int i = 0;
    if(*source == '\"' || *source == '\'') ++source;
    while(*source != '\"' && *source != '\'' && *source != '\0'){
        if(*source == '\\'){
            switch(*(++source)){
                case 'n': out[i] = '\n'; break;
                case 't': out[i] = '\t'; break;
                case 'b': out[i] = '\b'; break;
                case 'r': out[i] = '\r'; break;
                case '0': out[i] = '\0'; break;
                default: out[i] = *source;
            }
        } else out[i] = *source;
        ++i;
        ++source;
    }
    out[i] = '\0';
    return out;
}


char* anonfunc(char * parent)
{
    static char ret[BSIZE];
    ret[0] = '{';
    strcpy(ret+1, parent);
    ret[strlen(ret)+1] = '\0';
    ret[strlen(ret)] = '}';
    return ret;
}



memstore::memstore()
{
    head = NULL;
    tail = NULL;
    size = 0;
}

memstore::~memstore()
{
    while(head)
    {
        tail = head->next;
        delete head;
        head = tail;
    }
}

void memstore::add(scode to_add)
{
    if(!head) {
        head = new node;
        head->data = to_add;
        tail = head;
    } else {
        tail->next = new node;
        tail = tail->next;
        tail->data = to_add;
    }
    ++size;
    tail->next = NULL;
}

qword memstore::getsize()
{
    return size;
}

scode* memstore::getarray()
{
    if(!size) return NULL;
    scode * temp = new scode [size];
    node * trav = head;
    qword i = 0;

    while(i < size){
        temp[i] = trav->data;
        trav = trav->next;
        ++i;
    }
    return temp;

}

strtree::strtree()
{
    str = NULL;
    left = NULL;
    right = NULL;
}

strtree::~strtree()
{
    if(str) delete[] str;
    if(left) delete left;
    if(right) delete right;
}

char* strtree::add(char * s)
{
    if(!str) return str = copystr(s);
    if(strcmp(str, s) < 0)
    {
        if(!left) left = new strtree();
        return left->add(s);
    }
    if(strcmp(str, s) > 0){
        if(!right) right = new strtree();
        return right->add(s);
    }
    return str;
}

block::~block()
{
    delete[] c;
    delete next;
}
