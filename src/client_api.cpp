// C++ includes


// Extra library
#include <uWS/uWS.h>
#include "client_api.h"
#include "utils.h"

json sendMessage(string url, string command, json out_data) 
{
    uWS::Hub h;
    json in_data = json({});
  
    h.onConnection([&command, &out_data](uWS::WebSocket<uWS::CLIENT> ws, uWS::HttpRequest req) {
        string msg = "42[\"" + command + "\","+ out_data.dump()+"]";
        #if DEBUG
        std::cout << "[Send]:" << msg << std::endl;
        #endif
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
    });

    h.onMessage([&h, &in_data](uWS::WebSocket<uWS::CLIENT> ws, char *data, size_t length,
               uWS::OpCode opCode) {

        // "42" at the start of the message means there's a websocket message event.
        // The 4 signifies a websocket message
        // The 2 signifies a websocket event
        if (length && length > 2 && data[0] == '4' && data[1] == '2') {
            auto s = hasData(string(data));
            if (s != "") {
                auto jdata = json::parse(s);
                #if DEBUG
                cout << "[Return]:" << jdata << endl;
                #endif
                
                string cmd = jdata[0].get<string>();
                in_data = jdata[1];
            }
            h.getDefaultGroup<uWS::CLIENT>().close();
        }
    }); // end h.onMessage


    h.connect(url, nullptr);
    h.run();
  
    return in_data;

}


int checkUser(string user_name, string password) {
  json jOut,jIn;
  jOut["user_name"] = user_name;
  jOut["password"] = password;

  jIn = sendMessage(url, CCommand::CHK_USER, jOut);
  #if DEBUG
  cout << jIn << endl;
  cout << jIn["user_id"].get<string>();
  #endif
  return stoi(jIn["user_id"].get<string>());
}


int addUser(string user_name, string password) {
  json jOut,jIn;
  jOut["user_name"] = user_name;
  jOut["password"] = password;

  jIn = sendMessage(url, CCommand::ADD_USER, jOut);
  #if DEBUG
  cout << jIn << endl;
  cout << jIn["user_id"].get<string>();
  #endif
  return stoi(jIn["user_id"].get<string>());
}


int addJournal(int user_id, string title, string body) {
  json jOut,jIn;
  jOut["user_id"] = to_string(user_id);
  jOut["title"] = title;
  jOut["body"] = body;

  jIn = sendMessage(url, CCommand::ADD_JOURNAL, jOut);
  #if DEBUG
  cout << jIn << endl;
  cout << jIn["journal_id"].get<string>();
  #endif
  return stoi(jIn["journal_id"].get<string>());
}


int modJournal(int journal_id, int user_id, string title, string body) {
  json jOut,jIn;
  jOut["journal_id"] = to_string(journal_id);
  jOut["user_id"] = to_string(user_id);
  jOut["title"] = title;
  jOut["body"] = body;


  jIn = sendMessage(url, CCommand::MOD_JOURNAL, jOut);
  #if DEBUG
  cout << jIn << endl;
  cout << jIn["success"].get<string>();
  #endif
  return stoi(jIn["success"].get<string>());
}


int delJournal(int journal_id, int user_id) {
  json jOut,jIn;
  jOut["journal_id"] = to_string(journal_id);
  jOut["user_id"] = to_string(user_id);

  jIn = sendMessage(url, CCommand::DEL_JOURNAL, jOut);
  #if DEBUG
  cout << jIn << endl;
  cout << jIn["success"].get<string>();
  #endif
  return stoi(jIn["success"].get<string>());

}


json getJournal(int journal_id, int user_id){
  json jOut,jIn;
  jOut["journal_id"] = to_string(journal_id);
  jOut["user_id"] = to_string(user_id);

  jIn = sendMessage(url, CCommand::GET_JOURNAL, jOut);
  #if DEBUG
  cout << jIn << endl;
  #endif
  return jIn;
}


json getJournalsByUser(int user_id){
  json jOut,jIn;
  jOut["user_id"] = to_string(user_id);

  jIn = sendMessage(url, CCommand::GET_JOURNALS_USER, jOut);
  #if DEBUG
  cout << jIn << endl;
  #endif
  return jIn;
}


json getJournalsByDates(string startDate, string endDate) {
  json jOut,jIn;
  jOut["start_date"] = startDate;
  jOut["end_date"] = endDate;

  jIn = sendMessage(url, CCommand::GET_JOURNALS_DATES, jOut);
  #if DEBUG
  cout << jIn << endl;
  #endif
  return jIn;
}

