#include"command.h"
//#include<iostream>
#include"server.h"

//using namespace std;

vector<commandTable::Table> commandTable::table;

bool commandTable::setCommand(vector<string> &argv, clientDataBase * clientData){
	Server & server = Server::getServer();

	Server::DATABASE &database = server.getDatabase(clientData->dataNo);

	database[argv[1]] = argv[2];
	//cout << "in set command" << argv[1] << "\t" <<argv[2] << endl;

	argv.clear();
	argv[0] = "ok";

	return true;
}

bool commandTable::getCommand(vector<string> &argv, clientDataBase * clientData){
	Server & server = Server::getServer();

	Server::DATABASE &database = server.getDatabase(clientData->dataNo);

	if(database.find(argv[1]) != database.end()){
		argv.clear();
		argv.push_back(database[argv[1]]);
		//cout << "find success  " << database[argv[1]] << endl;
		return true;
	}
	else{
		argv.clear();
		argv.push_back("can not find");
		//cout << "can not find\n";
	}

	return true;
}

commandProc commandTable::lookupCommand(vector<string> &argv, clientDataBase * clientData){
	//finalFileProc(argv, clientData);
	
	findArgv(argv, clientData);
	if(argv.size() == 0)  return nullptr;

	//printf("look command %s\n", argv[0].data());
	
	for(Table it : table){
		if(it.cmd == argv[0]){
			return it.func;
		}
	}

	return nullptr;
}

void commandTable::findArgv(vector<string>& argv, clientDataBase *clientData){
	const char * buf = clientData->data();

	int i = 0, j = 1;

	
	//serverLog(LOG_DEBUG, "Client readQueryFromClient");

	while(i < clientData->size() && j < clientData->size()){
		for(; i < clientData->size(); i++){
			if(strchr("\t\r\n ", buf[i]) == nullptr)
				break;
		}

		for(j = i + 1; j < clientData->size(); j++){
			if(strchr("\t\r\n ", buf[j]) != nullptr)
				break;
		}

		if(i == clientData->size()) break;
		argv.push_back(string(buf + i, buf + j));

		i = j;
	}

	clientData->clear();
}

