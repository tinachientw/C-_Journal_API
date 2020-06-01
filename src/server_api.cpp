
// C++ includes
#include <iostream>
// #include <thread>
// #include <mutex>
#include <string>

// Extra library
#include <uWS/uWS.h>
#include "utils.h"
#include "journal.h"
#include "server_api.h"


using namespace std;

// std::mutex send_lock;

json cmdAddUser(json jdata){
  int user_id = 0;
  string userName = jdata["user_name"].get<string>();
  string password = jdata["password"].get<string>();
  user_id = CUser::checkUser(userName, password);
  cout << "check chk_user_id:" << user_id << endl;

  if (user_id == 0) {
    CUser u(userName, password);
    user_id = u.getUserId();
    CUser::addUser(u);
  }
  cout << "new u_id:" << user_id << endl;

  return json{{"user_id",to_string(user_id)}};
}

json cmdCheckUser(json jdata){
  string userName = jdata["user_name"].get<string>();
  string password = jdata["password"].get<string>();

  int userId = CUser::checkUser(userName, password);

  return json{{"user_id",to_string(userId)}};
}

json cmdAddJournal(json jdata){
  int userId = stoi(jdata["user_id"].get<string>());
  string title = jdata["title"].get<string>();
  string body = jdata["body"].get<string>();

  int journalId = CJournal::addJournal(userId, title, body);

  return json{{"journal_id",to_string(journalId)}};
}

json cmdModJournal(json jdata){
  int journalId = stoi(jdata["journal_id"].get<string>());
  int userId = stoi(jdata["user_id"].get<string>());
  string title = jdata["title"].get<string>();
  string body = jdata["body"].get<string>();

  int success = CJournal::modJournal(userId, journalId, title, body);

  return json{{"success",to_string(success)}};
}

json cmdDelJournal(json jdata){
  int journalId = stoi(jdata["journal_id"].get<string>());
  int userId = stoi(jdata["user_id"].get<string>());

  int success = CJournal::delJournal(userId, journalId);

  return json{{"success",to_string(success)}};
}

json cmdGetJournal(json jdata){
  json jResult;
  int journalId = stoi(jdata["journal_id"].get<string>());
  int userId = stoi(jdata["user_id"].get<string>());

  jResult = CJournal::getJournal(userId, journalId);

  return jResult;
}

json cmdGetJournalsByUser(json jdata){
  json jResult;
  int userId = stoi(jdata["user_id"].get<string>());

  jResult = CJournal::getJournals(userId);

  return jResult;
}

json cmdGetJournalsByDates(json jdata){
  json jResult;
  string startDate = jdata["start_date"].get<string>();
  string endDate   = jdata["end_date"].get<string>();

  jResult = CJournal::getJournals(startDate, endDate);

  return jResult;
}

int journal_service() {
    gConfig.getConfig();

    uWS::Hub h;
    h.onMessage([&h]
              (uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, 
               uWS::OpCode opCode) {

    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
        if (length && length > 2 && data[0] == '4' && data[1] == '2') {
            auto s = hasData(string(data));

            if (s != "") {

                #if DEBUG
                cout << "[GET]: " << s << endl;
                #endif

                json jdata = json::parse(s);

                string cmd = jdata[0].get<string>();
                CJournalMessage outMesg;
            
                outMesg.command = cmd;
                if (cmd == CCommand::ADD_JOURNAL) {
                  outMesg.data = cmdAddJournal(jdata[1]);
                } else if (cmd == CCommand::MOD_JOURNAL) {
                  outMesg.data = cmdModJournal(jdata[1]);
                } else if (cmd == CCommand::DEL_JOURNAL) {
                  outMesg.data = cmdDelJournal(jdata[1]);
                } else if (cmd == CCommand::GET_JOURNAL) {
                  outMesg.data = cmdGetJournal(jdata[1]);
                } else if (cmd == CCommand::GET_JOURNALS_USER) {
                  outMesg.data = cmdGetJournalsByUser(jdata[1]);
                } else if (cmd == CCommand::GET_JOURNALS_DATES) {
                  outMesg.data = cmdGetJournalsByDates(jdata[1]);
                } else if (cmd == CCommand::ADD_USER) {
                  outMesg.data = cmdAddUser(jdata[1]);
                } else if (cmd == CCommand::CHK_USER) {
                  outMesg.data = cmdCheckUser(jdata[1]);
                }
                string msg = "42[\"" + outMesg.command + "\","+ outMesg.data.dump()+"]";
                #if DEBUG
                std::cout << "[Send]:" << msg << std::endl;
                #endif
                ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
    
                // send_lock.lock(); 
                // send_lock.unlock();
            } //end if (s != "")
        }  // end if

    }); // end h.onMessage


    h.onConnection([&h]
                   (uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
        std::cout << "Connected!!!" << std::endl;
    });

    h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, 
                           char *message, size_t length) {
  
        h.getDefaultGroup<uWS::SERVER>().close();
        std::cout << "Disconnected" << std::endl;
    });

    while (1) {
        int port = 4567;
        if (h.listen(port)) {
          std::cout << "Listening to port " << port << std::endl;
        } else {
          std::cerr << "Failed to listen to port" << std::endl;
          return -1;
        }
        try {
          h.run();
        }
        catch (const std::exception& e) {
          std::cout << e.what() << std::endl;
        }
    }// end while
} // end service
