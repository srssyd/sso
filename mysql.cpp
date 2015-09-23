#include "mysql.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

void Database::init(const string &url, const string &username,const string &pwd,const string &table){
	driver = sql::mysql::get_mysql_driver_instance();
	conn = driver->connect(url, username, pwd);
	stmt = conn->createStatement();

	string op = "use ";
	op += table;
	stmt->execute(op);

	prep_query = conn->prepareStatement("select count(*) from user where username=? and pwd=?");
	prep_load = conn->prepareStatement("select * from user limit ?");
	prep_insert = conn->prepareStatement("insert user(username, pwd) values(?, ?)");
	prep_update = conn->prepareStatement("update user set pwd=? where username=?");

	this->url =url;
	this->username = username;
	this->pwd = pwd;
	this->table = table;
}

void Database::update(){
    driver = sql::mysql::get_mysql_driver_instance();
	conn = driver->connect(url, username, pwd);
	stmt = conn->createStatement();

	string op = "use ";
	op += table;
	stmt->execute(op);

	prep_query = conn->prepareStatement("select count(*) from user where username=? and pwd=?");
	prep_load = conn->prepareStatement("select * from user limit ?");
	prep_insert = conn->prepareStatement("insert user(username, pwd) values(?, ?)");
	prep_update = conn->prepareStatement("update user set pwd=? where username=?");

}

bool Database::login(const string &username,const string &pwd){
    if(conn->isClosed())
        update();

	prep_query->setString(1, username);
	prep_query->setString(2, pwd);
	sql::ResultSet* res = prep_query->executeQuery();
	res->first();
    int row = res->getInt(1);
    //printf("db information:%d\n",row);
	if(row> 0 ){
		delete res;
		return true;
	}
	else{
        delete res;
        return false;
	}
}

bool Database::insert(const string &username,const string &pwd){

    if(conn->isClosed())
        update();

	prep_insert->setString(1, username);
	prep_insert->setString(2, pwd);

	sql::ResultSet * output = prep_insert->executeQuery();
	if(!output)
        return false;

	return true;
}

bool Database::update(const string &username,const string &pwd){
    if(conn->isClosed())
        update();
	prep_update->setString(2, username);
	prep_update->setString(1, pwd);

	sql::ResultSet * output = prep_update->executeQuery();
	if(!output)
        return false;

	return true;
}

typedef pair<string,string> pss;

vector<pss> Database::load_users(int limit){

    if(conn->isClosed())
        update();
    vector<pss> result;

    prep_load->setInt(1,limit);

    sql::ResultSet * output = prep_load->executeQuery();

    while(output->next()){
        result.push_back(make_pair(output->getString(1),output->getString(2)));
    }

    return result;

}

Database::~Database(){
	delete stmt;
	delete conn;
}
