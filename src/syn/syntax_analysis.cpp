#include "syn.hpp"
using namespace tabulate;
char start;
set<char> Vt{'$'};
set<char> Vn;
bool first[300][300];
FF First;
vector<Quad> quads;             // 四元式
vector<Attribute> symbol_table; // 符号表
map<string, int> entry;         // 符号表入口
int tempvarnum = 0;
int NXQ = 0 + boolOffest;
vector<int> tmp_nxq{boolOffest};
void LR1::Push(AugmentedReg *p)
{
    reg.push_back(p);
}
int LR1::Size()
{
    return reg.size();
}
AugmentedReg *LR1::operator[](const int index)
{
    return reg[index]; // 第index个产生式
}
bool LR1::Contains(string _reg, string search)
{
    for (auto iter = reg.begin(); iter != reg.end(); iter++)
    {
        string existing = string(&(*iter)->left_reg, 1) + "->" + (*iter)->right_reg;
        if (strcmp(_reg.c_str(), existing.c_str()) == 0 && search == (*iter)->search)
        {
            return true;
        }
    }
    return false;
}
void add_closure(char lookahead, string belta, string search, LR1 &item, AugmentedGrammar &grammar)
{
    if (!isupper(lookahead) || lookahead == '@')
    { // terminal symbol or eplision, exit
        return;
    }
    string lhs = string(&lookahead, 1);
    for (int i = 0; i < grammar[lookahead].size(); i++)
    {
        string rhs = '.' + grammar[lookahead][i].first;
        if (belta.length() == 1)
        {
            if (belta[0] == '@')
            {
                grammar[lookahead][i].second.clear(); // 初始化
                grammar[lookahead][i].second = search;
            }
            else if (isupper(belta[0]))
            {
                grammar[lookahead][i].second.clear();                   // 初始化
                if (First[belta[0]].find('@') == First[belta[0]].end()) // 如果belta中没有空串
                {
                    for (set<char>::iterator it = First[belta[0]].begin(); it != First[belta[0]].end(); it++)
                    {
                        grammar[lookahead][i].second.push_back(*it);
                    }
                }
                else // 如果belta中有空串
                {
                    grammar[lookahead][i].second.clear(); // 初始化
                    for (set<char>::iterator it = First[belta[0]].begin(); it != First[belta[0]].end(); it++)
                    {
                        if (*it != '@')
                        {
                            grammar[lookahead][i].second.push_back(*it);
                        }
                    }
                    // grammar[lookahead][i].second.append(search); // 增加last item的展望符
                    for (int j = 0; j < search.length(); j++)
                    {
                        if (grammar[lookahead][i].second.find(search[j]) == string::npos)
                        {
                            grammar[lookahead][i].second.push_back(search[j]); // 增加last item的展望符
                        }
                    }
                }
            }
            else if (!isupper(belta[0]))
            {
                grammar[lookahead][i].second.clear(); // 初始化
                grammar[lookahead][i].second.push_back(belta[0]);
            }
        }
        else if (belta.length() == 2) // first(lookahead)包含空
        {
            if (isupper(belta[0]))
            {
                grammar[lookahead][i].second.clear(); // 初始化
                for (set<char>::iterator it = First[belta[0]].begin(); it != First[belta[0]].end(); it++)
                {
                    if (*it != '@')
                    {
                        grammar[lookahead][i].second.push_back(*it);
                    }
                }
                if (belta[1] == '@')
                {
                    for (int j = 0; j < search.length(); j++)
                    {
                        if (grammar[lookahead][i].second.find(search[j]) == string::npos)
                        {
                            grammar[lookahead][i].second.push_back(search[j]); // 增加last item的展望符
                        }
                    }
                    // grammar[lookahead][i].second.append(search[i]); // 增加last item的展望符
                }
                else if (!isupper(belta[1]))
                {
                    grammar[lookahead][i].second.push_back(belta[1]);
                }
                else if (isupper(belta[1]))
                {
                    for (set<char>::iterator it = First[belta[1]].begin(); it != First[belta[1]].end(); it++)
                    {
                        if (*it != '@')
                        {
                            grammar[lookahead][i].second.push_back(*it);
                        }
                    }
                }
            }
            else if (!isupper(belta[0]))
            {
                grammar[lookahead][i].second.clear();
                grammar[lookahead][i].second.push_back(belta[0]);
            }
        }
        // 如果对下一个输入符号的产生式规则还不存在于这个项目集，添加
        if (!item.Contains(lhs + "->" + rhs, grammar[lookahead][i].second))
        {
            // cout << "left reg = " << lookahead << "\tright reg = " << rhs << "\tsearch = " << grammar[lookahead][i].second << endl;
            item.Push(new AugmentedReg(lookahead, rhs, grammar[lookahead][i].second));
        }
        else
        {
            // cout << "repeat" << endl;
            return;
        }
    }
}
bool is_eplision(char lookahead, AugmentedGrammar &grammar)
{
    for (int i = 0; i < grammar[lookahead].size(); i++)
    {
        if (grammar[lookahead][i].first == "@")
        {
            return true;
        }
    }
    return false;
}
// 给定的增广文法生成LR(0)表
void LR1_snytax_analysis(vector<LR1> &lr1item, AugmentedGrammar &grammar, int &itemid, GotoMap &Goto, ofstream &result_syntax_analysis)
{
    result_syntax_analysis << 'I' << itemid << ':' << endl;
    // cout << 'I' << itemid << ":" << endl;
    // ensure that current item contains all the closure of regular
    for (int i = 0; i < lr1item[itemid].Size(); i++) // 对itemid的项目集的每个产生式
    {
        string right_reg = lr1item[itemid][i]->right_reg;
        char lookahead = right_reg[right_reg.find('.') + 1]; // 寻找项目集中的"."的下一个字符
        string belta = "";
        string search = lr1item[itemid][i]->search;

        if (right_reg[right_reg.find('.') + 2] == '\0' || lookahead == '\0')
        {
            belta = "@"; // represent eplision
        }
        else if (right_reg[right_reg.find('.') + 2] != '\0' && lookahead != '\0')
        {
            // First[lookahead].find('@') != First[lookahead].end()
            if (isupper(lookahead) && First[lookahead].find('@') != First[lookahead].end()) // lookahead能推导出空串规则
            {
                if (right_reg[right_reg.find('.') + 3] == '\0')
                {
                    belta.push_back(right_reg[right_reg.find('.') + 2]);
                    belta.push_back('@');
                }
                else if (right_reg[right_reg.find('.') + 3] != '\0')
                {
                    belta.push_back(right_reg[right_reg.find('.') + 2]);
                    belta.push_back(right_reg[right_reg.find('.') + 3]);
                }
                // cout << belta <<endl;
            }
            else
            {
                belta = right_reg[right_reg.find('.') + 2]; // 匹配A->alpha.B belta中的belta
            }
        }
        // cout << "search = " << search <<endl;
        add_closure(lookahead, belta, search, lr1item[itemid], grammar);
    }
    int nextpos;
    char lookahead, forward, left_reg;
    string right_reg;
    string search;
    AugmentedReg *reg;
    for (int i = 0; i < lr1item[itemid].Size(); i++)
    {
        // 当前的产生式(第itemid个项目集的第i个规则的左部与右部)
        left_reg = lr1item[itemid][i]->left_reg;
        right_reg = lr1item[itemid][i]->right_reg;
        search = lr1item[itemid][i]->search;
        string whole_reg = string(&left_reg, 1) + "->" + right_reg + ',' + search;
        lookahead = right_reg[right_reg.find('.') + 1];
        forward = right_reg[right_reg.find('.') - 1];
        if (lookahead == '\0')
        {
            if (forward != start) // 规约项
            {
                for (int i = 0; i < search.length(); i++)
                {
                    lr1item[itemid].gotos[search[i]] = -2; // reduce
                    lr1item[itemid].reduce_reg = whole_reg.substr(0, whole_reg.length() - search.length() - 2);
                }
                // printf("\t%-20s Use %s to reduce for terminal symbol %s\n", &whole_reg[0], &whole_reg.substr(0, whole_reg.length() - search.length() - 2)[0], &search[0]);
                result_syntax_analysis << whole_reg << "\t\t\tUse " << whole_reg.substr(0, whole_reg.length() - search.length() - 2) << " to reduce for terminal symbol " << search << endl;
                continue;
            }
            else // 接受项
            {
                lr1item[itemid].gotos['$'] = -1; // accept
                // printf("\t%-20s Accept\n", &whole_reg[0]);
                result_syntax_analysis << whole_reg << " Accept" << endl;
                continue;
            }
        }
        if (lookahead == '@')
        {
            for (int i = 0; i < search.length(); i++)
            {
                lr1item[itemid].gotos[search[i]] = -2; // reduce
                lr1item[itemid].reduce_reg = whole_reg.substr(0, whole_reg.length() - search.length() - 3) + '@';
            }

            // printf("\t%-20s Use %s to reduce for terminal symbol %s\n", &whole_reg[0], &(whole_reg.substr(0, whole_reg.length() - search.length() - 3) + '@')[0], &search[0]);
            result_syntax_analysis << whole_reg << "\t\t\tUse " << whole_reg.substr(0, whole_reg.length() - search.length() - 3) << " to reduce for terminal symbol " << search << endl;
        }
        if (lr1item[itemid].gotos.find(lookahead) == lr1item[itemid].gotos.end()) // not find lookahead in gotos, then create a new LR(1) item
        {
            if (Goto.find(whole_reg) == Goto.end())
            {
                if (lookahead != '@')
                {
                    lr1item.push_back(LR1()); // 创建一个新的LR1项目集
                    string newright_reg = right_reg;
                    int curpos = newright_reg.find('.');
                    swap(newright_reg[curpos], newright_reg[curpos + 1]); //.后移
                    // 添加项目并且更新goto表
                    lr1item.back().Push(new AugmentedReg(left_reg, newright_reg, lr1item[itemid][i]->search));
                    lr1item[itemid].gotos[lookahead] = lr1item.size() - 1;
                    Goto[whole_reg] = lr1item.size() - 1;
                }
            }
            else
            {
                lr1item[itemid].gotos[lookahead] = Goto[whole_reg];
            }
            // cout << '\t' << setiosflags(ios::left) << setw(20) << whole_reg << "goto(" << lookahead << ")=I" << Goto[whole_reg];
            if (isupper(lookahead))
            {
                // printf("\t%-20s goto(%c)=I%d\n", &whole_reg[0], lookahead, Goto[whole_reg]); //&whole_reg[0]找到产生式的首地址
                result_syntax_analysis << whole_reg << "\t\t\tgoto(" << lookahead << ")=I" << Goto[whole_reg] << endl;
            }
            else if (lookahead != '@')
            {
                // printf("\t%-20s action(%c)=I%d\n", &whole_reg[0], lookahead, Goto[whole_reg]);
                result_syntax_analysis << whole_reg << "\t\t\taction(" << lookahead << ")=I" << Goto[whole_reg] << endl;
            }
        }
        else
        {
            int curpos = right_reg.find('.');
            swap(right_reg[curpos], right_reg[curpos + 1]);
            int nextitem = lr1item[itemid].gotos[lookahead];
            if (!lr1item[nextitem].Contains(string(&left_reg, 1) + "->" + right_reg, search))
            {
                lr1item[nextitem].Push(new AugmentedReg(left_reg, right_reg, search));
            }
            swap(right_reg[curpos], right_reg[curpos + 1]);
            // printf("\t%-20s\n", &whole_reg[0]);
            result_syntax_analysis << whole_reg << endl;
        }
    }
}

