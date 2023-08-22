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
bool lexical_analysis::isKeyWord(string token) //�ж��Ƿ��ǹؼ���
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
int lexical_analysis::getKeyWordID(string token) //�õ��ؼ��ʵ�����ֵ
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
    /*δʶ��ķ���*/
    case TK_UNDEF:
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED); //δʶ��ķ���Ϊ��ɫ

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
    char ch;      //���ڴ洢���ļ��л�ȡ�ĵ����ַ�
    int each_col; //���ڴ洢ÿ�е�����
    TypeCode code;
    string rowbuffer;
    string msg;
    string token;//�ݴ�ʷ�������token
    int row = 1, col = 1;//store row and col
    int token_num = 0;//��¼�ڼ���token information
    while (file.get(ch)) //δ��ȡ���ļ�β�����ļ��л�ȡһ���ַ�
    {
        rowbuffer.push_back(ch);
        token = ch;                  //����ȡ���ַ�����token��
        if (ch == ' ' || ch == '\t') //���Կո�Tab
        {
            rowbuffer.pop_back();
            continue;
        }
        else if (ch == '\r' && (ch = file.get()) == '\n') //���Իس�
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
        else if (isalpha(ch) || ch == '_') //����ĸ���»��߿�ͷ���ؼ��ֻ��ʶ��
        {
            token.clear();                                  // token��ʼ��
            while (isalpha(ch) || isdigit(ch) || ch == '_') //����ĸ�����ֻ��»���ʱ�˳��������ʴ洢��token��
            {

                token.push_back(ch); //����ȡ���ַ�ch����token��
                ch = file.get();     //��ȡ��һ���ַ�
                rowbuffer.push_back(ch);
            }
            rowbuffer.pop_back();
            file.seekg(-1, ios::cur);
            if (isKeyWord(token)) //�ؼ���
                code = TypeCode(getKeyWordID(token));
            else                 //��ʶ��
                code = TK_IDENT; //����Ϊ��ʶ��
        }
        else if (ch == '.')
        {                  //.digit
            token.clear(); // token��ʼ��
            token.push_back(ch);
            if (isdigit(ch = file.get()))
            {
                bool issci = false; //����Ƿ�E/e
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
                            token.push_back(ch); //����E/e
                            rowbuffer.push_back(ch);
                            ch = file.get();
                        }
                        else if ((file.peek()) == '+' || (file.peek()) == '-')
                        {
                            token.push_back(ch); //����E/e
                            rowbuffer.push_back(ch);
                            ch = file.get();
                            if (isdigit(file.peek()))
                            {
                                token.push_back(ch); //����+/-
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
        else if (isdigit(ch)) //�޷�����
        {
            bool isfloat = false; //����Ƿ�ΪС��
            bool issci = false;   //����Ƿ�Ϊ��ѧ������
            token.clear();        // token��ʼ��
            rowbuffer.pop_back();
            while (isdigit(ch)) //�ַ�Ϊ����
            {
                token.push_back(ch);
                rowbuffer.push_back(ch);
                ch = file.get();
                if (ch == '.' && isfloat == false)
                {
                    // С������һλ������
                    if (isdigit(file.peek()))
                    {
                        isfloat = true;
                        rowbuffer.push_back(ch);
                        token.push_back(ch); //��С������token��
                        ch = file.get();     //��ȡС��������һλ����
                    }
                }
                else if ((ch == 'E' || ch == 'e') && issci == false)
                {

                    if (isdigit(file.peek()))
                    {
                        issci = true;
                        token.push_back(ch); //����E/e
                        rowbuffer.push_back(ch);
                        ch = file.get();
                    }
                    else if ((file.peek()) == '+' || (file.peek()) == '-')
                    {
                        issci = true;
                        token.push_back(ch); //����E/e
                        rowbuffer.push_back(ch);
                        ch = file.get();
                        if (isdigit(file.peek()))
                        {
                            token.push_back(ch); //����+/-
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
            /*�ַ���*/
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
            /*�ַ�����*/
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
            /*�����*/
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
                    file.seekg(-1, ios::cur); //����ǰ��ȡ���ַ����¶�ȡ
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
                    file.seekg(-1, ios::cur); //����ǰ��ȡ���ַ����¶�ȡ
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
                    token.push_back(ch); //����Ϊ"++"
                    code = TK_AIN;
                }
                else
                {
                    code = TK_PLUS;           //+�Ӻ�
                    file.seekg(-1, ios::cur); //����ǰ��ȡ���ַ����¶�ȡ
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
                    code = TK_ACDEIN; //����Ϊ"-="
                }
                else if (ch == '-')
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch);
                    code = TK_ADEIN; //����Ϊ"--"
                }
                else if (ch == '>')
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch);
                    code = TK_POINT; //����Ϊ"->"
                }
                else
                {
                    code = TK_MINUS;          //-����
                    file.seekg(-1, ios::cur); //����ǰ��ȡ���ַ����¶�ȡ
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
                    code = TK_AUTOMUL; //����Ϊ"*="
                }
                else
                {
                    file.seekg(-1, ios::cur);
                    code = TK_STAR; //*�˺�
                }

                break;
            }
            case '/':
            {
                ch = file.get();
                if (ch == '/') //ɾ������ע��
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
                else if (ch == '*') //ɾ������ע��
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
                        if (ch == '\r' && (ch = file.get()) == '\n') //�������ע�͵�����
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
                    code = TK_AUTODIV; //����Ϊ"/="
                }
                else
                {
                    file.seekg(-1, ios::cur);
                    code = TK_DIVIDE; //����
                }
                break;
            }
            case '=':
            {
                ch = file.get(); //��ǰ��ȡ'='������ַ�
                if (ch == '=')   //==���ں�
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch); //��'='�����'='����token��
                    code = TK_EQ;
                }
                else
                { //=��ֵ�����
                    code = TK_ASSIGN;
                    file.seekg(-1, ios::cur); //����ǰ��ȡ���ַ����¶�ȡ
                }
            }
            break;
            case '<':
            {
                ch = file.get(); //��ǰ��ȡ'<'������ַ�
                if (ch == '=')   //<=С�ڵ��ں�
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch); //��'<'�����'='����token��
                    code = TK_LEQ;
                }
                else if (ch == '<') // left shift
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch);
                    code = TK_OSTREAM;
                }
                else
                { //<С�ں�
                    code = TK_LT;
                    file.seekg(-1, ios::cur); //����ǰ��ȡ���ַ����¶�ȡ
                }
            }
            break;
            case '>':
            {
                ch = file.get(); //��ǰ��ȡ'>'������ַ�
                if (ch == '=')
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch); //��'>'�����'='����token��
                    code = TK_GEQ;
                }
                else if (ch == '>') // right shift
                {
                    rowbuffer.push_back(ch);
                    token.push_back(ch);
                    code = TK_ISTREAM;
                }
                else
                { //>���ں�
                    code = TK_GT;
                    file.seekg(-1, ios::cur); //����ǰ��ȡ���ַ����¶�ȡ
                }
            }
            break;
            /*�ֽ��*/
            case '(':
                code = TK_OPENPA; //(��Բ����
                break;
            case ')':
                code = TK_CLOSEPA; //)��Բ����
                break;
            case '[':
                code = TK_OPENBR; //[��������
                break;
            case ']':
                code = TK_CLOSEBR; //]��������
                break;
            case '{':
                code = TK_BEGIN; //{�������
                break;
            case '}':
                code = TK_END; //}�Ҵ�����
                break;
            case ',':
                code = TK_COMMA; //,����
                break;
            case ';':
                code = TK_SEMOCOLOM; //;�ֺ�
                break;
            // case '"':
            //     code = TK_DOUBLEQUTA; //"˫����
            //     break;
            // case '\'':
            //     code = TK_SINGLEQUTA; //'������
            //#
            case '#':
                code = TK_WELL;
                break;
            //δʶ�����
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
    else if(code >= 57 && code <= 62) //˫�ַ������
    {
        ofile << code - 53;
    }
    else if ((code >= 65 && code <= 75) || (code >= 37 && code <= 49)) //���
    {
        ofile << Tokens[token_num]->token;
        simplified_string += Tokens[token_num]->token;
    }
    else if (code == 78 || code == 79) //����
    {
        ofile << 3;
        simplified_string += to_string(3);
    }
    else if (code == 80) //��ʶ��
    {
        ofile << 2;
        simplified_string += to_string(2);
    }
}
void lexical_analysis::showcode() //��ʾԴ����
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
