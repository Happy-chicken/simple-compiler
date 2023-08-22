#include "lex.hpp"
#include "direct.h"
using namespace std;
using namespace tabulate;
tabulate::Table lex_table;
string keyword[37] = {"auto", "double", "int", "struct", "break", "else", "long", "switch", "case",
                      "void", "main", "register", "enum", "typedef", "char", "extern", "union",
                      "const", "for", "while", "float", "short", "unsigned", "continue", "signed",
                      "defalut", "goto", "sizeof", "do", "static", "if", "return", "cout", "cin",
                      "include", "iostream"};

lexical_analysis::lexical_analysis(string filepath)
{
    this->file = ifstream(filepath, ios_base::binary | ios_base::in);
}
vector<Token_info* > lexical_analysis::getTokens()
{
    return Tokens;
}
char lexical_analysis::transfer(TypeCode code)
{
    switch (code)
    {
    case 2:
        return 'z'; // double->z
        break;
    case 3:
        return 'h'; // int->h
        break;
    case 5:
        return 'b'; // break->b
        break;
    case 6:
        return 'e'; // else->e
        break;
    case 7:
        return 'l'; // long->l
        break;
    case 8:
        return 's'; // switch->s
        break;
    case 9:
        return 'c'; // case->c
        break;
    case 10:
        return 'v'; // void->v
        break;
    case 11:
        return 'm'; // main->m
        break;
    case 14:
        return 't'; // typedef->t
        break;
    case 15:
        return 'a'; // char->a
        break;
    case 19:
        return 'f'; // for->f
        break;
    case 20:
        return 'w'; // while->w
        break;
    case 21:
        return 'y'; // float->y
        break;
    case 23:
        return 'u'; // unsigned->u
        break;
    case 24:
        return 'o'; // continue->o
        break;
    case 26:
        return 'n'; // default->n
        break;
    case 29:
        return 'd'; // do->d
        break;
    case 31:
        return 'i'; // if->i
        break;
    case 32:
        return 'r'; // return->r
        break;
    case 33:
        return 'g'; // cout->g
        break;
    case 35:
        return 'p'; // include->p
        break;
    case 36:
        return 'k'; // iostream->k
        break;
    default:
        break;
    }
}
bool lexical_analysis::isKeyWord(string token) //判断是否是关键词
{
    for (int i = 0; i < 37; i++)
    {
        if (token == keyword[i])
        {
            return true;
        }
    }
    return false;
}
int lexical_analysis::getKeyWordID(string token) //得到关键词的种类值
{
    for (int i = 0; i < 37; i++)
    {
        if (token == keyword[i])
        {
            return i + 1;
        }
    }
    return -1;
}

