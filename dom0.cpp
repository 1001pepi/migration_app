/*
By TANANKEM MENKEM Ramses
December 2022
*/
#include <bits/stdc++.h>
#include<unistd.h>
#include <signal.h>
#include <sys/wait.h>

using namespace std;

struct FeaturesList {string list[32];};

//**********************************************************************************************************
//GLOBAL VARIABLES
int REGISTER_SIZE = 32;
string FEATURES_FILE_NAME = "features.txt";
string TESTS_LOG_FILE_NAME = "tests_log.txt";
string LAUNCHING_FAILED_FILE_NAME = "launchingFailed.txt";
string DOMU_NAME = "myvm";
string ORIGINAL_CONFIG_FILE = "myvm.cfg";
string CUSTOM_CONFIG_FILE = "curr_conf.cfg";
string TESTS_RESULTS_DIR  = "tests_results";
string DOMU_DEFAULT_FEATURES_DIR = "domU_default_features";
string APP_USEFUL_FEATURES_FILE = "app_useful_features.txt";
string DOMU_USEFUL_FEATURES_FILE = "domU_useful_features.txt";
string FEATURES_TO_TEST_FILE = "features_to_test.txt";

/*
The structure of the following map is as follow:
{(eax_value, ecx_value, register_to_read), list_of_features}
*/
map<tuple<int32_t, int32_t, string>, FeaturesList> FEATURES_BITS{
    {make_tuple(1, -1, "ecx"), {"hypervisor", "rdrand", "f16c", "avx", "osxsave", "xsave", "aes", "tsc-deadline", "popcnt", "movbe", "x2apic", "sse4.2", "sse4.1", "dca", "pcid", "", "pdcm", "xtpr", "", "fma", "sdbg", "cnxt-id", "ssse3", "tm2", "est", "smx", "vmx", "dscpl", "monitor", "dtes64", "pclmulqdq", "sse3"}},
    {make_tuple(1, -1, "edx"), {"pbe", "ia64", "tm", "htt", "ss", "sse2", "sse", "fxsr", "mmx", "acpi", "ds", "", "clfsh", "psn", "pse36", "pat", "cmov", "mca", "pge", "mtrr", "", "", "apic", "", "mce","pae", "msr", "tsc", "pse", "de", "vme", "fpu"}},
    //{make_tuple(7, 0, "ebx"), {"avx512vl", "avx512bw", "sha", "avx512cd", "avx512er", "avx512pf", "pt", "clwb", "clflushopt", "", "avx512ifma", "smap", "adx", "rdseed", "avx512-dq", "avx512f", "rdt-a", "mpx", "", "rdt-m", "rtm", "invpcid", "erms", "bmi2", "smep", "", "avx2", "hle", "bmi1", "sgx", "tsc_adjust", "fsgsbase"}},
    //{make_tuple(7, 0, "ecx"), {"pks", "sgx-lc", "enqcmd", "movdir64b", "movdiri", "", "cldemote", "", "kl", "rdpid", "", "", "", "", "", "la57", "", "avx512-vpopcntdq", "tme", "avx512-bitalg", "avx512-vnni", "vpclmulqdq", "vaes", "gfni", "cet_ss", "avx512-vbmi2", "waitpkg", "ospke", "pku", "umip", "avx512vbmi", "prefetchwt1"}},
    //{make_tuple(7, 0, "edx"), {"ssbd", "", "", "L1D_FLUSH", "stibp", "IBRS_IBPB / spec_ctrl", "amx-int8", "amx-tile", "avx512-fp16", "amx-bf16", "", "cet-ibt", "lbr", "pconfig", "", "tsxldtrk", "hybrid", "serialize", "", "", "rtm-always-abort", "mc-clear", "srdbs-ctrl", "avx512-vp2intersect", "", "", "uintr", "fsrm", "avx512-4fmaps", "avx512-4vnniw", "", ""}},
    //{make_tuple(7, 1, "eax"), {"", "", "", "", "msrlist", "lam", "", "", "avx-ifma", "hreset", "amx-fp16", "", "wrmsrns", "lkgs", "fred", "", "", "", "", "fast_short_rep_cmpsb_scasb", "fast_short_rep_stosb", "fast_zero_rep_movsb", "", "archperfmonext", "cmpccxadd", "avx-vnni", "rao-int", "", "", ""}},
    //{make_tuple(7, 1, "edx"), {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "prefetchiti", "", "", "", "", "", "", "", "", "avx-ne-convert", "avx-vnn-int8", "", "", "", ""}},
    //eax=0Dh, ecx=1
    //{make_tuple(13, 1, "eax"), {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "xss", "xgetbv_ecx1", "xsavec", "xsaveopt"}},
    //eax=12h, ecx=0
    //{make_tuple(18, 0, "eax"), {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "oss", "", "", "", "sgx2", "sgx1"}},
    //eax=19h
    //{make_tuple(18, -1, "ebx"), {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "kl_msrs", "", "aes_wide_kl", "", "aes_kle"}},
    //eax=80000001h
    //{make_tuple(2147483649, -1, "ecx"), {"", "addr_mask_ext", "monitorx", "pcx_l2i", "perftsc", "dbx", "", "perfctr_nb", "perfctr_core", "topoext", "tbm", "", "nodeid_msr", "", "tce", "fma4", "lwp", "", "wdt", "skinit", "xop", "ibs", "osvw", "3dnowprefetch", "misalignsse", "sse4a", "abm", "cr8_legacy", "extapic", "svm", "cmp_legacy", "lahf_lm"}},
    //eax=80000001h
    //{make_tuple(2147483649, -1, "edx"), {"3dnow", "3dnowext", "lm", "", "rdtscp", "pdpe1gb", "fxsr_opt", "fxsr", "mmx", "mmxext", "", "nx", "mp", "", "pse36", "pat", "cmov", "mca", "pge", "mtrr", "syscall", "", "apic", "cx8", "mce", "pae", "msr", "tsc", "pse", "de", "vme", "fpu"}},
    //eax=8000001Fh
    //{make_tuple(2147483679, -1, "eax"), {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "","vte", "prevent_host_ibs", "debug_swap", "alternate_injection", "restricted_injection", "64_host", "hw_cache_coherency", "", "", "", "", "vmpl", "sev_snp", "sev_es", "page_flush", "sev", "sme"}},
    //eax=80000021h
    //{make_tuple(2147483681, -1, "eax"), {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "CpuidUserDis", "", "", "", "PrefetchCtlMsr", "", "", "", "NoSmmCtlMSR", "AutomaticIBRS", "UpperAddressIgnore", "NullSelectClearsBase", "", "", "SmmPgCfgLock", "LFenceAlwaysSerializing", "", "NoNestedDataBp"}}
};

