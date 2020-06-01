#include "client_api.h"

int main(void) {

  int u_id = addUser("Steven", "pw");
  cout << "u_id:" << u_id << endl;

  int j_id = addJournal(u_id,"title_1","body_1");
  j_id = addJournal(u_id,"title_2","body_2");

  u_id = addUser("tina", "pw");
  cout << "u_id:" << u_id << endl;
  
  j_id = addJournal(u_id,"title_3","body_3");
  j_id = addJournal(u_id,"title_4","body_4");
  j_id = addJournal(u_id,"title_5","body_5");
  cout << "j_id:" << j_id << endl;

  modJournal(3, 2, "mod title_3", "mod body_3");
  json j = getJournal(3, u_id); //j_id,u_id
  cout << j << endl;

  u_id = checkUser("tina", "pw");
  cout << "u_id:" << u_id << endl;

  delJournal(5, u_id); //j_id,u_id

  json j2 = getJournalsByUser(2);
  cout << j2 << endl;

  json j3 = getJournalsByDates("2020-5-21","2020-5-31");
  cout << j3 << endl;
	
	return 0;
}