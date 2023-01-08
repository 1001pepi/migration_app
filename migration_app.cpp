/*
By TANANKEM MENKEM Ramses
December 2022
*/
#include <bits/stdc++.h>

using namespace std;

void displayHelp(){
    cout << "./migration_app [action]" << endl;
    cout << "\t action:" << endl;
    cout << "\t\t start: launch the application" << endl;
    cout << "\t\t stop: stop the execution of the application" << endl;
    cout << "\t\t resume: resume the execution of the application" << endl;
}

int main(int argc, char **argv){
    if(argc == 2){
        string action = argv[1];

        if(action == "start"){
            system("./dom0 start");
        }else if(action == "stop"){
            system("killall dom0 && xl destroy myvm");

        }else if(action == "resume"){
            system("./dom0 resume");
        }else{
            cout << "Unknown action!" << endl;
            displayHelp();
        }

    }else{
        cout << "Missing argument!" << endl;
        displayHelp();
    }
    return 0;
}