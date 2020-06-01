
// C++ includes
#include "utils.h"
using namespace std;


// for convenience
using json = nlohmann::json;

const string CCommand::ADD_USER           = "add_user";
const string CCommand::CHK_USER           = "chk_user";
const string CCommand::ADD_JOURNAL        = "add_journal";
const string CCommand::DEL_JOURNAL        = "del_journal";
const string CCommand::MOD_JOURNAL        = "mod_journal";
const string CCommand::GET_JOURNAL        = "get_journal";
const string CCommand::GET_JOURNALS_USER  = "get_journal_user";
const string CCommand::GET_JOURNALS_DATES = "get_journal_dates";

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(const string &s) {
  std::size_t lb = s.find_first_of('[');
  std::size_t rb = string::npos;
  string result = "";
  if (lb < string::npos) {
    int lcnt = 1;
    int rcnt = 0;
    std::size_t slen = s.length();

    for (std::size_t i=lb+1; i<slen; i++) {
      if (s[i] == ']') { rcnt ++; }
      if (lcnt == rcnt) { 
        rb = i;
        break; 
      };
      if (s[i] == '[') { lcnt ++; }
    }
    if (lcnt == rcnt) {
      result = s.substr(lb, rb-lb+1);
    } else {
      result = "";
    }
  }
  return result;
}