map<string, vector<string>> FEATURES_DEPENDENCIES{
    {"mmx", {"fpu"}},
    {"pse36", {"pse"}},
    {"lm", {"pae"}},
    {"nx", {"pae"}},
    {"tsc-deadline", {"tsc", "apic"}},
    {"rdtscp", {"tsc"}},
    {"tsc_adjust", {"tsc"}},
    {"x2apic", {"apic"}},
    {"extapic", {"apic"}},
    {"mmxext", {"mmx"}},
    {"3dnow", {"mmx"}},
    {"sse", {"fxsr"}},
    {"sse2", {"sse"}},
    {"misalignesse", {"sse"}},
    {"sse3", {"sse2"}},
    {"lm", {"sse2"}},
    {"pclmulqdq", {"sse2"}},
    {"sha", {"sse2"}},
    {"ssse3", {"sse3"}},
    {"sse4a", {"sse3"}},
    {"sse4.1", {"ssse3"}},
    {"sse4.2", {"sse4.1"}},
    {"avx", {"xsave"}},
    {"mpx", {"xsave"}},
    {"pku", {"xsave", "lm"}},
    {"lwp", {"xsave"}},
    {"fma", {"avx"}},
    {"fma4", {"avx"}},
    {"f16c", {"avx"}},
    {"avx2", {"avx"}},
    {"xop", {"avx"}},
    {"smap", {"nx"}},
    {"pcid", {"lm"}},
    {"3dnowext", {"3dnow"}},
    {"avx512f", {"avx2"}},
    {"avx512dq", {"avx512f"}},
    {"avx512ifma", {"avx512f"}},
    {"avx512pf", {"avx512f"}},
    {"avx512er", {"avx512f"}},
    {"avx512cd", {"avx512f"}},
    {"avx512bw", {"avx512f"}},
    {"avx512vl", {"avx512f"}},
    {"avx512-4fmaps", {"avx512f"}},
    {"avx512-4vnniw", {"avx512f"}},
    {"avx512vbmi", {"avx512f"}}
};
//**************************************************************

