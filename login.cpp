#include "login.hpp"
#include <algorithm>
#include <cstdlib>
#include <string>

/* @Constructors, @Destructors */
Login::Login() {
	this->filepath_userdata = "./login_credentials.csv";
	this->filepath_logs = "./login_logs.csv";
	csv_load();
}

/* @CSV */
void Login::csv_save() {
	std::ofstream csv(this->filepath_userdata, std::ios_base::app);
	if (!csv.is_open())
		throw std::runtime_error("Failed to open a file!");
	if (csv.good()) {
		csv << CSV_USER_DATA_RECORD(this->user, this->csv_delim)
		    << '\n';
	}
	csv.close();
}

void Login::csv_load() {
	std::ifstream csv(this->filepath_userdata);
	if (!csv.is_open())
		throw std::runtime_error("Failed to open a file!");

	this->users.clear();
	UserData record;

	string line, dummy;
	int i = 0;
	while (csv.good()) {
		/* ignoring headers */
		if (i == 0) {
			std::getline(csv, line, '\n');
			i++;
			continue;
		}
		/* converts str to a number */
		std::getline(csv, dummy, this->csv_delim);
		char *rest;
		record.id = std::strtoul(dummy.c_str(), &rest, 10);
		/* record.id = std::stoi(dummy); */

		std::getline(csv, record.login, this->csv_delim);
		std::getline(csv, record.passwd, this->csv_delim);
		std::getline(csv, record.email, this->csv_delim);
		std::getline(csv, record.tel_num, this->csv_delim);
		this->users.push_back(record);
	}
	csv.close();
}

/* @Field_validation */

/* checks if a field @s contains a illegal characters */
bool Login::is_valid_field(string s) {
	string invalid = " \t\n";
	invalid += this->csv_delim;
	return s.find_first_of(invalid) == string::npos && !s.empty();
}

bool Login::is_valid_number(string s) {
	remove_whitespc(&s);

	return s.size() == this->TEL_NUM_LEN &&
	       s.find_first_not_of("0123456789") == string::npos;
}

bool Login::is_valid_email(string s) {
	const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
	return is_valid_field(s) && std::regex_match(s, pattern);
}

bool Login::is_valid_passwd(string s) {
	return is_valid_field(s) && s.size() >= PASSWD_MIN_LEN;
}

void Login::field_enter(string prompt, string *field) {
	std::cout << prompt;
	std::getline(std::cin, *field);
	std::system("clear");
}
/* removes whitespace and changes the length of the string */
void Login::remove_whitespc(string *s) {
	s->erase(std::remove_if(s->begin(), s->end(), isspace), s->end());
}

bool Login::log_in() {
	string login, passwd;

	field_enter("Login: ", &login);
	field_enter("Password: ", &passwd);

	if (login.empty() || passwd.empty())
		return false;

	for (auto u : this->users) {
		if (u.login == login && u.passwd == passwd)
			return true;
	}
	std::cout << "Invalid login or password!\n";

	return false;
}

bool Login::sign_up() {
	string login, passwd, email, tel_num;
	uint id = this->users.size();

	while (!is_valid_field(login)) {
		field_enter("Login: ", &login);
	}
	while (!is_valid_passwd(passwd)) {
		field_enter("Password: ", &passwd);
	}
	while (!is_valid_email(email)) {
		field_enter("Email: ", &email);
	}
	while (!is_valid_field(tel_num)) {
		field_enter("Number: ", &tel_num);
	}

	this->user = {
	    .id = id,
	    .login = login,
	    .passwd = passwd,
	    .email = email,
	    .tel_num = tel_num,
	};

	csv_save();
	return true;
}

