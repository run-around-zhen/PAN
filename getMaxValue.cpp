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

string configId = "92703393";
string pre = "mix/output/";
string suf = "_out_voq.txt";
int *a[100005];
map<int,int> timeRecord;

int* getVal(string s) {
    int* res = new int[5]{0};
    int val = 0;
    int id = 0;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == ',') {
            res[id++] = val;
            val = 0;
        }else {
            val = val * 10 + s[i] - '0';
        }
    }   
    res[id] = val;
    int* ans = new int[2];
    ans[0] = res[0];
    ans[1] = res[4];
    return ans;
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
        timeRecord[a[id][0]] = max(timeRecord[a[id][0]],a[id][1]);
        id ++;
    }
    int maxValue = 0;
    for (const auto& pair : timeRecord) {
       // cout << pair.first << " " << pair.second << endl;
        maxValue = max(maxValue, pair.second);
    }
    cout << "max value is " << maxValue << endl;
    return 0;
}