void lexical_analysis::print(string &msg, int token_num)
{

    switch (Tokens[token_num]->code)
    {
    /*未识别的符号*/
    case TK_UNDEF:
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED); //未识别的符号为红色

        cout << "(" << Tokens[token_num]->row << "):[ " << Tokens[token_num]->code << " , " << Tokens[token_num]->token << " ]"
             << "   Lexical Error: At Line " << Tokens[token_num]->row << " Col " << Tokens[token_num]->col << endl;
        msg += "(" + to_string(Tokens[token_num]->row) + "):[ " + to_string(Tokens[token_num]->code) + " , " + Tokens[token_num]->token + " ]" + '\n';
        return;
        break;
    }
    case ANNOTATION:
    {
        break;
    }
    default:
        msg += "(" + to_string(Tokens[token_num]->row) + "):[ " + to_string(Tokens[token_num]->code) + " , " + Tokens[token_num]->token + " ]" + '\n';
        break;
    }
}
void lexical_analysis::getToken()
{
    file.clear();
    file.seekg(0, ios::beg);
    char ch;      //用于存储从文件中获取的单个字符
    int each_col; //用于存储每行的列数
    TypeCode code;
    string rowbuffer;
    string msg;
    string token;//暂存词法分析的token
    int row = 1, col = 1;//store row and col
    int token_num = 0;//记录第几个token information
    while (file.get(ch)) //未读取到文件尾，从文件中获取一个字符
    {
        rowbuffer.push_back(ch);
        token = ch;                  //将获取的字符存入token中
        if (ch == ' ' || ch == '\t') //忽略空格、Tab
        {
            rowbuffer.pop_back();
            continue;
        }
        else if (ch == '\r' && (ch = file.get()) == '\n') //忽略回车
        {

            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            string orignal_info = "line " + to_string(row) + ':' + rowbuffer;
            lex_table.add_row({orignal_info});
            lex_table.add_row({msg});
            row++;
            each_col = file.tellg();
            rowbuffer.clear();
            msg.clear();
            continue;
        }
        else if (isalpha(ch) || ch == '_') //以字母或下划线开头，关键字或标识符
        {
            token.clear();                                  // token初始化
            while (isalpha(ch) || isdigit(ch) || ch == '_') //非字母或数字或下划线时退出，将单词存储在token中
            {

                token.push_back(ch); //将读取的字符ch存入token中
                ch = file.get();     //获取下一个字符
                rowbuffer.push_back(ch);
            }
            rowbuffer.pop_back();
            file.seekg(-1, ios::cur);
            if (isKeyWord(token)) //关键字
                code = TypeCode(getKeyWordID(token));
            else                 //标识符
                code = TK_IDENT; //单词为标识符
        }
        else if (ch == '.')
        {                  //.digit
            token.clear(); // token初始化
            token.push_back(ch);
            if (isdigit(ch = file.get()))
            {
                bool issci = false; //标记是否含E/e
                while (isdigit(ch))
                {
                    token.push_back(ch);
                    rowbuffer.push_back(ch);
                    ch = file.get();
                    if ((ch == 'E' || ch == 'e') && !issci)
                    {
                        if (isdigit(file.peek()))
                        {
                            issci = true;
                            token.push_back(ch); //放入E/e
                            rowbuffer.push_back(ch);
                            ch = file.get();
                        }
                        else if ((file.peek()) == '+' || (file.peek()) == '-')
                        {
                            token.push_back(ch); //放入E/e
                            rowbuffer.push_back(ch);
                            ch = file.get();
                            if (isdigit(file.peek()))
                            {
                                token.push_back(ch); //放入+/-
                                rowbuffer.push_back(ch);
                                ch = file.get();
                            }
                        }
                    }
                }
                code = TK_FLOAT;
                file.seekg(-1, ios::cur);
            }
            else
            {
                file.seekg(-1, ios::cur);
                code = TK_DOT;
            }
        }
        else if (isdigit(ch)) //无符号数
        {
            bool isfloat = false; //标记是否为小数
            bool issci = false;   //标记是否为科学计数法
            token.clear();        // token初始化
            rowbuffer.pop_back();
            while (isdigit(ch)) //字符为数字
            {
                token.push_back(ch);
                rowbuffer.push_back(ch);
                ch = file.get();
                if (ch == '.' && isfloat == false)
                {
                    // 小数点下一位是数字
                    if (isdigit(file.peek()))
                    {
                        isfloat = true;
                        rowbuffer.push_back(ch);
                        token.push_back(ch); //将小数点入token中
                        ch = file.get();     //读取小数点后的下一位数字
                    }
                }
                else if ((ch == 'E' || ch == 'e') && issci == false)
                {

                    if (isdigit(file.peek()))
                    {
                        issci = true;
                        token.push_back(ch); //放入E/e
                        rowbuffer.push_back(ch);
                        ch = file.get();
                    }
                    else if ((file.peek()) == '+' || (file.peek()) == '-')
                    {
                        issci = true;
                        token.push_back(ch); //放入E/e
                        rowbuffer.push_back(ch);
                        ch = file.get();
                        if (isdigit(file.peek()))
                        {
                            token.push_back(ch); //放入+/-
                            rowbuffer.push_back(ch);
                            ch = file.get();
                        }
                    }
                }
            }
            if (isfloat || issci)
                code = TK_FLOAT; // float
            else
                code = TK_INT; // intrger
            // rowbuffer.pop_back();
            file.seekg(-1, ios::cur);
        }
        else
            switch (ch)
            {
            /*字符串*/
            case '"':
            {
                code = TK_STRING;
                while ((ch = file.get()) != '"')
                {
                    if (ch == '\r' && (ch = file.get()) == '\n')
                    {
                        row++;
                        code = TK_UNDEF;
                        col = int(file.tellg()) - each_col;
                        continue;
                    }
                    if (ch == '\\' && (ch = file.get()) == '\r')
                    {
                        file.seekg(1, ios::cur);
                        row++;
                        continue;
                    }
                    rowbuffer.push_back(ch);
                    token.push_back(ch);
                }
                rowbuffer.push_back(ch);
                token.push_back(ch);
                break;
            }
            /*字符常量*/
            case '\'':
            {
                ch = file.get();
                rowbuffer.push_back(ch);
                token.push_back(ch);
                if ((ch = file.get()) == '\'')
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch);
                    code = TK_CHARCONST;
                }
                else
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch);
                    while ((ch = file.get()) != '\'')
                    {
                        rowbuffer.push_back(ch);
                        token.push_back(ch);
                    }
                    rowbuffer.push_back(ch);
                    token.push_back(ch);
                    code = TK_UNDEF;
                    col = int(file.tellg()) - each_col;
                }
                break;
            }
            /*运算符*/
            case '!': // logical not
            {
                ch = file.get();
                if (ch == '=')
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch);
                    code = TK_NOTEQ;
                }
                else
                {
                    file.seekg(-1, ios::cur);
                    code = TK_NOT;
                }
                break;
            }
            case '|':
            {
                ch = file.get();
                if (ch = '|') // or
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch);
                    code = TK_OR;
                }
                else // logical or
                {
                    code = TK_BITWISEOR;
                    file.seekg(-1, ios::cur); //将超前读取的字符重新读取
                }
                break;
            }
            case '&':
            {
                ch = file.get();
                if (ch = '&') // and
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch);
                    code = TK_AND;
                }
                else // logical and
                {
                    code = TK_BITWISEAND;
                    file.seekg(-1, ios::cur); //将超前读取的字符重新读取
                }
                break;
            }
            case '^':
            { // bitwise xor
                code = TK_BITWISEXOR;
                break;
            }
            case '~':
            { // bitwise ont
                code = TK_BITWISENOT;
                break;
            }
            case '+':
            {
                ch = file.get();
                if (ch == '=') //+=
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch);
                    code = TK_ACIN;
                }
                else if (ch == '+')
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch); //单词为"++"
                    code = TK_AIN;
                }
                else
                {
                    code = TK_PLUS;           //+加号
                    file.seekg(-1, ios::cur); //将超前读取的字符重新读取
                }
            }
            break;
            case '-':
            {
                ch = file.get();
                if (ch == '=') //-=
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch);
                    code = TK_ACDEIN; //单词为"-="
                }
                else if (ch == '-')
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch);
                    code = TK_ADEIN; //单词为"--"
                }
                else if (ch == '>')
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch);
                    code = TK_POINT; //单词为"->"
                }
                else
                {
                    code = TK_MINUS;          //-减号
                    file.seekg(-1, ios::cur); //将超前读取的字符重新读取
                }
            }
            break;
            case '*':
            {
                ch = file.get();
                if (ch == '=')
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch);
                    code = TK_AUTOMUL; //单词为"*="
                }
                else
                {
                    file.seekg(-1, ios::cur);
                    code = TK_STAR; //*乘号
                }

                break;
            }
            case '/':
            {
                ch = file.get();
                if (ch == '/') //删除单行注释
                {
                    token.clear();
                    code = ANNOTATION;
                    while (ch != '\n')
                    {
                        rowbuffer.push_back(ch);
                        ch = file.get();
                        if (ch == '\r' && file.peek() == '\n')
                        {
                            string orignal_info = "line " + to_string(row) + ':' + rowbuffer;

                            lex_table.add_row({orignal_info});
                            msg.clear();
                            row++;
                        }
                    }
                    rowbuffer.clear();
                }
                else if (ch == '*') //删除多行注释
                {
                    token.clear();
                    code = ANNOTATION;
                    rowbuffer.push_back(ch);
                    while (ch = file.get())
                    {
                        // ch = file.get();
                        if (ch != '\r' || ch != '\n')
                        {
                            rowbuffer.push_back(ch);
                        }
                        if (ch == '*' && (ch = file.get()) == '/')
                        {
                            rowbuffer.push_back(ch);
                            break;
                        }
                        if (ch == '\r' && (ch = file.get()) == '\n') //计算多行注释的行数
                        {
                            string orignal_info = "line " + to_string(row) + ':' + rowbuffer;
                            lex_table.add_row({orignal_info});
                            row++;
                            rowbuffer.clear();
                            continue;
                        }
                    }
                }
                else if (ch == '=')
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch);
                    code = TK_AUTODIV; //单词为"/="
                }
                else
                {
                    file.seekg(-1, ios::cur);
                    code = TK_DIVIDE; //除号
                }
                break;
            }
            case '=':
            {
                ch = file.get(); //超前读取'='后面的字符
                if (ch == '=')   //==等于号
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch); //将'='后面的'='存入token中
                    code = TK_EQ;
                }
                else
                { //=赋值运算符
                    code = TK_ASSIGN;
                    file.seekg(-1, ios::cur); //将超前读取的字符重新读取
                }
            }
            break;
            case '<':
            {
                ch = file.get(); //超前读取'<'后面的字符
                if (ch == '=')   //<=小于等于号
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch); //将'<'后面的'='存入token中
                    code = TK_LEQ;
                }
                else if (ch == '<') // left shift
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch);
                    code = TK_OSTREAM;
                }
                else
                { //<小于号
                    code = TK_LT;
                    file.seekg(-1, ios::cur); //将超前读取的字符重新读取
                }
            }
            break;
            case '>':
            {
                ch = file.get(); //超前读取'>'后面的字符
                if (ch == '=')
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch); //将'>'后面的'='存入token中
                    code = TK_GEQ;
                }
                else if (ch == '>') // right shift
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch);
                    code = TK_ISTREAM;
                }
                else
                { //>大于号
                    code = TK_GT;
                    file.seekg(-1, ios::cur); //将超前读取的字符重新读取
                }
            }
            break;
            /*分界符*/
            case '(':
                code = TK_OPENPA; //(左圆括号
                break;
            case ')':
                code = TK_CLOSEPA; //)右圆括号
                break;
            case '[':
                code = TK_OPENBR; //[左中括号
                break;
            case ']':
                code = TK_CLOSEBR; //]右中括号
                break;
            case '{':
                code = TK_BEGIN; //{左大括号
                break;
            case '}':
                code = TK_END; //}右大括号
                break;
            case ',':
                code = TK_COMMA; //,逗号
                break;
            case ';':
                code = TK_SEMOCOLOM; //;分号
                break;
            // case '"':
            //     code = TK_DOUBLEQUTA; //"双引号
            //     break;
            // case '\'':
            //     code = TK_SINGLEQUTA; //'单引号
            //#
            case '#':
                code = TK_WELL;
                break;
            //未识别符号
            default:
                code = TK_UNDEF;
                col = int(file.tellg()) - each_col;
            }
        Tokens.push_back(new Token_info(code, token, row, col));
        print(msg, token_num);
        saveToken(token_num);
        token_num++;
    }
}
void lexical_analysis::saveToken(int token_num)
{
    char BUFFER[100];
    getcwd(BUFFER, 100);
    string exe_path = BUFFER;
    ofstream ofile(exe_path + "\\dependency\\result_lexical_analysis.txt", ios_base::out | ios_base::binary | ios_base::app);
    TypeCode code = Tokens[token_num]->code;
    if (code >= 1 && code <= 36) // keyword
    {
        ofile << transfer(code);
        simplified_string += to_string(transfer(code));
    }
    else if(code >= 57 && code <= 62) //双字符运算符
    {
        ofile << code - 53;
    }
    else if ((code >= 65 && code <= 75) || (code >= 37 && code <= 49)) //界符
    {
        ofile << Tokens[token_num]->token;
        simplified_string += Tokens[token_num]->token;
    }
    else if (code == 78 || code == 79) //常量
    {
        ofile << 3;
        simplified_string += to_string(3);
    }
    else if (code == 80) //标识符
    {
        ofile << 2;
        simplified_string += to_string(2);
    }
}
void lexical_analysis::showcode() //显示源代码
{

    string eachline;
    int linenum = 1;
    cout << "Your codes are as follows:" << endl
         << endl;
    while (getline(file, eachline, '\n'))
    {
        cout << '(' << linenum << ')' << ' ' << eachline << endl;
        linenum++;
    }
}
