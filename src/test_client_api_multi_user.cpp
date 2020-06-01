#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "client_api.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf ("Usage: %s \n Please key in test user_id:(1 or 2 or 3)\n", argv[0]);
    exit(0);
  } 

  int u_id,j_id;

  u_id = addUser("tina", "pw");
  cout << "u_id:" << u_id << endl;

  j_id = addJournal(u_id,"title_1","body_1");
  cout << "j_id:" << j_id << endl;
  j_id = addJournal(u_id,"title_2","body_2");
  cout << "j_id:" << j_id << endl;
  j_id = addJournal(u_id,"title_3","body_3");
  cout << "j_id:" << j_id << endl;

  u_id = addUser("steven", "pw");
  cout << "u_id:" << u_id << endl;

  u_id = addUser("tom", "pw");
  cout << "u_id:" << u_id << endl;
 

  u_id = stoi(argv[1]);

  if (u_id == 1) {
    modJournal(2, 1, "mod title_2", "mod body_2");
    sleep(5);

    json j = getJournal(3, 1); //j_id,u_id
    cout << j << endl;
    sleep(5);

    checkUser("tina", "pw");
    cout << "u_id:" << u_id << endl;
    sleep(5);

    delJournal(3, 1); //j_id,u_id
    sleep(5);

    json j2 = getJournalsByUser(1);
    cout << j2 << endl;
    sleep(5);

	}

  if (u_id == 2){
    j_id = addJournal(u_id,"title_4","body_4");
    cout << "j_id:" << j_id << endl;
    sleep(5);

    j_id = addJournal(u_id,"title_5","body_5");
    cout << "j_id:" << j_id << endl;
    sleep(5);

    json j3 = getJournalsByDates("2020-6-5","2020-5-25");
    cout << j3 << endl;
    sleep(5);
  }

  if (u_id == 3) {
    j_id = addJournal(u_id,"title_6","body_6");
    cout << "j_id:" << j_id << endl;
    sleep(5);

    j_id = addJournal(u_id,"title_7","body_7");
    cout << "j_id:" << j_id << endl;
    sleep(5);

    j_id = addJournal(u_id,"title_8","body_8");
    cout << "j_id:" << j_id << endl;
    sleep(5);
  }

	return 0;
}