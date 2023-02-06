#include "run_tests.h"
#include <bits/stdc++.h>
using namespace std;

string POSTGRES_INSTALLATION_DIRECTORY = "/home/postgres/postgresql-14.0";
string TESTS_LOG_FILE_NAME = "tests_log.txt";

void runTests(){
    //start the postgresql server
    system("sudo -u postgres /usr/local/pgsql/bin/pg_ctl -D /usr/local/pgsql/data -l logfile start");

    //delete the previous log file
    system(("rm " + POSTGRES_INSTALLATION_DIRECTORY + "/" + TESTS_LOG_FILE_NAME).c_str());

    //launch the tests
    system(("cd " + POSTGRES_INSTALLATION_DIRECTORY + " && sudo -u postgres make check-world > " + POSTGRES_INSTALLATION_DIRECTORY + "/" + TESTS_LOG_FILE_NAME).c_str());
}