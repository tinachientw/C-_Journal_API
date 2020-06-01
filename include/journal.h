#ifndef JOURNAL_H
#define JOURNAL_H

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
using namespace std;

#include "json.hpp"
// for convenience
using nlohmann::json;



class CConfig{
public:
	CConfig(int userCount = 0, int journalCount = 0) {
		userCount_ = userCount;
		journalCount_ = journalCount;
	}

	~CConfig() {
		setConfig();
	}

	static void getConfig();
	static void setConfig();
	void setJournalCount(int journalCount) { journalCount_ = journalCount; }
	void setUserCount(int userCount) { userCount_ = userCount; }
private:
	int journalCount_;
	int userCount_;
};

class CUser{
public:
	static int userCount;

	CUser(string userName = "", string password = "") {
		userId_ = ++userCount;
		userName_ = userName;
		password_ = password;
	}

	static void addUser(CUser user);
	static int checkUser(string userName, string password);
	int getUserId() { return userId_; }

private:
	int userId_;
	string userName_;
	string password_;
};

class CJournal{
public:
	static int journalCount;

	CJournal(string t = "", string title = "", string body = "") {
		journalId_ = ++journalCount;
		time_ = t;
		title_ = title;
		body_ = body;
	}

	static int  addJournal(int userId, string title, string body);
	static json getJournals(int userId);
	static json getJournals(string startDate, string endtDate);
	static json getJournal(int userId, int journalId);
	static int  delJournal(int userId, int journalId);
	static int  modJournal(int userId, int journalId, string title, string body);
private:
	int 	journalId_;
	string 	time_;
	string 	title_;
	string 	body_;
};

class CJournalEntry{
public:
	CJournalEntry(string UTCdate, int journalId, int userId, string title) {
		UTCdate_ = UTCdate;
		journalId_ = journalId;
		userId_ = userId;
		title_ = title;
	}

	static void addJournalEntry(CJournalEntry entry);
	static json getJournalEntries(string UTCdate);
	static int  delJournalJournalEntry(string date, int journalId);
private:
	string 	UTCdate_;
	int 	journalId_;
	int 	userId_;
	string 	title_;
};

string timeToUTCdate(time_t t);

string timeToUTCdatetime(time_t t);

time_t UTCdateToTime(string UTCdate);

int daysBetweenTwoDates(string startDate, string endtDate);

vector<string> datelistBetweenTwoDates(string startDate, string endtDate);

extern CConfig gConfig;


#endif // JOURNAL_H