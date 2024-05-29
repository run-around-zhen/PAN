#include <fstream>
#include <iostream>
#include <string>
using namespace std;


// Scheduling input flows from flow.txt
struct FlowInput {
    uint32_t src, dst, pg, maxPacketCount, port;
    double start_time;
    uint32_t idx;
};
FlowInput flow_input = {0};  // global variable
uint32_t flow_num;
std::ifstream flowf;

/**
 * Read flow input from file "flowf"
 */
void ReadFlowInput() {
    cout << flow_input.idx << endl;
    if (flow_input.idx < flow_num) {
        flowf >> flow_input.src >> flow_input.dst >> flow_input.pg >> flow_input.maxPacketCount >>
            flow_input.start_time;
        cout << flow_input.idx << endl;
    } else {
        std::cout << "*** input flow is over the prefixed number -- flow number : " << flow_num
                  << std::endl;
        std::cout << "*** flow_input.idx : " << flow_input.idx << std::endl;
        std::cout << "*** THIS IS THE LAST FLOW TO SEND :) " << std::endl;
    }
}

int main() {
    
    string flow_file = "../config/L_20.00_CDF_AliStorage2019_N_128_T_100ms_B_100_flow.txt";
    flowf.open(flow_file.c_str());
    
    flowf >> flow_num;
    cout << flow_num << endl;
    ReadFlowInput();
    return 0;
}
