#ifndef CLIENT_API_H
#define CLIENT_API_H

// C++ includes
#include <iostream>
#include <string>
using namespace std;

// Extra library
#include "json.hpp"
using json = nlohmann::json;

static const string url = "ws://localhost:4567";

/*
Check if user exist or not
@return : user_id / 0 if not exist
*/
int checkUser(string user_name, string password);

/*
Add new user
@return : user_id
*/
int addUser(string user_name, string password);

/*
Add new journal
@return : journal_id
*/
int addJournal(int user_id, string title, string body);

/*
Modify journal
@return :result true:1 /false:0
*/
int modJournal(int journal_id, int user_id, string title, string body);

/*
Delete journal
@return : result true:1 /false:0
*/
int delJournal(int journal_id, int user_id);

/*
Get a journal
@return : journal data (JSON)
*/
json getJournal(int journal_id, int user_id);

/*
Get journals
@return : journals data (JSON)
*/
json getJournalsByUser(int user_id);

/*
Get journal entries
@return : journal data (JSON)
*/
json getJournalsByDates(string startDate, string endDate);

#endif  // CLIENT_API_H