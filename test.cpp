#include<bits/stdc++.h>
#include<unistd.h>

using namespace std;

/* string feature_directory = TESTS_RESULTS_DIR + "/" + feature;
if(doesFileExists((feature_directory + "/" + FEATURES_FILE_NAME).c_str()) && doesFileExists((feature_directory + "/" + TESTS_LOG_FILE_NAME).c_str())){
    return 0;

}else if(doesFileExists((feature_directory + "/" + LAUNCHING_FAILED_FILE_NAME).c_str())){
    return -1;
}
 */

int main(){
    ofstream f("testfile.txt");
    f << "hello2" ;
    f.close();

    return 0;
}
