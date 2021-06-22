//Copyright 2021 Dailiang
//License GPL 3.0
//Author Dailiang

#ifndef COMMAND_H_
#define COMMAND_H_

#include<string>
#include<vector>
#include "ae.h"

class clientDataBase;
using commandProc = bool(*)(vector<string> &, clientDataBase *);

class commandTable{
public:
	class Table{
	public:
		string cmd;
		commandProc func;
	public:
		Table(string str, commandProc func):cmd(str),func(func){}
	};

private:
	static vector<Table> table;

	commandTable(){
		table.push_back(Table("set", setCommand));
		table.push_back(Table("get", getCommand));
	}

public:
	static bool setCommand(vector<string> &, clientDataBase *);
	static bool getCommand(vector<string> &, clientDataBase *);
	static commandProc lookupCommand(vector<string> &, clientDataBase * clientData);
	static void findArgv(vector<string>& argv, clientDataBase *clientData);
	
	static const commandTable & getCommandTable(){
		static commandTable table;
		return table;
	}

	commandTable & operator=(const commandTable &) = delete;
	commandTable(const commandTable&) = delete;
};

#endif
