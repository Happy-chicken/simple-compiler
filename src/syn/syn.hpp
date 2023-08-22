#ifndef SYN_H
#define SYN_H
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <map>
#include <fstream>
#include <stack>
#include <set>
#include <unordered_set>
#include <windows.h>
#include "symbols.h"
#include "lex.hpp"
#include "tabulate/table.hpp"
#include <tabulate/markdown_exporter.hpp>
using namespace std;
typedef map<char, vector<pair<string, string > > > AugmentedGrammar;
typedef map<string, int> GotoMap;
typedef map<char, set<char>> FF; // First
extern char start;	 // 开始符号
extern set<char> Vt; // terminal symbol
extern set<char> Vn; // non-terminal symbol
extern bool first[][300];
extern FF First; // first set
const int boolOffest = 10000;
extern map<string, int> entry;//符号表入口
extern int tempvarnum;//临时变量数
extern int NXQ;
extern vector <int> tmp_nxq;
// extern stack<Attribute> attr;//属性栈
// 增广文法
typedef struct AugmentedReg
{
	char left_reg;	  // 规则左部
	string right_reg; // 规则右部
	string search;
	// overload the construct function                                                                                                          //向前搜索符
	AugmentedReg() {}																										   // 默认构造函数
	AugmentedReg(char left_reg, string right_reg) : left_reg(left_reg), right_reg(right_reg) {}								   // 含参构造函数
	AugmentedReg(char left_reg, string right_reg, string search) : left_reg(left_reg), right_reg(right_reg), search(search) {} // 含参构造函数
} AR;

// 四元式
struct Quad
{

	Quad() = default;
	Quad(string o, int l, int r, int d)
		: opt(o), lhs(l), rhs(r), dest(d) {}

	string opt;	 // 操作符
	int lhs;	 // 左操作数
	int rhs;	 // 右操作数
	int dest; // 目标数符号表地址
};
extern vector<Quad> quads;//四元式数组
//属性
struct Attribute
{
	Attribute() = default;
	Attribute(string name):Varname(name) {}
	string Varname;//变量名
	string value;//变量值
	int place{-1};//符号表中的位置
	int TC{-1};//布尔表达式的真出口，用于回填
	int FC{-1};//假出口
	int chain{-1};//if控制语句
	int head{-1};//do-while
	int quad{-1};//while
};
extern vector<Attribute> symbol_table;//符号表数组
class LR1
{
private:
	vector<AugmentedReg *> reg; // 产生式数组

public:
	map<char, int> gotos; // 出边表
	string reduce_reg = "";
	LR1() {}								   // 构造函数
	~LR1() {}								   // 析构函数
	void Push(AugmentedReg *p);				   // 添加产生式
	int Size();								   // 返回产生式的数量
	bool Contains(string _reg, string search); // 判断产生式数组是否包含产生式二元组_reg及其向前看搜索符
	AugmentedReg *operator[](const int index); // 重载指针运算符,索引第i个产生式
};
void add_closure(char lookahead, string belta, set<char> search, LR1 &item, AugmentedGrammar &grammar);
bool is_eplision(char lookahead, AugmentedGrammar &grammar);
void LR1_snytax_analysis(vector<LR1> &lr1item, AugmentedGrammar &grammar, int &itemid, GotoMap &Goto, ofstream &result_syntax_analysis);
void snytax_analysis(string input_string, vector<LR1> &lr1item, vector<Token_info* > Tokens);
void read_grammar(string path, AugmentedGrammar &grammar, vector<LR1> &lr1item, vector<AR *> &orignal_grammar);
void getfirst(bool first[300][300], set<char> Vt, set<char> Vn, vector<AR *> orignal_grammar);
void write_first_set();
void showtable(tabulate::Table table, vector<LR1> &lr1item);
void SDT(string toreduce, stack<string> &semantic, vector<Attribute> &attr);//语法制导翻译
Attribute newtemp();//生成新的临时变量
void Gen(string op, int arg1, int arg2, int dest);
void finalQuad();//对于布尔表达式采用回填技术
int merge(int P1, int P2);//返回并链后的首部，P1，P2代表的是四元式的编号
void backpatch(int P, int t);//回填技术
void translate();
#endif
