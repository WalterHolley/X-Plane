//
// Created by Zero on 8/9/2024.
//

#include <sqlite3.h>
#include <fstream>
#include <chrono>
#include <iostream>
#include "include/Recorder.h"

using namespace std;

sqlite3* db;

bool wasInited = false;
dataFrame* recorderData;

string sessionIdentifier;
string dbName;

string createColumns(vector<dataStruct> columns)
{
    string columnStatement;
    for(auto i = columns.begin(); i != columns.end(); ++i)
    {
        columnStatement += i->index;
        switch(i->unitsEnum)
        {
            case DEG:
            case DEGMIN:
            case INHG:
            case METERSPERSECOND:
            case METERS:
            case FEET:
            case KTIAS:
            case KTS:
            case HZ:
            case MHZ:
            case RATIO:
            case FLOATINGPOINT:
            case SECONDS:
                columnStatement += " REAL";
                break;
            case TEXT:
                columnStatement += " TEXT";
                break;
            case INTEGER:
                columnStatement += " INTEGER";
                break;
            case TRUEFALSE:
                columnStatement += " NUMERIC";
                break;
        }

        //add statement separator
        if(i != columns.end())
        {
            columnStatement += ", ";
        }

    }

    return columnStatement;
}

string createTable(dataElement element)
{
    string createStatement;
    const char*  idRow = "ID INT PRIMARY KEY NOT NULL, TIMESTAMP NUMERIC NOT NULL, ";
    const char* stateKeyRow = "STATE_ID INT NOT NULL, ";
    createStatement = "CREATE TABLE ";

    switch(element)
    {
        case STATE:
            createStatement += "STATES (";
            createStatement += idRow;
            createStatement += createColumns(recorderData->state);
            break;
        case INPUTS:
            createStatement += "INPUTS (";
            createStatement += idRow;
            createStatement += stateKeyRow;
            createStatement += createColumns(recorderData->inputs);
            break;
        case INSTRUCTIONS:
            createStatement += "INSTRUCTIONS (";
            createStatement += idRow;
            createStatement += stateKeyRow;
            createStatement += createColumns(recorderData->instructions);
            break;
        case FAILURES:
            createStatement += "FAILURES (";
            createStatement += idRow;
            createStatement += stateKeyRow;
            createStatement += createColumns(recorderData->failures);
            break;
    }

    createStatement += ");";

    return createStatement;

}



bool fileExists(string &fileName)
{
    ifstream f(fileName.c_str());
    return f.good();
}

//=====PUBLIC METHODS=====//

Recorder::Recorder(std::string &sessionId, dataFrame* df, Logger* logger) {
    sessionIdentifier = sessionId;
    recorderData = df;
    _log = logger;

}

bool Recorder::init()
{

    if(!wasInited)
    {
        dbName = sessionIdentifier + ".db";
        bool  dbExisted = fileExists(dbName);
        ec = sqlite3_open(dbName.c_str(), &db);

        if(ec)
        {
            char* errMsg;
            sprintf(errMsg, "Could not open db: %d", ec);
            _log->error(errMsg);
        }
        else if(!dbExisted)
        {
            wasInited = writeSchema();
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
    char* errMsg;
    if(recorderData)
    {
        if(((!ec) || ec == SQLITE_OK) && wasInited)
        {
            uint64_t t = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::now()).time_since_epoch().count();
            string statement = createInsertStatement(recorderData->state, dataElementString[dataElement::STATE], t, t);

            ec = sqlite3_exec(db, statement.c_str(), NULL, 0, &errMsg);

            if((!ec) || ec == SQLITE_OK)
            {
                 sqlite3_uint64 rowId = sqlite3_last_insert_rowid(db);
                 statement = createInsertStatement(recorderData->inputs, dataElementString[dataElement::INPUTS], t, rowId);
                 statement += createInsertStatement(recorderData->instructions, dataElementString[dataElement::INSTRUCTIONS], t, rowId);
                 statement += createInsertStatement(recorderData->failures, dataElementString[dataElement::FAILURES], t, rowId);
                 ec = sqlite3_exec(db, statement.c_str(), NULL, 0, &errMsg);

            }
            else
            {
                _log->error("Could not insert state: " + string(errMsg));
            }

            if(ec != SQLITE_OK)
            {
                _log->error("Could not write to supporting tables: " +string(errMsg));
            }
        }

    }

}

//********PRIVATE METHODS********//
bool Recorder::writeSchema()
{
    char* errMsg;
    bool result = false;
    string schema = createTable(dataElement::STATE);
    schema += createTable(dataElement::INPUTS);
    schema += createTable(dataElement::INSTRUCTIONS);
    schema += createTable(dataElement::FAILURES);

    _log->debug("SQL Create Table Statement: " + schema);

    int code = sqlite3_exec(db, schema.c_str(), NULL, 0, &errMsg);

    if(code != SQLITE_OK)
    {
        _log->error("DB Schema could not be implemented");
    }
    else
    {
        _log->info("DB setup successful");
        result = true;
    }

    return result;
}

string Recorder::createInsertStatement(vector<dataStruct> values, const char* &tableName, uint64_t &timestamp, uint64_t &stateId)
{
    string insertStatement;
    string columnsSection = "(TIMESTAMP, ";
    string valuesSection = "VALUES(" + to_string(timestamp) + ", ";

    insertStatement = "INSERT INTO " + string(tableName) + " ";

    switch(stringToDataElement[tableName])
    {
        case STATE:
            break;
        case INSTRUCTIONS:
        case INPUTS:
        case FAILURES:
            columnsSection += "STATE_ID, ";
            valuesSection += to_string(stateId) + ", ";
            break;
    }

    for(auto i = values.begin(); i != values.end(); ++i)
    {
        columnsSection += i->index;
        if(i->unitsEnum == units::TEXT)
        {
            valuesSection += "'" + i->value + "'";
        }
        else
        {
            valuesSection += i->value;
        }


        if(i != values.end())
        {
            columnsSection += ", ";
            valuesSection += ", ";
        }
        else
        {
            columnsSection += ")";
            valuesSection += ")";
        }
    }

    insertStatement += columnsSection + valuesSection + ";";
    _log->debug("INSERT Statement: " + insertStatement);

    return insertStatement;
}

//***Destructor
Recorder::~Recorder()
{
    sqlite3_close(db);

    if(db)
    {
        free(db);
    }
}