void snytax_analysis(string input_string, vector<LR1> &lr1item, vector<Token_info *> Tokens)
{
    stack<char> symbol;          // symbol stack
    stack<int> status;           // status stack
    stack<string> semantic;      // semantic stack
    vector<Attribute> attr(200); // attribute stack
    symbol.push('$');            // init symbol
    status.push(0);              // init status
    Gen("_", -1, -1, boolOffest);
    string point = "->";
    int i = 0;
    int oldpoint = -1;
    char BUFFER[100];
    getcwd(BUFFER, 100);      // 当前可执行文件的路径
    string exe_path = BUFFER; // E:\code\C_C++\Compiler\build\bin
    ofstream log(exe_path + "\\dependency\\log.txt", ios_base::out | ios_base::binary);

    while (i < input_string.length())
    {
        bool err_recover = false;
        bool push_flag = false;
        if (Tokens[i]->code == 80 || Tokens[i]->code == 78 || Tokens[i]->code == 79)
        {
            if (oldpoint != i)
            {
                semantic.push(Tokens[i]->token);
                Attribute tempattr;

                if (entry.count(Tokens[i]->token) == 0)
                {
                    tempattr.Varname = Tokens[i]->token;
                    tempattr.value = Tokens[i]->token;
                    tempattr.place = symbol_table.size();
                    symbol_table.push_back(tempattr);
                    entry[tempattr.Varname] = tempattr.place;
                }
                else
                {
                    if (Tokens[i]->code == 80)
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                        if (input_string[i - 1] == 'h' || input_string[i - 1] == 'z' || input_string[i - 1] == ',')
                        {
                            cout << "Warning: Multiple definition:" << Tokens[i]->token << " at line " << Tokens[i]->row << endl;
                        }
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    }
                }
            }
            oldpoint = i;
        }
        if (lr1item[status.top()].gotos[input_string[i]] == 0) // error
        {
            /*短语层次的错误恢复：
            如果栈顶状态的项目集中有规约项，并且其展望符中含有; } )，强制规约
            */
            for (int j = 0; j < lr1item[status.top()].Size(); j++)
            {
                string s = lr1item[status.top()][j]->search;
                string reg = lr1item[status.top()][j]->right_reg;

                if (reg.back() == '.' && (s.find(';') != string::npos || s.find(')') != string::npos || s.find('}') != string::npos))
                {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                    err_recover = true;
                reduce: // 无条件跳转
                {
                    int _top = status.top();
                    string toreduce = lr1item[_top].reduce_reg;
                    auto pos = toreduce.find(point);
                    if (toreduce.substr(pos + point.length(), string::npos) == "@") // 规约项右部为@，空串规则
                    {
                        symbol.push(toreduce[0]);
                        string _(1, toreduce[0]);
                        attr.push_back(Attribute(_));
                        status.push(lr1item[_top].gotos[toreduce[0]]); // 错误恢复语义关联
                        log << "error recover: reduce with " << toreduce << endl;
                    }
                    else
                    {
                        for (int ii = 0; ii < toreduce.substr(pos + point.length(), string::npos).length(); ii++)
                        {
                            symbol.pop();
                            status.pop();
                        }
                        symbol.push(lr1item[_top].reduce_reg[0]);
                        status.push(lr1item[status.top()].gotos[symbol.top()]);
                        // string _(1, lr1item[_top].reduce_reg[0]);
                        // attr.push_back(Attribute(_));
                        SDT(toreduce, semantic, attr);
                        log << "error recover: reduce with " << toreduce << endl;
                    }
                }
                    if (!push_flag)
                    {
                        if (s.find(';') != string::npos)
                        {
                            push_flag = true;
                            if (lr1item[status.top()].gotos[';'] == -2)
                            {
                                goto reduce;
                            }
                            else
                            {
                                symbol.push(';');
                                attr.push_back(Attribute(";"));
                                log << "error recover: shift ; into stack" << endl;
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
                                cout << "Nonfatal syntax error occured!" << endl;
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                                status.push(lr1item[status.top()].gotos[';']);
                            }
                            cout << "Warning: Lack of ';'"
                                 << " before " << Tokens[i]->token << " At line " << Tokens[i]->row << " col " << Tokens[i]->col << endl;
                        }
                        else if (s.find('}') != string::npos)
                        {
                            push_flag = true;
                            if (lr1item[status.top()].gotos['}'] == -2)
                            {
                                goto reduce;
                            }
                            else
                            {
                                symbol.push('}');
                                attr.push_back(Attribute("}"));
                                log << "error recover: shift } into stack" << endl;
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
                                cout << "Nonfatal syntax error occured!" << endl;
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                                status.push(lr1item[status.top()].gotos['}']);
                            }
                            cout << "Warning: Lack of '}'"
                                 << " before " << Tokens[i]->token << " At line " << Tokens[i]->row << " col " << Tokens[i]->col << endl;
                        }
                        else if (s.find(')') != string::npos)
                        {
                            push_flag = true;
                            if (lr1item[status.top()].gotos[')'] == -2)
                            {
                                goto reduce;
                            }
                            else
                            {
                                symbol.push(')');
                                attr.push_back(Attribute(")"));
                                log << "error recover: shift ) into stack" << endl;
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
                                cout << "Nonfatal syntax error occured!" << endl;
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                                status.push(lr1item[status.top()].gotos[')']);
                            }
                            cout << "Warning: Lack of ')'"
                                 << " before " << Tokens[i]->token << " At line " << Tokens[i]->row << endl;
                        }
                    }
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                }
            }
            if (!err_recover) // 无法继续进行错误恢复
            {
                // ostringstream err_msg;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
                cout << "Fatal error, can't recover !" << endl;
                cout << "Maybe lack: ";
                for (auto it = Vt.begin(); it != Vt.end(); it++)
                {
                    if (lr1item[status.top()].gotos[*it] != 0)
                    {
                        cout << '\'' << *it << '\'' << " ";
                    }
                }
                cout << "before " << Tokens[i]->token << " At line " << to_string(Tokens[i]->row); // 可以再更新改进
                // throw runtime_error(err_msg.str());
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                cout << endl;
                break;
            }
        }
        else if (lr1item[status.top()].gotos[input_string[i]] == -1)
        { // accept
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
            cout << "Result of lexical analysis : \"" << input_string.substr(0, input_string.length() - 1) << "\""
                 << " is the sentential form of this grammar after error recover" << endl
                 << "No syntax error after error recover!" << endl;
            i++;
        }
        else if (lr1item[status.top()].gotos[input_string[i]] == -2)
        { // reduce

            int _top = status.top();
            string toreduce = lr1item[_top].reduce_reg;
            auto pos = toreduce.find(point);
            log << "reduce with " << toreduce << endl;
            if (toreduce.substr(pos + point.length(), string::npos) == "@") // 规约项右部为@，空串规则
            {
                symbol.push(toreduce[0]);
                status.push(lr1item[_top].gotos[toreduce[0]]);
                string _(1, toreduce[0]);
                attr.push_back(Attribute(_));
            }
            else
            {
                for (int i = 0; i < toreduce.substr(pos + point.length(), string::npos).length(); i++)
                {
                    symbol.pop();
                    status.pop();
                }
                symbol.push(lr1item[_top].reduce_reg[0]);
                status.push(lr1item[status.top()].gotos[symbol.top()]);
                SDT(toreduce, semantic, attr);
            }
        }
        else if (lr1item[status.top()].gotos[input_string[i]] > 0) // action/goto shift
        {
            log << "shift " << Tokens[i]->token << " into stack" << endl;
            symbol.push(input_string[i]);
            status.push(lr1item[status.top()].gotos[input_string[i]]);
            attr.push_back(Attribute(Tokens[i]->token));
            i++;
        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
void SDT(string toreduce, stack<string> &semantic, vector<Attribute> &attr)
{
    string point = "->";
    auto pos = toreduce.find(point);
    if (toreduce == "F->2=E;")
    {
        Attribute I, E;
        for (int poptime = 0; poptime < 4; poptime++)
        {
            if (poptime == 1)
            {
                E = attr.back();
            }
            if (poptime == 3)
            {
                I = attr.back();
            }
            attr.pop_back();
        }
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        semantic.pop();
        // I.Varname = semantic.top();
        semantic.pop();
        I.place = entry[I.Varname];
        I.value = symbol_table[E.place].value;
        symbol_table[I.place].value = symbol_table[E.place].value;
        Gen("=", E.place, -1, I.place);
    }
    else if (toreduce == "O->3" || toreduce == "O->2" || toreduce == "K->2" || toreduce == "K->3")
    {
        attr.pop_back();
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        attr.back().place = entry[semantic.top()];
    }
    else if (toreduce == "U->O" || toreduce == "T->U" ||
             toreduce == "E->T" || toreduce == "Q->R" ||
             toreduce == "R->X" || toreduce == "X->Y" ||
             toreduce == "Y->V" || toreduce == "V->K")
    {
        Attribute T = attr.back();
        attr.pop_back();
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        attr.back().place = T.place;
        attr.back().TC = T.TC;
        attr.back().FC = T.FC;
    }
    else if (toreduce == "O->(E)")
    {
        Attribute E1;
        for (int poptime = 0; poptime < 3; poptime++)
        {
            if (poptime == 1)
            {
                E1 = attr.back();
            }
            attr.pop_back();
        }
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        attr.back().place = E1.place;
    }
    else if (toreduce == "C->iH")
    {
        Attribute E = attr.back();
        attr.pop_back();
        attr.pop_back();
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        backpatch(E.TC, NXQ);
        attr.back().chain = E.FC;
    }
    else if (toreduce == "F->CF" || toreduce == "F->GF")
    {
        Attribute F = attr.back();
        attr.pop_back();
        Attribute C = attr.back();
        attr.pop_back();
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        attr.back().chain = merge(C.chain, F.chain);
    }
    else if (toreduce == "G->CFe")
    {
        Attribute C, F;
        for (int poptime = 0; poptime < 3; poptime++)
        {
            if (poptime == 1)
            {
                F = attr.back();
            }
            if (poptime == 2)
            {
                C = attr.back();
            }
            attr.pop_back();
        }
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        int q = NXQ;
        Gen("Jump", -1, -1, 0 + boolOffest);
        backpatch(C.chain, NXQ);
        attr.back().chain = merge(F.chain, q);
    }
    else if (toreduce == "Z->w")
    {
        Attribute _while = attr.back();
        attr.pop_back();
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        attr.back().quad = NXQ;
    }
    else if (toreduce == "Z->d")
    {
        Attribute _Do = attr.back();
        attr.pop_back();
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        attr.back().head = NXQ;
    }
    else if (toreduce == "W->ZH")
    {
        Attribute Z, H;
        for (int poptime = 0; poptime < 2; poptime++)
        {
            if (poptime == 0)
            {
                H = attr.back();
            }
            if (poptime == 1)
            {
                Z = attr.back();
            }
            attr.pop_back();
        }
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        backpatch(H.TC, NXQ);
        attr.back().chain = H.FC;
        attr.back().quad = Z.quad;
    }
    else if (toreduce == "F->WF")
    {
        Attribute F = attr.back();
        attr.pop_back();
        Attribute W = attr.back();
        attr.pop_back();
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        backpatch(F.chain, W.quad);
        Gen("Jump", -1, -1, W.quad);
        attr.back().chain = W.chain;
    }
    else if (toreduce == "F->LH;")
    {
        attr.pop_back();
        Attribute H = attr.back();
        attr.pop_back();
        Attribute L = attr.back();
        attr.pop_back();
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        backpatch(H.TC, L.head);
        attr.back().chain = H.FC;
    }
    else if (toreduce == "L->ZFZ")
    {
        Attribute Z, F;
        for (int poptime = 0; poptime < 3; poptime++)
        {
            if (poptime == 1)
            {
                F = attr.back();
            }
            if (poptime == 2)
            {
                Z = attr.back();
            }
            attr.pop_back();
        }
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        attr.back().head = Z.head;
        backpatch(F.chain, NXQ);
    }
    else if (toreduce == "B->{DS}")
    {
        Attribute S;
        for (int poptime = 0; poptime < 4; poptime++)
        {
            if (poptime == 1)
            {
                S = attr.back();
            }
            attr.pop_back();
        }
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        // backpatch(S.chain, NXQ);
        attr.back().chain = S.chain;
        tmp_nxq.push_back(NXQ);
    }
    else if (toreduce == "F->B")
    {
        Attribute F = attr.back();
        attr.pop_back();
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        // backpatch(F.chain, NXQ);
        attr.back().chain = F.chain;
    }
    else if (toreduce == "P->B")
    {
        Attribute P = attr.back();
        attr.pop_back();
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        backpatch(P.chain, NXQ);
        Gen("End", -1, -1, -1);
    }
    else if (toreduce == "H->J" || toreduce == "J->M")
    {
        Attribute H = attr.back();
        attr.pop_back();
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        attr.back().place = H.place;
        attr.back().TC = H.TC;
        attr.back().FC = H.FC;
    }
    else if (toreduce == "M->Q")
    {
        Attribute M = attr.back();
        attr.pop_back();
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));

        // attr.back().TC = M.TC;
        // attr.back().FC = M.FC;
        attr.back().place = M.place;
        attr.back().TC = NXQ;
        attr.back().FC = NXQ + 1;
        Gen("Jtrue", M.place, -1, 0 + boolOffest);
        Gen("Jump", -1, -1, 0 + boolOffest);
    }
    else if (toreduce == "K->(H)")
    {
        Attribute K;
        for (int poptime = 0; poptime < 3; poptime++)
        {
            if (poptime == 1)
            {
                K = attr.back();
            }
            attr.pop_back();
        }
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        attr.back().TC = K.TC;
        attr.back().FC = K.FC;
        attr.back().place = K.place;
        backpatch(K.TC, NXQ);
        backpatch(K.FC, NXQ + 1);
    }
    else if (toreduce == "H->IJ")
    {
        Attribute J = attr.back();
        attr.pop_back();
        Attribute I = attr.back();
        attr.pop_back();
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        attr.back().FC = J.FC;
        attr.back().TC = merge(I.TC, J.TC);
    }
    else if (toreduce == "J->NM")
    {
        Attribute M = attr.back();
        attr.pop_back();
        Attribute N = attr.back();
        attr.pop_back();
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        attr.back().TC = M.TC;
        attr.back().FC = merge(N.FC, M.FC);
    }
    else if (toreduce == "I->H5")
    {
        attr.pop_back();
        Attribute H = attr.back();
        attr.pop_back();
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        backpatch(H.FC, NXQ);
        attr.back().TC = H.TC;
        attr.back().place = H.place;
    }
    else if (toreduce == "N->J6")
    {
        attr.pop_back();
        Attribute J = attr.back();
        attr.pop_back();
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        backpatch(J.TC, NXQ);
        attr.back().FC = J.FC;
        attr.back().place = J.place;
    }
    else if (toreduce == "V->!V")
    {
        Attribute U = attr.back();
        attr.pop_back();
        attr.pop_back();
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        attr.back().TC = U.FC;
        attr.back().FC = U.TC;
    }
    else if (toreduce == "R->R<X" || toreduce == "R->R8X" ||
             toreduce == "R->R>X" || toreduce == "R->R9X" ||
             toreduce == "Q->Q7R" || toreduce == "Q->Q4R")
    {
        Attribute AE1, AE2;
        for (int poptime = 0; poptime < 3; poptime++)
        {
            if (poptime == 0)
            {
                AE2 = attr.back();
            }
            if (poptime == 2)
            {
                AE1 = attr.back();
            }
            attr.pop_back();
        }
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        attr.back().TC = NXQ;
        attr.back().FC = NXQ + 1;
        int q = NXQ + 1;
        if (toreduce[4] == '8')
        {
            Gen("J<=", AE1.place, AE2.place, q);
        }
        else if (toreduce[4] == '9')
        {
            Gen("J>=", AE1.place, AE2.place, q);
        }
        else if (toreduce[4] == '4')
        {
            Gen("J!=", AE1.place, AE2.place, q);
        }
        else if (toreduce[4] == '7')
        {
            Gen("J==", AE1.place, AE2.place, q);
        }
        else if (toreduce[4] == '<')
        {
            Gen("J<", AE1.place, AE2.place, q);
        }
        else if (toreduce[4] == '>')
        {
            Gen("J>", AE1.place, AE2.place, q);
        }
        // Gen("Jump", -1, -1, 0 + boolOffest);
    }
    else if (toreduce == "T->T*U" || toreduce == "T->T/U" ||
             toreduce == "E->E+T" || toreduce == "E->E-T" ||
             toreduce == "Y->Y*V" || toreduce == "Y->Y/V" ||
             toreduce == "X->X+Y" || toreduce == "X->X-Y")
    {
        Attribute E1, E2;
        for (int poptime = 0; poptime < 3; poptime++)
        {
            if (poptime == 0)
            {
                E2 = attr.back();
            }
            if (poptime == 2)
            {
                E1 = attr.back();
            }
            attr.pop_back();
        }
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        Attribute T = newtemp();
        T.place = symbol_table.size();
        try
        {
            if (toreduce[4] == '+')
            {
                T.value = to_string(stoi(symbol_table[E1.place].value) + stoi(symbol_table[E2.place].value));
            }
            else if (toreduce[4] == '-')
            {
                T.value = to_string(stoi(symbol_table[E1.place].value) - stoi(symbol_table[E2.place].value));
            }
            else if (toreduce[4] == '*')
            {
                T.value = to_string(stoi(symbol_table[E1.place].value) * stoi(symbol_table[E2.place].value));
            }
            else if (toreduce[4] == '/')
            {
                if(stoi(symbol_table[E2.place].value) == 0)
                {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
                    cout << "A fatal semantic error: divide by zero." << endl;
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                }
                else
                {
                    T.value = to_string(stoi(symbol_table[E1.place].value) / stoi(symbol_table[E2.place].value));
                }
            }
        }
        catch (...)
        {
            // cout << "A fatal semantic error: divide by zero." << endl;
        }
        symbol_table.push_back(T);
        entry[T.Varname] = T.place;
        string s(1, toreduce[4]);
        Gen(s, E1.place, E2.place, T.place);
        semantic.pop();
        semantic.pop();
        semantic.push(T.Varname);
        attr.back().place = T.place;
        attr.back().value = T.value;
    }
    else if (toreduce == "S->SF")
    {

        Attribute F = attr.back();
        attr.pop_back();
        Attribute S = attr.back();
        attr.pop_back();
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
        int q;
        if (S.chain != -1)
        {
            for (int iter = 1; iter < tmp_nxq.size(); iter++)
            {
                tmp_nxq[iter] = abs(S.chain - tmp_nxq[iter]);
            }
            q = *min_element(tmp_nxq.begin() + 1, tmp_nxq.end());
        }

        backpatch(S.chain, S.chain + q);
        attr.back().chain = F.chain;
    }
    else if (toreduce == "D->A;D" || toreduce == "A->A,2" ||
             toreduce == "A->h2" || toreduce == "A->z2" ||
             toreduce == "U->-U" || toreduce == "V->-V")
    {
        for (int i = 0; i < toreduce.substr(pos + point.length(), string::npos).length(); i++)
        {
            attr.pop_back();
        }
        string _(1, toreduce[0]);
        attr.push_back(Attribute(_));
    }
}
void read_grammar(string path, AugmentedGrammar &grammar, vector<LR1> &lr1item, vector<AR *> &orignal_grammar)
{
    char _production[20];
    char _lhs[10];
    string right_reg;
    string production;
    string point = "->";

    ifstream file(path, ios_base::binary);
    file.getline(_lhs, 10); // scan start production
    string left_reg(&_lhs[0], &_lhs[strlen(_lhs)]);
    left_reg = left_reg.substr(0, left_reg.length() - 1); // 剔除文件中的\r
    start = left_reg.data()[0];
    Vn.insert(start);
    Vt.insert('$');
    grammar['\''].push_back(make_pair(left_reg, "$"));

    lr1item[0].Push(new AugmentedReg('\'', "." + left_reg, "$"));

    printf("'->%s\n", left_reg.c_str());
    while (!file.eof())
    {
        file.getline(_production, 20);
        string production(&_production[0], &_production[strlen(_production)]);
        production = production.substr(0, production.length() - 1); // 剔除文件中的\r
        auto pos = production.find(point);
        if (pos != string::npos)
        {
            left_reg = production.substr(0, pos);
            right_reg = production.substr(pos + point.length(), string::npos);
            for (int i = 0; i < right_reg.length(); i++)
            {
                if (!isupper(right_reg[i]))
                {
                    Vt.insert(right_reg[i]);
                }
                else
                {
                    Vn.insert(right_reg[i]);
                }
            }
        }
        orignal_grammar.push_back(new AugmentedReg(left_reg[0], right_reg));
        grammar[left_reg[0]].push_back(make_pair(right_reg, ""));
        printf("%s->%s\n", left_reg.c_str(), right_reg.c_str());
        // lr1item[0].Push(new AugmentedReg(left_reg[0], '.' + right_reg));
    }
}

