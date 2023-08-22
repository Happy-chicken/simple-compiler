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
extern char start;	 // ��ʼ����
extern set<char> Vt; // terminal symbol
extern set<char> Vn; // non-terminal symbol
extern bool first[][300];
extern FF First; // first set
const int boolOffest = 10000;
extern map<string, int> entry;//���ű����
extern int tempvarnum;//��ʱ������
extern int NXQ;
extern vector <int> tmp_nxq;
// extern stack<Attribute> attr;//����ջ
// �����ķ�
typedef struct AugmentedReg
{
	char left_reg;	  // ������
	string right_reg; // �����Ҳ�
	string search;
	// overload the construct function                                                                                                          //��ǰ������
	AugmentedReg() {}																										   // Ĭ�Ϲ��캯��
	AugmentedReg(char left_reg, string right_reg) : left_reg(left_reg), right_reg(right_reg) {}								   // ���ι��캯��
	AugmentedReg(char left_reg, string right_reg, string search) : left_reg(left_reg), right_reg(right_reg), search(search) {} // ���ι��캯��
} AR;

// ��Ԫʽ
struct Quad
{

	Quad() = default;
	Quad(string o, int l, int r, int d)
		: opt(o), lhs(l), rhs(r), dest(d) {}

	string opt;	 // ������
	int lhs;	 // �������
	int rhs;	 // �Ҳ�����
	int dest; // Ŀ�������ű��ַ
};
extern vector<Quad> quads;//��Ԫʽ����
//����
struct Attribute
{
	Attribute() = default;
	Attribute(string name):Varname(name) {}
	string Varname;//������
	string value;//����ֵ
	int place{-1};//���ű��е�λ��
	int TC{-1};//�������ʽ������ڣ����ڻ���
	int FC{-1};//�ٳ���
	int chain{-1};//if�������
	int head{-1};//do-while
	int quad{-1};//while
};
extern vector<Attribute> symbol_table;//���ű�����
class LR1
{
private:
	vector<AugmentedReg *> reg; // ����ʽ����

public:
	map<char, int> gotos; // ���߱�
	string reduce_reg = "";
	LR1() {}								   // ���캯��
	~LR1() {}								   // ��������
	void Push(AugmentedReg *p);				   // ��Ӳ���ʽ
	int Size();								   // ���ز���ʽ������
	bool Contains(string _reg, string search); // �жϲ���ʽ�����Ƿ��������ʽ��Ԫ��_reg������ǰ��������
	AugmentedReg *operator[](const int index); // ����ָ�������,������i������ʽ
};
void add_closure(char lookahead, string belta, set<char> search, LR1 &item, AugmentedGrammar &grammar);
bool is_eplision(char lookahead, AugmentedGrammar &grammar);
void LR1_snytax_analysis(vector<LR1> &lr1item, AugmentedGrammar &grammar, int &itemid, GotoMap &Goto, ofstream &result_syntax_analysis);
void snytax_analysis(string input_string, vector<LR1> &lr1item, vector<Token_info* > Tokens);
void read_grammar(string path, AugmentedGrammar &grammar, vector<LR1> &lr1item, vector<AR *> &orignal_grammar);
void getfirst(bool first[300][300], set<char> Vt, set<char> Vn, vector<AR *> orignal_grammar);
void write_first_set();
void showtable(tabulate::Table table, vector<LR1> &lr1item);
void SDT(string toreduce, stack<string> &semantic, vector<Attribute> &attr);//�﷨�Ƶ�����
Attribute newtemp();//�����µ���ʱ����
void Gen(string op, int arg1, int arg2, int dest);
void finalQuad();//���ڲ������ʽ���û����
int merge(int P1, int P2);//���ز�������ײ���P1��P2���������Ԫʽ�ı��
void backpatch(int P, int t);//�����
void translate();
#endif
