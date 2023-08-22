#include "lex.hpp"
#include "syn.hpp"
#include <windows.h>
#include <direct.h>
using namespace std;
using namespace tabulate;
int main()
{
    vector<Token_info *> Tokens;
    char BUFFER[100];
    getcwd(BUFFER, 100);//当前可执行文件的路径
    string exe_path = BUFFER;//E:\code\C_C++\Compiler\build\bin
    string lex_path;
    cout << "please input file path:" << endl;
    cin >> lex_path;
    ofstream outfile(exe_path + "\\dependency\\result_lexical_analysis.txt", ios_base::out | ios_base::binary);
    lexical_analysis la(lex_path);
    outfile.close();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    la.showcode();
    cout << endl
         << "RESULT OF LEXICAL ANALYSIS AS FOLLOWS:" << endl
         << endl;
    la.getToken();
    Tokens = la.getTokens();
    lex_table.column(0).format().width(40).font_align(FontAlign::center);
    size_t index = 0;
    for (auto &row : lex_table)
    {
        row.format()
            .font_style({FontStyle::bold});
        if (index >= 0 && index % 2 == 0)
        {
            for (auto &cell : row)
            {
                cell.format()
                    .font_background_color(Color::blue);
            }
        }
        index += 1;
    }
    cout << lex_table << endl;

    Table syn_table;
    
    string syn_path = exe_path + "\\dependency\\grammar.txt";
    int itemid = -1;
    AugmentedGrammar grammar;
    vector<LR1> lr1item = {LR1()};
    vector<AR *> orignal_grammar;
    GotoMap Goto;

    cout << "Loading The Grammar..." << endl;
    cout << "Augmented grammar as follows:" << endl;
    read_grammar(syn_path, grammar, lr1item, orignal_grammar);
    getfirst(first, Vt, Vn, orignal_grammar);
    write_first_set();
    cout << "----------------------------------------" << endl;
    cout << "First Set:" << endl;
    for (set<char>::iterator it = Vn.begin(); it != Vn.end(); it++)
    { // 遍历Vt set
        cout << "First[" << *it << "]={";
        for (set<char>::iterator i = First[*it].begin(); i != First[*it].end(); i++)
        {
            cout << *i << ' ';
        }
        cout << '}' << endl;
    }
    cout << "----------------------------------------" << endl;
    cout << "Sets of LR(1) Items save in result_syntax_analysis.txt" << endl;
    ofstream result_syntax_analysis(exe_path + "\\dependency\\result_syntax_analysis.txt", ios_base::out | ios_base::binary);
    while (++itemid < int(lr1item.size()))
    {
        LR1_snytax_analysis(lr1item, grammar, itemid, Goto, result_syntax_analysis);
    }
    cout << endl;
    showtable(syn_table, lr1item);
    MarkdownExporter exporter;
    auto markdown = exporter.dump(syn_table);
    // cout<<table<<endl;
    ofstream analysis_table(exe_path + "\\dependency\\analysis table.md", ios_base::out | ios_base::binary);
    analysis_table << markdown << endl;
    string input_string;
    ifstream lexresult(exe_path + "\\dependency\\result_lexical_analysis.txt", ios_base::in | ios_base::binary);
    getline(lexresult, input_string);
    input_string += '$';
    for(auto iter = Tokens.begin();iter != Tokens.end();)//清除由词法分析单行与多行注释带来的空白token
    {
        if((*iter)->token.empty())
        {
            iter = Tokens.erase(iter);
        }
        else{
            ++iter;
        }
    }
    Tokens.push_back(new Token_info(TK_UNDEF, "$", -1, -1));
    snytax_analysis(input_string, lr1item, Tokens);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    finalQuad();
    translate();
    system("pause");
    return 0;
}