void getfirst(bool first[300][300], set<char> Vt, set<char> Vn, vector<AR *> orignal_grammar)
{
    bool done = true, isempty;
    int p2sym; // point to every symbol in every production
    int k;
    while (done)
    {
        done = false;
        for (int i = 0; i < orignal_grammar.size(); i++)
        {

            p2sym = 0; // 仅指向规则右部
            isempty = true;
            while (isempty && p2sym < orignal_grammar[i]->right_reg.length())
            {
                isempty = false;
                if (isupper(orignal_grammar[i]->right_reg[p2sym])) // 出现非终结符号
                {
                    for (k = 0; k <= 63; ++k)
                    {
                        if (first[orignal_grammar[i]->right_reg[p2sym] - 'A'][k] && !first[orignal_grammar[i]->left_reg - 'A'][k])
                        {
                            first[orignal_grammar[i]->left_reg - 'A'][k] = true;
                            done = true;
                        }
                    }
                    if (first[orignal_grammar[i]->right_reg[p2sym] - 'A'][64])
                    {
                        isempty = true;
                        ++p2sym;
                    }
                    for (k = 91; k < 128; k++)
                    {
                        if (first[orignal_grammar[i]->right_reg[p2sym] - 'A'][k] && !first[orignal_grammar[i]->left_reg - 'A'][k])
                        {
                            first[orignal_grammar[i]->left_reg - 'A'][k] = true;
                            done = true;
                        }
                    }
                }
                else if (!first[orignal_grammar[i]->left_reg - 'A'][orignal_grammar[i]->right_reg[p2sym]])
                {
                    done = true;
                    first[orignal_grammar[i]->left_reg - 'A'][orignal_grammar[i]->right_reg[p2sym]] = true;
                    // First[orignal_grammar[i]->left_reg].insert(char(p2sym));
                }
            }
            if (orignal_grammar[i]->right_reg.length() + 1 == p2sym)
            {
                first[orignal_grammar[i]->left_reg - 'A'][26] = true;
            }
        }
    }
}
void write_first_set()
{
    for (int i = 0; i < 26; ++i)
    {
        char ch = char(i + 'A');
        if (Vn.find(ch) != Vn.end())
        { // find ch in non-termianl symbol
            for (int j = 0; j < 128; ++j)
            {
                if (first[i][j])
                {
                    First[ch].insert(char(j));
                }
            }
        }
    }
}
void showtable(Table syn_table, vector<LR1> &lr1item)
{
    cout << "----------------------------------------" << endl;
    cout << "Generate LR(1) analysis table..." << endl;
    Table::Row_t header;
    header.push_back("State");
    for (auto it = Vt.begin(); it != Vt.end(); it++)
    {
        string _ = string(1, *it);
        header.push_back(_);
    }
    for (auto it = Vn.begin(); it != Vn.end(); it++)
    {
        string _ = string(1, *it);
        header.push_back(_);
    }
    syn_table.add_row(header);
    for (size_t i = 0; i < lr1item.size(); i++)
    {
        Table::Row_t row;
        row.push_back('I' + to_string(i));
        for (set<char>::iterator j = Vt.begin(); j != Vt.end(); j++)
        { // 添加终结符的分析表
            if (lr1item[i].gotos[*j] == -1)
            {
                row.push_back("Accept");
            }
            else if (lr1item[i].gotos[*j] == -2)
            {
                row.push_back("Reduce with " + lr1item[i].reduce_reg);
            }
            else if (!lr1item[i].gotos[*j])
            {
                row.push_back("Error");
            }
            else
            {
                row.push_back(to_string(lr1item[i].gotos[*j]));
            }
        }
        for (set<char>::iterator j = Vn.begin(); j != Vn.end(); j++)
        { // 添加非终结符的分析表
            if (lr1item[i].gotos[*j])
            {
                row.push_back(to_string(lr1item[i].gotos[*j]));
            }
            else
            {
                row.push_back("Error");
            }
        }
        syn_table.add_row(row);
    }
    cout << "----------------------------------------" << endl;
}

