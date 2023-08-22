#ifndef LEX_H
#define LEX_H
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <windows.h>
#include "tabulate/table.hpp"
using namespace std;
extern tabulate::Table lex_table;
extern string keyword[];
enum TypeCode
{
    TK_UNDEF = 0, // undefined
    /*������*/
    KW_AUTO,     // auto
    KW_DOUBLE,   // double
    KW_INT,      // int
    KW_STRUCT,   // struct
    KW_BREAK,    // break
    KW_ELSE,     // else
    KW_LONG,     // long
    KW_SWITCH,   // switch
    KW_CASE,     // case
    KW_VOID,     // void
    KW_MAIN,     // main
    KW_REGISTER, // register
    KW_ENUM,     // enum
    KW_TYPEDEF,  // typedef
    KW_CHAR,     // char
    KW_EXTERN,   // extern
    KW_UNION,    // union
    KW_CONST,    // const
    KW_FOR,      // for
    KW_WHILE,    // while
    KW_FLOAT,    // float
    KW_SHORT,    // short
    KW_UNSIGNED, // unsigned
    KW_CONTINUE, // continue
    KW_SIGNED,   // signed
    KW_DEFALUT,  // defalut
    KW_GOTO,     // goto
    KW_SIZEOF,   // sizeof
    KW_DO,       // do
    KW_STATIC,   // static
    KW_IF,       // if
    KW_RETURN,   // return

    KW_COUT,     // cout
    KW_CIN,      // cin
    KW_INCLUDE,  // include
    KW_IOSTREAM, // iostream
    /* ����� */
    //���ַ�
    TK_NOT,        //!

    
    TK_BITWISEOR,  //|
    TK_BITWISEAND, //&
    TK_BITWISENOT, //~
    TK_BITWISEXOR, //^
    TK_PLUS,       //+

    
    TK_MINUS,      //-
    
    
    
    TK_STAR,       //*
    
    TK_DIVIDE,     ///
    TK_ASSIGN,     //=
   
    
    TK_LT,         //<
    
    TK_GT,         //>
    
    TK_DOT,        //.
    //˫�ַ�

    TK_AIN,        //++
    TK_ACIN,       //+=
    TK_ADEIN,      //--
    TK_POINT,      //->
    TK_ACDEIN,     //-=
    TK_AUTOMUL,    //*=
    TK_AUTODIV,    ///=
    TK_NOTEQ,      //!=
    TK_OR,         //||
    TK_AND,        //&&
    TK_EQ,         //==
    TK_LEQ,        //<=
    TK_GEQ,        //>=
    TK_OSTREAM, //<<
    TK_ISTREAM, //>>
    /*���*/
    TK_OPENPA,     //(
    TK_CLOSEPA,    //)
    TK_OPENBR,     //[
    TK_CLOSEBR,    //]
    TK_BEGIN,      //{
    TK_END,        //}
    TK_COMMA,      //,
    TK_SEMOCOLOM,  //;
    TK_DOUBLEQUTA, //"
    TK_SINGLEQUTA, //'

    TK_WELL, //#
    /*�ַ���*/
    TK_STRING,    // string
    TK_CHARCONST, //�ַ�����
    /*��ֵ�ͳ���*/
    TK_INT,   // integer
    TK_FLOAT, // float
    /*��ʶ��*/
    TK_IDENT, // identy
    /*ע��*/
    ANNOTATION // annotation
};

struct Token_info
{
    TypeCode code = TK_UNDEF;
    string token = "";
    int row = 1;
    int col = 1;
    Token_info() = default;
    Token_info(TypeCode code, string token, int row, int col):code(code), token(token), row(row), col(col){}
    ~Token_info() = default;
};

class lexical_analysis
{
private:
    vector<Token_info* > Tokens;//vector store all tokens' information
    string simplified_string;//result of lexical analysis, same as reslut_lexical_analysis.txt
    ifstream file;

public:
    lexical_analysis(string filepath); //��ȱʡֵ�Ĺ��캯��
    bool isKeyWord(string token);
    int getKeyWordID(string token);
    void getToken();
    void print(string &msg, int token_num);
    void showcode();
    void saveToken(int token_num);
    char transfer(TypeCode code); //���ֱ���ӳ�䵽һ���ַ�
    vector<Token_info* > getTokens();
};
#endif 
