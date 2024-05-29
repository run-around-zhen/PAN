#include <iostream>
#include <fstream>
#include <map>
using namespace std;


// 228250423:16主机,无优化
// 573513307:16主机,有优化
// mix/output/150434464

// 903784:32主机
// 726493702: 64主机
// 872029591： 64主机
// mix/output/330935480：128主机
// 716807433:64主机，1000g带宽   705824555
// mix/output/441092068   64主机   200g带宽

string configId = "107670470";
string configId2 = "578378337";
string pre = "mix/output/";
string suf = "_out_fct.txt";
int *a[200005];
map<uint64_t,int> beginTime;
map<uint64_t,int> costTime;

int* getVal(string s) {
    int* res = new int[8]{0};
    int val = 0;
    int id = 0;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == ' ') {
            res[id++] = val;
            val = 0;
        }else {
            val = val * 10 + s[i] - '0';
        }
    }   
    res[id] = val;
    return res;
}
uint64_t getKey(uint64_t a1,uint64_t a2,uint64_t a3,uint64_t a4) {
    return (a1<<48) + (a2<<32) + (a3<<16) + a4;
}

int main() {
    string fileRoot = pre+configId+"/"+configId+suf;
    cout << fileRoot << endl;
    ifstream voqInfo(fileRoot);
    if (!voqInfo.is_open()) {
        cout << "无法打开文件" << endl;
        return 0;
    }
    int id = 0;
    while (!voqInfo.eof()) {
        string line;
        getline(voqInfo, line);
        a[id] = getVal(line);
        uint64_t key = getKey(a[id][0],a[id][1],a[id][2],a[id][3]);
        beginTime[key] = a[id][5];
        costTime[key] = a[id][6];
        id ++;
    }
    
    string fileRoot2 = pre+configId2+"/"+configId2+suf;
    ifstream voqInfo2(fileRoot2);
    if (!voqInfo2.is_open()) {
        cout << "无法打开文件" << endl;
        return 0;
    }
    while (!voqInfo2.eof()) {
        string line;
        getline(voqInfo2, line);
        int *b = getVal(line);
        uint64_t key = getKey(b[0],b[1],b[2],b[3]);
        if (costTime[key] != b[6] && costTime[key] > b[6]) {
            cout << "花费的时间不同:" << costTime[key] << " " << b[6] << " " << beginTime[key] <<< " " << b[4] < std::endl;
        }
    }
    return 0;
}