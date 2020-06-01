
#include "journal.h"
#include "utils.h"


int CJournal::journalCount = 0;
int CUser::userCount = 0;
CConfig gConfig(CJournal::journalCount, CUser::userCount);


int CJournal::addJournal(int userId, string title, string body) {
	//create new journal
	CJournal jo(time(0), title, body);
	json jAdd;
	jAdd[0]["journal_id"] = jo.journalId_;
	jAdd[0]["date"] = jo.time_;
	jAdd[0]["title"] = jo.title_;
	jAdd[0]["body"] = jo.body_;

	string file_name = "journal_" + to_string(userId) + ".json";
	std::ifstream iFile(file_name);
	bool foundFile = false;
	json jFile;
	if (iFile.is_open()) {
		foundFile = true;
		//get old journals from file
		jFile = json::parse(iFile);
		//Add to old journals
		jFile.insert(jFile.end(), jAdd.begin(), jAdd.end());
	}

	std::ofstream oFile(file_name);
	assert (!oFile.fail());
	if (foundFile) 
		oFile << std::setw(4) << jFile << std::endl;
	else
		oFile << std::setw(4) << jAdd << std::endl;

	//update related data
	CJournalEntry entry(timeToUTCdate(jo.time_), jo.journalId_, userId, title);
	CJournalEntry::addJournalEntry(entry);

	CConfig::setConfig();

	return jo.journalId_;
}

json CJournal::getJournals(int userId) {
	string file_name = "journal_" + to_string(userId) + ".json";
	std::ifstream iFile(file_name);

	auto j = json::parse(iFile);
	for (json::iterator it = j.begin(); it != j.end(); ++it) {
		json i = it.value();
		#if DEBUG
		std::cout << i["journal_id"].get<int>() << "\n";
		std::cout << i["date"].get<time_t>() << "\n";
		std::cout << i["title"].get<std::string>() << "\n";
		std::cout << i["body"].get<std::string>() << "\n";
		#endif
	}

	return j;
}

json CJournal::getJournals(string startDate, string endtDate) {
	json jAllEntries = json::array();
	vector<string> datelist;
	datelist = datelistBetweenTwoDates(startDate, endtDate);

	for(string date : datelist){
		json jTmp = CJournalEntry::getJournalEntries(date);
		jAllEntries.insert(jAllEntries.end(),jTmp.begin(),jTmp.end());
	}
	#if DEBUG
	cout << "AllEntries:"<< jAllEntries << endl;
	#endif

	return jAllEntries;
}

json CJournal::getJournal(int userId, int journalId) {
	json jResult = json::array();
	string file_name = "journal_" + to_string(userId) + ".json";
	std::ifstream iFile(file_name);

	auto j = json::parse(iFile);
	for (json::iterator it = j.begin(); it != j.end(); ++it) {
		json i = it.value();
		if (i["journal_id"].get<int>() == journalId) {
			jResult.push_back(*it);
			#if DEBUG
			std::cout << i["journal_id"].get<int>() << "\n";
			std::cout << i["date"].get<time_t>() << "\n";
			std::cout << i["title"].get<std::string>() << "\n";
			std::cout << i["body"].get<std::string>() << "\n";
			#endif
		}
	}

	return jResult;
}

int CJournal::delJournal(int userId, int journalId) {
	json jResult = json::array();
	string file_name = "journal_" + to_string(userId) + ".json";
	std::ifstream iFile(file_name);
	if (!iFile.is_open()) return 0;

	auto j = json::parse(iFile);
	for (json::iterator it = j.begin(); it != j.end(); ++it) {
		json i = it.value();
		if (i["journal_id"]== journalId) {
			time_t t = i["date"].get<time_t>();
			string date = timeToUTCdate(t);
			CJournalEntry::delJournalJournalEntry(date, journalId);
		}else{
			jResult.push_back(*it);
		}
	}

    std::ofstream oFile(file_name);
    assert (!oFile.fail( ));
	oFile << std::setw(4) << jResult << std::endl;

	return 1;
}

int CJournal::modJournal(int userId, int journalId, string title, string body) {
	json jResult = json::array();
	string file_name = "journal_" + to_string(userId) + ".json";
	std::ifstream iFile(file_name);
	if (!iFile.is_open()) return 0;

	auto j = json::parse(iFile);
	for (json::iterator it = j.begin(); it != j.end(); ++it) {
		json i = it.value();
		if (i["journal_id"]!= journalId) {
			jResult.push_back(*it);
		} else {
			//create updated journal
			json jAdd;
			jAdd["journal_id"] = journalId;
			jAdd["date"] = time(0);
			jAdd["title"] = title;
			jAdd["body"] = body;
			jResult.push_back(jAdd);
		}
	}

	//wirte back to file
    std::ofstream oFile(file_name);
    assert (!oFile.fail( ));
	oFile << std::setw(4) << jResult<< std::endl;

	return 1;
}

void CConfig::getConfig() {
	//Get last journalCount, userCount
	std::ifstream ifile("config.json");
	if (ifile) {
		auto j = json::parse(ifile);
		CJournal::journalCount = j["journalCount"].get<int>();
		CUser::userCount = j["userCount"].get<int>();

		gConfig.setJournalCount(CJournal::journalCount);
		gConfig.setUserCount(CUser::userCount);
	}
}

void CConfig::setConfig() {
	//Set last journalCount, userCount
	json j;
	j["journalCount"] = CJournal::journalCount;
	j["userCount"] = CUser::userCount;

	std::ofstream o("config.json");
	o << std::setw(4) << j << std::endl;

	gConfig.setJournalCount(CJournal::journalCount);
	gConfig.setUserCount(CUser::userCount);
}

