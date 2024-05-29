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

string pre = "config/";
string paths[] = {
    "L_15.00_CDF_AliStorage2019_N_32_T_10ms_B_100_flow.txt",
    "L_25.00_CDF_AliStorage2019_N_32_T_10ms_B_100_flow.txt",
    "L_40.00_CDF_AliStorage2019_N_32_T_10ms_B_100_flow.txt"
};
string outPath = "L_46.00_CDF_AliStorage2019_N_32_T_30ms_B_100_flow.txt";

int a[500005][10];
double t[500005];

int main() {
    ofstream outInfo;
    outInfo.open(pre+outPath, ios::out);
    if (!outInfo.is_open()) {
        cout << "无法打开写文件" << endl;
    }
    int id = 0;
    int sum = 0;
    for (int i = 0; i < 3; i++) {
        ifstream inInfo(pre+paths[i]);
        if (!inInfo.is_open()) {
            cout << "无法打开文件" << endl;
            return 0;
        }
        bool isFirst = true;
        while(!inInfo.eof()) {
            if (isFirst) {
                int num;
                inInfo >> num;
                sum += num;
                cout << num << endl;
                isFirst = false;
            }else {
                inInfo >> a[id][0] >> a[id][1] >> a[id][2] >> a[id][3];
                inInfo >> t[id];
                if (a[id][0] == 0 && a[id][1] == 0) continue;
                t[id] += 0.01*i;
                cout.precision(9);
                // cout << a[id][0] << " " << a[id][1] << " " << a[id][2] << " " <<  a[id][3] << " " << t[id] << endl;
                id ++;
            }            
        }
    }
    outInfo << sum << endl;
    for (int i = 0; i < id; i++) {
        outInfo.precision(10);
        outInfo << a[i][0] << " " << a[i][1] << " " << a[i][2] << " " <<  a[i][3] << " " << t[i] << endl;
        //ofile.write("%d %d 3 %d %.9f\n"%(a[id][0], a[id][1], a[id][3], t[id]));
    }
    return 0;
}