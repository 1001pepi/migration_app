/*
By TANANKEM MENKEM Ramses
December 2022
*/
#include <bits/stdc++.h>
#include<unistd.h>

#include "run_tests.h"

using namespace std;

struct FeaturesList {string list[32];};

//**********************************************************************************************************
//GLOBAL VARIABLES
int REGISTER_SIZE = 32;
std::map<std::string, std::string> options;
string DOM0_APP_DIRECTORY;
string LOG_FILE_PATH;
string FEATURES_FILE_NAME = "features.txt";

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
    //{make_tuple(-2147483647, -1, "ecx"), {"", "addr_mask_ext", "monitorx", "pcx_l2i", "perftsc", "dbx", "", "perfctr_nb", "perfctr_core", "topoext", "tbm", "", "nodeid_msr", "", "tce", "fma4", "lwp", "", "wdt", "skinit", "xop", "ibs", "osvw", "3dnowprefetch", "misalignsse", "sse4a", "abm", "cr8_legacy", "extapic", "svm", "cmp_legacy", "lahf_lm"}},
    //eax=80000001h
    //{make_tuple(2147483649, -1, "edx"), {"3dnow", "3dnowext", "lm", "", "rdtscp", "pdpe1gb", "fxsr_opt", "fxsr", "mmx", "mmxext", "", "nx", "mp", "", "pse36", "pat", "cmov", "mca", "pge", "mtrr", "syscall", "", "apic", "cx8", "mce", "pae", "msr", "tsc", "pse", "de", "vme", "fpu"}},
    //eax=8000001Fh
    //{make_tuple(2147483679, -1, "eax"), {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "","vte", "prevent_host_ibs", "debug_swap", "alternate_injection", "restricted_injection", "64_host", "hw_cache_coherency", "", "", "", "", "vmpl", "sev_snp", "sev_es", "page_flush", "sev", "sme"}},
    //eax=80000021h
    //{make_tuple(2147483681, -1, "eax"), {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "CpuidUserDis", "", "", "", "PrefetchCtlMsr", "", "", "", "NoSmmCtlMSR", "AutomaticIBRS", "UpperAddressIgnore", "NullSelectClearsBase", "", "", "SmmPgCfgLock", "LFenceAlwaysSerializing", "", "NoNestedDataBp"}}
};
//********************************************

//***************************************************************
//CUSTOM FUNCTIONS

//function to read config file options
void parseConfig(std::istream & cfgfile){
    for (std::string line; std::getline(cfgfile, line); ){

        std::istringstream iss(line);
        std::string id, eq, val;

        bool error = false;

        if (!(iss >> id)){
            error = true;

        }else if (id[0] == '#'){
            continue;

        }else if (!(iss >> eq >> val >> std::ws) || eq != "=" || iss.get() != EOF){
            error = true;
        }

        if (error){
            // do something appropriate: throw, skip, warn, etc.
        }else{
            options[id] = val;
        }
    }
}

//function to get the list of available features
void lookForAvailableFeatures(){
    map<tuple<int32_t, int32_t, string>, vector<string>> availableFeatures;

    ofstream featuresFile(FEATURES_FILE_NAME.c_str());

    map<tuple<int32_t, int32_t, string>, FeaturesList>::iterator it;
    for(it = FEATURES_BITS.begin(); it != FEATURES_BITS.end(); it++){
        tuple<int32_t, int32_t, string> key = it->first;
        FeaturesList featuresList = it -> second;
        availableFeatures[key] = {};
        int32_t registers_values[4];
        int32_t register_val= 0;
        
        int32_t eax_val = get<0>(key);
        int32_t ecx_val = get<1>(key);
        string register_to_read = get<2>(key);

        if(ecx_val > -1){
            __asm__("mov %0, %%eax\n\t": :"r"(eax_val));
            __asm__("mov %0, %%ecx\n\t": :"r"(ecx_val));

            __asm__("cpuid\n\t");

            __asm__("mov %%eax, %0\n\t":"=r" (registers_values[0]));
            __asm__("mov %%ebx, %0\n\t":"=r" (registers_values[1]));
            __asm__("mov %%ecx, %0\n\t":"=r" (registers_values[2]));
            __asm__("mov %%edx, %0\n\t":"=r" (registers_values[3]));

        }else{
            __asm__("mov %0, %%eax\n\t": :"r"(eax_val));

            __asm__("cpuid\n\t");

            __asm__("mov %%eax, %0\n\t":"=r" (registers_values[0]));
            __asm__("mov %%ebx, %0\n\t":"=r" (registers_values[1]));
            __asm__("mov %%ecx, %0\n\t":"=r" (registers_values[2]));
            __asm__("mov %%edx, %0\n\t":"=r" (registers_values[3]));
        }

        if(register_to_read == "eax"){
           register_val = registers_values[0];
        }else if (register_to_read == "ebx"){
            register_val = registers_values[1];
        }else if (register_to_read == "ecx"){
            register_val = registers_values[2];
        }else if (register_to_read == "edx"){
            register_val = registers_values[3];
        }
        
        string register_binary_str = bitset<32>(register_val).to_string();

        for(int i = 0; i < REGISTER_SIZE; i++){
            if(register_binary_str[i] == '1' && featuresList.list[i] != ""){
                availableFeatures[key].push_back(featuresList.list[i]);
            }
        }

        //write the features in the featuresFile
        featuresFile << eax_val << " " << ecx_val << " " << register_to_read;
        for(auto feature = availableFeatures[key].begin(); feature != availableFeatures[key].end(); feature++){
            featuresFile << " " << *feature;
        }

        featuresFile << endl;
    }

    featuresFile.close();
}
//**************************************************************************************************************

//**********************************
//MAIN FUNCTION
int main(){
    ifstream confFile("/usr/local/share/migration_app/config.cfg");
    parseConfig(confFile);

    DOM0_APP_DIRECTORY = options["DOM0_APP_DIRECTORY"];
    LOG_FILE_PATH = options["LOG_FILE_PATH"];

    //look for available features
    lookForAvailableFeatures();

    //send the features file to the domO
    string cmd = "scp " + FEATURES_FILE_NAME + " " + DOM0_APP_DIRECTORY;
    int status = system(cmd.c_str());

    //run the tests
    runTests();
    
    //send the results of the tests
    string cmd2 = "scp " + LOG_FILE_PATH + " " + DOM0_APP_DIRECTORY;
    system(cmd2.c_str());

    return 0;
}