//***************************************************************
//CUSTOM FUNCTIONS
//function to create a copy of a file
void copyFile(string origin, string destination){
    string line;
    // For writing text file
    // Creating ofstream & ifstream class object
    ifstream ini_file{ origin }; // This is the original file
    ofstream out_file{ destination };

    if (ini_file && out_file) {
        while (getline(ini_file, line)) {
            out_file << line << "\n";
        }
    }else {
        // Something went wrong
        printf("An error occurred while reading the origin file");
    }
    // Closing file
    ini_file.close();
    out_file.close();
}

//function to add a string at the end of a file
void writeAtEnd(string file, string text){
    ofstream fileOUT(file, ios::app); // open file in append mode

    fileOUT << text << endl; // append the text to the end of the file

    fileOUT.close(); // close the file
}

//function to check if a file exists
bool doesFileExists(const char *fileName){
    ifstream infile(fileName);
    return infile.good();
}

//function to get the default features of the domU
int getDomUDefaultFeatures(string config_file){
    system(("rm " + FEATURES_FILE_NAME).c_str());
    system(("rm " + LAUNCHING_FAILED_FILE_NAME).c_str());
    system(("xl destroy " + DOMU_NAME).c_str());

    pid_t child_pid = fork();

    if(child_pid == 0){
        int status = system(("xl create -c " + config_file).c_str());
        int statusCode = WEXITSTATUS(status);
        if(statusCode != 0){
            ofstream failureFile(LAUNCHING_FAILED_FILE_NAME);
            failureFile.close();
        }else{
            if(!doesFileExists(FEATURES_FILE_NAME.c_str()) && !doesFileExists((DOMU_DEFAULT_FEATURES_DIR + "/" + FEATURES_FILE_NAME).c_str())){
                ofstream failureFile(LAUNCHING_FAILED_FILE_NAME);
                failureFile.close();
            }
        }
        kill(getpid(), SIGKILL);
    }else{
       
        while(!doesFileExists(LAUNCHING_FAILED_FILE_NAME.c_str()) && !doesFileExists(FEATURES_FILE_NAME.c_str())){
            this_thread::sleep_for(chrono::milliseconds(1000));
        }

        int status = system(("xl destroy " + DOMU_NAME).c_str());

        if(doesFileExists(FEATURES_FILE_NAME.c_str())){
            return 0;
        }else if(doesFileExists(LAUNCHING_FAILED_FILE_NAME.c_str())){
            return -1;
        }
    }

    return -1;
}

//fonction pour lire les fichiers par défaut du domU
map<tuple<int32_t, int32_t, string>, vector<string>> readDomUDefaultFeatures(){
    map<tuple<int32_t, int32_t, string>, vector<string>> availableFeatures;

    ifstream featuresFile(("./domU_default_features/" + FEATURES_FILE_NAME).c_str());
    string text;

    while(getline(featuresFile, text)){
        string delimiter = " ";
        size_t pos = 0;
        string token;
        text = text + " ";

        //read eax_val
        pos = text.find(delimiter);
        int eax_val = stoi(text.substr(0, pos));
        text.erase(0, pos + delimiter.length());

        //read ecx_val
        pos = text.find(delimiter);
        int ecx_val = stoi(text.substr(0, pos));
        text.erase(0, pos + delimiter.length());

        //read the register to read
        pos = text.find(delimiter);
        string register_to_read = text.substr(0, pos);
        text.erase(0, pos + delimiter.length());

        //read the list of features
        tuple<int32_t, int32_t, string> key = make_tuple(eax_val, ecx_val, register_to_read);
        availableFeatures[key] = {};

        while((pos = text.find(delimiter)) != string::npos){
            token = text.substr(0, pos);
            availableFeatures[key].push_back(token);
            text.erase(0, pos + delimiter.length());
        }
    }

    featuresFile.close();

    return availableFeatures;
} 

