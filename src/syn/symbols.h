#pragma once
#include <unordered_map>
#include <string>
#include <memory>

// Ĭ�ϳ�ʼֵΪ 0
class Id {
public:
	enum Type{
		INT = 0,
		DOUBLE
	};
	Id(const std::string &id = std::string(), const Type &t = INT, const double& val = 0) : name(id), type(t), value(val) {}
	~Id() = default;

	std::string name;	// ��ʶ��
	Type type;
	double value;
};

// ���ű�
class SymTable {
public:
	SymTable() = default;
	bool insert(const Id& id)		// �ڷ��ű��в���һ��
	{
		if (table.find(id.name) != table.end())//������ű����Ѿ�����name�����ʶ��
		{
			return false;
		}
		else {
			table[id.name] = id;
			return true;
		}
	}
	void erase(const std::string& name)
	{
		table.erase(name);
	}
	bool get_info(const std::string& name, Id& id) const
	{
		if (table.find(name) == table.end())
		{
			return false;
		}
		else {
			id = table.at(name);
			return true;
		}
	}
	bool set_val(const std::string& name, double val)
	{
		if (table.find(name) == table.end())
		{
			return false;
		}
		else {
			table[name].value = val;
			return true;
		}
	}

	std::unordered_map<std::string, Id> table;
};



