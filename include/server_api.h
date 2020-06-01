#ifndef SERVER_API_H
#define SERVER_API_H

// Extra library
#include "json.hpp"
using json = nlohmann::json;

/*
Add new user
@return :json format user_id
*/
json cmdAddUser(json jdata);

/*
Check if user exist or not
@return : json format user_id / 0 if not exist
*/
json cmdCheckUser(json jdata);

/*
Add new journal
@return :json format journal_id
*/
json cmdAddJournal(json jdata);

/*
Modify journal
@return : json format result(true:1 /false:0)
*/
json cmdModJournal(json jdata);

/*
Delete journal
@return : json format result(true:1 /false:0)
*/
json cmdDelJournal(json jdata);

/*
Get a journal
@return : journal data (JSON)
*/
json cmdGetJournal(json jdata);

/*
Get journals
@return : journals data (JSON)
*/
json cmdGetJournalsByUser(json jdata);

/*
Get journal entries
@return : journal data (JSON)
*/
json cmdGetJournalsByDates(json jdata);

/*
* handle WebSocket journal service
*/
int journal_service();

#endif // SERVER_API_H