//fonction pour ordonner les features pour une exploration optimale
vector<string> orderFeatures(vector<string> featuresList){
    vector<string> keys;
    vector<string> res_keys = {}, res_vals = {};

    for(auto it = FEATURES_DEPENDENCIES.begin(); it != FEATURES_DEPENDENCIES.end(); it++){
        keys.push_back(it->first);
    }

    for(auto feature = featuresList.begin(); feature != featuresList.end(); feature++){
        if(find(keys.begin(), keys.end(), *feature) != keys.end()){
            res_keys.push_back(*feature);
        }else{
            res_vals.push_back(*feature);
        }
    }

    res_keys.insert(res_keys.end(), res_vals.begin(), res_vals.end());

    return res_keys;
}

//function to rest the test of a feature
int runFeatureTest(string config_file, string feature){
    system(("rm " + FEATURES_FILE_NAME).c_str());
    system(("rm " + LAUNCHING_FAILED_FILE_NAME).c_str());
    system(("xl destroy " + DOMU_NAME).c_str());

    pid_t child_pid = fork();

    if(child_pid == 0){
        int status = system(("xl create -c " + config_file).c_str());
        int statusCode = WEXITSTATUS(status);

        cout << "statuscode from child: " << statusCode  << endl;
        if(statusCode != 0){
            ofstream failureFile(LAUNCHING_FAILED_FILE_NAME);
            failureFile.close();
        }else{
            string feature_directory = TESTS_RESULTS_DIR + "/" + feature;
            if(!doesFileExists(FEATURES_FILE_NAME.c_str()) && !doesFileExists((feature_directory + "/" + FEATURES_FILE_NAME).c_str())){
                ofstream failureFile(LAUNCHING_FAILED_FILE_NAME);
                failureFile.close();
            }
        }

        kill(getpid(), SIGKILL);
    }else{
       
        while(!doesFileExists(LAUNCHING_FAILED_FILE_NAME.c_str()) && !(doesFileExists(FEATURES_FILE_NAME.c_str()) && doesFileExists(TESTS_LOG_FILE_NAME.c_str()))){
            this_thread::sleep_for(chrono::milliseconds(500));
        }

        int status = system(("xl destroy " + DOMU_NAME).c_str());

        if(doesFileExists(FEATURES_FILE_NAME.c_str()) && doesFileExists(TESTS_LOG_FILE_NAME.c_str())){
            return 0;

        }else if(doesFileExists(LAUNCHING_FAILED_FILE_NAME.c_str())){
            return -1;
        }
    }

    return -1;
}

//fonction pour évaluer les résultats des tests suites contenus dans le fichier log
bool evalLogs(string filePath){
    string PATTERN = "All 209 tests passed.";
    ifstream featuresFile(filePath.c_str());
    string line;
    int count = 0;

    while(getline(featuresFile, line)){
        if(line.find(PATTERN, 0) != string::npos){
            count++;
        }

        if(count == 2){
            return true;
        }
    }

    return false;
}

//fonction pour écrire le résultat de test d'une feature dans les fichiers logs
vector<string> backupResult(string fileName, vector<string> currFeaturesList, string feature){
    writeAtEnd(fileName, feature);

    vector<string> children = FEATURES_DEPENDENCIES[feature];

    for(auto child = children.begin(); child != children.end(); child++){
        if(find(currFeaturesList.begin(), currFeaturesList.end(), *child) != currFeaturesList.end()){
            writeAtEnd(fileName, *child);
            auto invalid = remove(currFeaturesList.begin(), currFeaturesList.end(), *child);
            currFeaturesList.erase(invalid, currFeaturesList.end());
        }
    }

    auto invalid = remove(currFeaturesList.begin(), currFeaturesList.end(), feature);
    currFeaturesList.erase(invalid, currFeaturesList.end());

    return currFeaturesList;
}

//fonction pour supprimer les espaces à la fin d'une chaîne de caractères
string rtrim(string s){
    const std::string WHITESPACE = " \n\r\t\f\v";
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == string::npos) ? "" : s.substr(0, end + 1);
}

//fonction pour sauvegarder les features à tester
void backupFeaturesToTest(vector<string> featuresList){
    string features_str = "";

    for(auto feature = featuresList.begin(); feature != featuresList.end(); feature++){
        features_str += (*feature + " ");
    }

    features_str = rtrim(features_str);

    ofstream backupFile(FEATURES_TO_TEST_FILE.c_str());
    backupFile << features_str;
    backupFile.close();
}

