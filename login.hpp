#ifndef LOGIN_H
#define LOGIN_H 1

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <vector>
/* csv user record format */
/* @u == user */
/* @d == delimiter */
#define CSV_USER_DATA_RECORD(u, d)                                             \
  u.id << d << u.login << d << u.passwd << d << u.email << d << u.tel_num

using std::string;
using std::vector;

typedef struct {
  uint id;
  string login;
  string passwd;
  string email;
  string tel_num;
} UserData;

class Login {
  const uint PASSWD_MIN_LEN = 8;
  const uint TEL_NUM_LEN = 9;
  const char csv_delim = ',';

  vector<UserData> users;
  UserData user; // current user

  string filepath_userdata;
  string filepath_logs;

public:
  Login();
  ~Login() = default;
  bool sign_up();
  bool log_in();

private:
  bool is_valid_passwd(string s);
  bool is_valid_login(string s);
  bool is_valid_number(string s);
  bool is_valid_email(string s);
  bool is_valid_field(string s);

  void field_enter(string prompt, string *field);
  void remove_whitespc(string *s);

  void csv_load();
  void csv_save();
};

#endif // !LOGIN_H