void translate()
{
    cout << "Assembly is as follows:" << endl;
    int labelNum = 1;
    map<int, string> L;
    string out_msg;
    for (int i = 1; i < quads.size(); i++)
    {
        if (quads[i].dest > boolOffest)
        {
            L.insert(make_pair(quads[i].dest - boolOffest, "L"));
        }
        else if (quads[i].dest == boolOffest)
        {
            quads[i].dest = NXQ;
            L.insert(make_pair(quads[i].dest - boolOffest, "L"));
        }
    }
    for (auto it = L.begin(); it != L.end(); it++)
    {
            it->second += to_string(labelNum);
            labelNum++;
    }
    for (int i = 1; i < quads.size() - 1; i++)
    {
        string op = quads[i].opt;
        int arg1 = quads[i].lhs;
        int arg2 = quads[i].rhs;
        Attribute result;
        if (quads[i].dest < boolOffest)
        {
            result = symbol_table[quads[i].dest];
        }
        if (L.count(i) != 0)
            cout << L[i] << ":" << endl;

        if (op == "=")
        {
            cout << "   MOV R0," << symbol_table[arg1].Varname << endl;
            cout << "   MOV " << result.Varname << ",R0" << endl;
        }
        else if (op == "+")
        {
            cout << "   MOV R0," << symbol_table[arg1].Varname << endl;
            cout << "   ADD R0," << symbol_table[arg2].Varname << endl;
            cout << "   MOV " << result.Varname << " R0" << endl;
        }
        else if (op == "-")
        {
            cout << "   MOV R0," << symbol_table[arg1].Varname << endl;
            cout << "   SUB R0," << symbol_table[arg2].Varname << endl;
            cout << "   MOV " << result.Varname << " R0" << endl;
        }
        else if (op == "*")
        {
            cout << "   MOV AL," << symbol_table[arg1].Varname << endl;
            cout << "   MOV BL," << symbol_table[arg2].Varname << endl;
            cout << "   MUL BL" << endl;
            cout << "   MOV " << result.Varname << ",AX" << endl;
        }
        else if (op == "/")
        {
            cout << "   MOV AX," << symbol_table[arg1].Varname << endl;
            cout << "   MOV BH," << symbol_table[arg2].Varname << endl;
            cout << "   DIV BH" << endl;
            cout << "   MOV " << result.Varname << ",AL" << endl;
        }
        else if (op == "J>")
        {
            out_msg.clear();
            cout << "   MOV R0," << symbol_table[arg1].Varname << endl;
            cout << "   CMP R0," << symbol_table[arg2].Varname << endl;
            cout << "   JA " << L[quads[i].dest - boolOffest] << endl;
            out_msg += "   MOV R0," + symbol_table[arg1].Varname + '\n'
                    + "   CMP R0," + symbol_table[arg2].Varname + '\n'
                    + "   JA ";
        }
        else if (op == "J>=")
        {
            out_msg.clear();
            cout << "   MOV R0," << symbol_table[arg1].Varname << endl;
            cout << "   CMP R0," << symbol_table[arg2].Varname << endl;
            cout << "   JNB " << L[quads[i].dest - boolOffest] << endl;
            out_msg += "   MOV R0," + symbol_table[arg1].Varname + '\n'
                    + "   CMP R0," + symbol_table[arg2].Varname + '\n'
                    + "   JNB ";
        }
        else if (op == "J==")
        {
            out_msg.clear();
            cout << "   MOV R0," << symbol_table[arg1].Varname << endl;
            cout << "   CMP R0," << symbol_table[arg2].Varname << endl;
            cout << "   JE " << L[quads[i + 1].dest - boolOffest] << endl;
            out_msg += "   MOV R0," + symbol_table[arg1].Varname + '\n'
                    + "   CMP R0," + symbol_table[arg2].Varname + '\n'
                    + "   JE ";
        }
        else if (op == "J!=")
        {
            out_msg.clear();
            cout << "   MOV R0," << symbol_table[arg1].Varname << endl;
            cout << "   CMP R0," << symbol_table[arg2].Varname << endl;
            cout << "   JNE " << L[quads[i].dest - boolOffest] << endl;
            out_msg += "   MOV R0," + symbol_table[arg1].Varname + '\n'
                    + "   CMP R0," + symbol_table[arg2].Varname + '\n'
                    + "   JNE ";
        }
        else if (op == "J<")
        {
            out_msg.clear();
            cout << "   MOV R0," << symbol_table[arg1].Varname << endl;
            cout << "   CMP R0," << symbol_table[arg2].Varname << endl;
            cout << "   JB " << L[quads[i].dest - boolOffest] << endl;
            out_msg += "   MOV R0," + symbol_table[arg1].Varname + '\n'
                    + "   CMP R0," + symbol_table[arg2].Varname + '\n'
                    + "   JB ";
        }
        else if (op == "J<=")
        {
            out_msg.clear();
            cout << "   MOV R0," << symbol_table[arg1].Varname << endl;
            cout << "   CMP R0," << symbol_table[arg2].Varname << endl;
            cout << "   JNA " << L[quads[i].dest - boolOffest] << endl;
            out_msg += "   MOV R0," + symbol_table[arg1].Varname + '\n'
                    + "   CMP R0," + symbol_table[arg2].Varname + '\n'
                    + "   JNA ";
        }
        else if (op == "Jump")
        {
            cout << "   JMP " << L[quads[i].dest - boolOffest] << endl;
        }
        else if (op == "Jtrue")
        {
            if (arg1 == -1)
            {
                // if(quads[i - 1].opt == "J>")
                //     cout << "   JA " << L[quads[i].dest - boolOffest] << endl;
                // else if(quads[i - 1].opt == "J>=")
                //     cout << "   JNB " << L[quads[i].dest - boolOffest] << endl;
                // else if(quads[i - 1].opt == "J==")
                //     cout << "   JE " << L[quads[i].dest - boolOffest] << endl;
                // else if(quads[i - 1].opt == "J!=")
                //     cout << "   JNE " << L[quads[i].dest - boolOffest] << endl;
                // else if(quads[i - 1].opt == "J<")
                //     cout << "   JB " << L[quads[i].dest - boolOffest] << endl;
                // else if(quads[i - 1].opt == "J<=")
                //     cout << "   JNA " << L[quads[i].dest - boolOffest] << endl;
                cout << out_msg << L[quads[i].dest - boolOffest] << endl;
            }
            if (arg1 != -1 && symbol_table[arg1].value == "0")
            {
                cout << "   MOV R0," << symbol_table[arg1].Varname << endl;
                cout << "   TEST R0,R0 " << endl;
                cout << "   JZ " << L[quads[i].dest - boolOffest] << endl;
            }
            else if (arg1 != -1 && symbol_table[arg1].value != "0")
            {
                cout << "   MOV R0," << symbol_table[arg1].Varname << endl;
                cout << "   TEST R0,R0 " << endl;
                cout << "   JNZ " << L[quads[i].dest - boolOffest] << endl;
            }
        }
    }
    cout << "L" << labelNum - 1 << ':' << endl
         << "   END" << endl;
    cout << "----------------------------------------" << endl;
}
Attribute newtemp()
{
    tempvarnum++;
    return Attribute{"T" + to_string(tempvarnum)};
}
void Gen(string op, int arg1, int arg2, int dest)
{
    quads.push_back(Quad(op, arg1, arg2, dest));
    NXQ++;
}
void finalQuad()
{
    cout << "Quads are as follows:" << endl;
    for (int i = 0; i < quads.size(); i++)
    {
        string op = quads[i].opt;
        int arg1 = quads[i].lhs;
        int arg2 = quads[i].rhs;
        int dest = quads[i].dest;
        // 输出四元式
        cout << '(' << i << ")\t" << '(' << op << '\t';
        arg1 != -1 ? cout << symbol_table[arg1].Varname : cout << '_';
        cout << '\t';
        arg2 != -1 ? cout << symbol_table[arg2].Varname : cout << '_';
        cout << '\t';
        if (dest == -1)
        {
            cout << '?';
        }
        else
        {
            dest < boolOffest ? cout << symbol_table[dest].Varname : cout << dest - boolOffest;
        }
        cout << ')' << endl;
    }
    cout << "----------------------------------------" << endl;
}
int merge(int P1, int P2)
{
    if (P1 == 0 + boolOffest || P2 == -1 || P2 == 0)
    {
        return P1;
    }
    else
    {
        int P = P2;
        while (quads[P - boolOffest].dest != 0 + boolOffest)
        {
            P = quads[P - boolOffest].dest;
        }
        quads[P - boolOffest].dest = P1;
        return P2;
    }
}
void backpatch(int P, int t)
{
    if (P == -1 || P == 0) // P是拉链首部，TC是增长链，FC为递减链
    {
        return;
    }
    int Q = P;
    while (Q != 0 + boolOffest)
    {
        int m = quads[Q - boolOffest].dest;
        quads[Q - boolOffest].dest = t;
        Q = m;
    }
}