//fonction pour lire les features à tester précédemment sauvegardées
vector<string> readFeaturesToTest(){
    vector<string> features = {};

    ifstream featuresFile(FEATURES_TO_TEST_FILE.c_str());
    string text;

    while(getline(featuresFile, text)){
        string delimiter = " ";
        size_t pos = 0;
        string token;
        text = text + " ";

        while((pos = text.find(delimiter)) != string::npos){
            token = text.substr(0, pos);
            features.push_back(token);
            text.erase(0, pos + delimiter.length());
        }
    }

    featuresFile.close();

    return features;
}
//*******************************************************************************************************************

//**********************************
//MAIN FUNCTION
int main(int argc, char **argv){
    bool start = true, resume = false;

    if(argc == 2){
        string action = argv[1];

        if(action == "resume"){
            start = false;
            resume = true;
        }
    }

    if(start){
        system(("rm " + DOMU_USEFUL_FEATURES_FILE + " " + APP_USEFUL_FEATURES_FILE + " " + FEATURES_TO_TEST_FILE).c_str());
        //system(("rm -r " + TESTS_RESULTS_DIR + "/*").c_str());

        system(("touch " + DOMU_USEFUL_FEATURES_FILE).c_str());
        system(("touch " + APP_USEFUL_FEATURES_FILE).c_str());
        system(("touch " + FEATURES_TO_TEST_FILE).c_str());
    }

    //récupération de la liste des features par défaut du domU
    /* int res =  getDomUDefaultFeatures(ORIGINAL_CONFIG_FILE);
    if(res == 0){
        system(("mv " + FEATURES_FILE_NAME + " ./" + DOMU_DEFAULT_FEATURES_DIR + "/").c_str());

    }else if(res == -1){
        system(("rm " + LAUNCHING_FAILED_FILE_NAME).c_str());
        cout << "Failed to get the domU default features!" << endl;
        return -1;
    } */

    vector<string> orderedFeatures;

    if(start){
        //lecture de la liste des feature récupérées
        map<tuple<int32_t, int32_t, string>, vector<string>> availableFeatures = readDomUDefaultFeatures();

        //built the list of features available on the domU
        vector<string> domUFeatures = {};

        map<tuple<int32_t, int32_t, string>, vector<string>>::iterator it;
        for(it = availableFeatures.begin(); it != availableFeatures.end(); it++){
            vector<string> featuresList = it -> second;
            domUFeatures.insert(domUFeatures.end(), featuresList.begin(), featuresList.end());
        }

        orderedFeatures = orderFeatures(domUFeatures);

    }else if(resume){
        orderedFeatures = readFeaturesToTest();
    }

    //go through the list of available features to test them
    while(orderedFeatures.size() > 0){
        auto feature = orderedFeatures.begin();

        string feature_directory = TESTS_RESULTS_DIR + "/" + *feature;
        system(("rm " + CUSTOM_CONFIG_FILE).c_str());
        system(("mkdir " + feature_directory).c_str());

        //buld the cpuid string to disable the current feature
        string cpuidstr = "cpuid = \"host," + *feature + "=0\"";

        copyFile(ORIGINAL_CONFIG_FILE.c_str(), CUSTOM_CONFIG_FILE.c_str());
        writeAtEnd(CUSTOM_CONFIG_FILE.c_str(), cpuidstr);

        int test_res = runFeatureTest(CUSTOM_CONFIG_FILE, *feature);
        if(test_res == 0){
            system(("mv " + FEATURES_FILE_NAME + " " + feature_directory).c_str());
            system(("mv " + TESTS_LOG_FILE_NAME + " " + feature_directory).c_str());

            string tests_log_file_path = feature_directory + "/" + TESTS_LOG_FILE_NAME;
            if(!evalLogs(tests_log_file_path)){
                orderedFeatures = backupResult(APP_USEFUL_FEATURES_FILE, orderedFeatures, *feature);
            }

        }else if(test_res == -1){
            system(("mv " + LAUNCHING_FAILED_FILE_NAME + " " + feature_directory).c_str());

            orderedFeatures = backupResult(DOMU_USEFUL_FEATURES_FILE, orderedFeatures, *feature);
        }

        auto invalid = remove(orderedFeatures.begin(), orderedFeatures.end(), *feature);
        orderedFeatures.erase(invalid, orderedFeatures.end());

        backupFeaturesToTest(orderedFeatures);
    }
    
    return 0;
}