void CJournalEntry::addJournalEntry(CJournalEntry entry) {
	//create new journal
	json jAdd;
	jAdd[0]["journal_id"] = entry.journalId_;
	jAdd[0]["user_id"] = entry.userId_;
	jAdd[0]["title"] = entry.title_;
	
	string file_name = "entry_" + entry.UTCdate_ + ".json";
	std::ifstream iFile(file_name);
	bool foundFile = false;
	json jFile;
	if (iFile.is_open()) {
		foundFile = true;
		//get old journals from file
		jFile = json::parse(iFile);
		//Add to old journals
		jFile.insert(jFile.end(), jAdd.begin(), jAdd.end());
	}

	std::ofstream oFile(file_name);
	assert (!oFile.fail());
	if (foundFile) 
		oFile << std::setw(4) << jFile << std::endl;
	else
		oFile << std::setw(4) << jAdd << std::endl;
}

json CJournalEntry::getJournalEntries(string UTCdate) {
	json j = json::array();;

	string file_name = "entry_" + UTCdate + ".json";
	std::ifstream iFile(file_name);


	if (iFile.is_open())
		j = json::parse(iFile);

	for (json::iterator it = j.begin(); it != j.end(); ++it) {
		json i = it.value();
		#if DEBUG
		std::cout << i["journal_id"].get<int>() << "\n";
		std::cout << i["user_id"].get<int>() << "\n";
		std::cout << i["title"].get<std::string>() << "\n";
		#endif
	}

	return j;
}

int CJournalEntry::delJournalJournalEntry(string date, int journalId) {
	json jResult = json::array();
	string file_name = "entry_" + date + ".json";
	std::ifstream iFile(file_name);
	if (!iFile.is_open()) return 0;

	auto j = json::parse(iFile);
	for (json::iterator it = j.begin(); it != j.end(); ++it) {
		json i = it.value();
		if (i["journal_id"]!= journalId) {
			jResult.push_back(*it);
		}
	}

    std::ofstream oFile(file_name);
    assert (!oFile.fail( ));
	oFile << std::setw(4) << jResult << std::endl;

	return 1;
}

void CUser::addUser(CUser u) {
	//create new journal
	json jAdd;
	jAdd[0]["user_id"] = u.userId_;
	jAdd[0]["user_name"] = u.userName_;
	jAdd[0]["password"] = u.password_;

	string file_name = "user.json";
	std::ifstream iFile(file_name);
	bool foundFile = false;
	json jFile;
	if (iFile.is_open()) {
		foundFile = true;
		//get old journals from file
		jFile = json::parse(iFile);
		//Add to old journals
		jFile.insert(jFile.end(), jAdd.begin(), jAdd.end());
	}

	std::ofstream oFile(file_name);
	assert (!oFile.fail());
	if (foundFile) 
		oFile << std::setw(4) << jFile << std::endl;
	else
		oFile << std::setw(4) << jAdd << std::endl;

	CConfig::setConfig();
};

int CUser::checkUser(string userName, string password) {
	string file_name = "user.json";
	std::ifstream iFile(file_name);
	if (!iFile.is_open()) return 0;

	auto j = json::parse(iFile);
	for (json::iterator it = j.begin(); it != j.end(); ++it) {
		json i = it.value();
		if (i["user_name"].get<string>() == userName && 
			i["password"].get<string>() == password) {
			return i["user_id"].get<int>();
		}
	}

	return 0;

};

string timeToUTCdate(time_t t){
	string UTCdate = ""; // YYYY-MM-DD

	// convert t to tm struct for UTC
	tm *gmtm = gmtime(&t);

   	UTCdate = to_string(1900 + gmtm->tm_year) + "-" + \
   			  to_string(1 + gmtm->tm_mon) + "-" + \
   			  to_string(gmtm->tm_mday);

   	return UTCdate;
}

time_t UTCdateToTime(string UTCdate) {
	time_t UTCtime;

	string strDateTime = UTCdate + "T0:0:0Z";

   	std::istringstream date_s( strDateTime );
	
	// Create a tm object to store the parsed date and time.
	std::tm dt;

 	// Now we read from buffer using get_time manipulator
 	// and formatting the input appropriately.
 	date_s >> std::get_time(&dt, "%Y-%m-%dT%H:%M:%SZ");

	// Convert the tm structure to time_t value and return.
   	UTCtime = std::mktime(&dt);

	#if DEBUG
	// tm *gmtm = gmtime(&UTCtime);
	// char* dt_s = asctime(gmtm);
 	// cout << "The UTC date and time is:"<< dt_s << endl;
   	#endif

	return UTCtime;
}

int daysBetweenTwoDates(string startDate, string endtDate) {
	//
	// Date format = YYYY-MM-DD
	//
	time_t startTime = UTCdateToTime(startDate);
	time_t endTime = UTCdateToTime(endtDate);
	double difference = 0; 

    if ( startTime != (std::time_t)(-1) && endTime != (std::time_t)(-1) )
    {
        difference = std::difftime(endTime,startTime) / (60 * 60 * 24);
        #if DEBUG
        std::cout << std::ctime(&startTime);
        std::cout << std::ctime(&endTime);
        std::cout << "difference = " << difference << " days" << endl;
        #endif
    }

    return (int)difference;
}


vector<string> datelistBetweenTwoDates(string startDate, string endtDate) {
	vector<string> datelist;

	time_t startTime = min(UTCdateToTime(startDate),UTCdateToTime(endtDate));
	int days = daysBetweenTwoDates(startDate, endtDate);

	for(int i = 0; i <= days; ++i) {
		string date = "";
		struct tm* tm = localtime(&startTime);
	    tm->tm_mday += i;
	    time_t next = mktime(tm);
	    //date = ctime(&next);
	    date = timeToUTCdate(next);
	    datelist.push_back(date);
	}

	return datelist;

}