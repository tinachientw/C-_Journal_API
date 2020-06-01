#ifndef UTILS_H
#define UTILS_H

#ifndef DEBUG
#define DEBUG 0 //1:Show debug msg,0:No debug msg
#endif

// C++ includes
// #include <iostream>
// #include <thread>
// #include <mutex>
#include <string>

#include "json.hpp"

using json = nlohmann::json;

class CCommand {   
public:    
  static const std::string ADD_USER;     
  static const std::string CHK_USER;    
  static const std::string ADD_JOURNAL;
  static const std::string DEL_JOURNAL;
  static const std::string MOD_JOURNAL;
  static const std::string GET_JOURNAL;
  static const std::string GET_JOURNALS_USER;
  static const std::string GET_JOURNALS_DATES;
};

std::string hasData(const std::string &s);

class CJournalMessage {
  public:
    std::string command;
    json   data;
}; 


#endif  // UTILS_H
