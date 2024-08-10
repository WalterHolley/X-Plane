//
// Created by Zero on 8/9/2024.
//

#include <sqlite3.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "include/Recorder.h"

using namespace std;

sqlite3* db;
int ec;
bool wasInited = false;
dataFrame* recorderData;
string sessionIdentifier;
string dbName;

string createColumns(vector<dataStruct> columns)
{
    string columnStatement;
    for(vector<dataStruct>::iterator i = columns.begin(); i != columns.end(); ++i)
    {
        columnStatement += i->index;
        switch(i->units)
        {

        }
    }



}

string createTable(dataElement &element)
{
    string createStatement;
    const char*  idRow = "ID INT PRIMARY KEY NOT NULL, ";
    const char* stateKeyRow = "state_id INT NOT NULL, ";
    createStatement = "CREATE TABLE ";

    switch(element)
    {
        case STATE:
            createStatement += "STATES (";
            createStatement += idRow;
            break;
        case INPUTS:
            createStatement += "INPUTS (";
            createStatement += idRow;
            createStatement += stateKeyRow;
            break;
        case INSTRUCTIONS:
            createStatement += "INSTRUCTIONS (";
            createStatement += idRow;
            createStatement += stateKeyRow;
            break;
        case FAILURES:
            createStatement += "FAILURES (";
            createStatement += idRow;
            createStatement += stateKeyRow;
            break;
    }

}

void writeSchema()
{
    stringstream  ss;
}

bool fileExists(string &fileName)
{
    ifstream f(fileName.c_str());
    return f.good();
}

//=====PUBLIC METHODS=====//

Recorder::Recorder(std::string &sessionId, dataFrame* df) {
    dbName = sessionId + ".db";
    sessionIdentifier = sessionId;
    recorderData = df;
}

bool Recorder::init()
{

    bool  dbExisted = fileExists(dbName);
    if(!wasInited)
    {
        ec = sqlite3_open(dbName.c_str(), &db);

        if(ec)
        {
            //TODO: log fail

        }
        else if(!dbExisted)
        {
            //TODO: write schema
        }
        else
        {
            wasInited = true;
        }
    }

    return wasInited;
}

void Recorder::write()
{
    if(recorderData)
    {
        if(!ec)
        {

            //TODO: write frame
        }
    }

}
Recorder::~Recorder()
{
    sqlite3_close(db);

    if(db)
    {
        free(db);
    }
}

