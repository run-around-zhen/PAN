switchNum = 8  
hostBase = 16
hostNum = 8 * hostBase
oversubscript = 1 # over-subscription between ToR uplink - downlink
link_rate = 100 # Gbps
link_latency = 1000 # ns
num_link = hostNum + 64

filename = "/home/zhen/workspace/experiment/ns-3.19/config/leaf_spine_{}_{}G_OS{}.txt".format(hostNum, 
                                                                                              link_rate, oversubscript)
print(filename)
try:
    with open(filename, "w") as f:
        f.write("{} {} {}\n".format(hostNum+switchNum*2, switchNum*2, num_link))
        for num in range(hostNum, hostNum+16):
            f.write("{}".format(num))
            if num == hostNum + 15:
                f.write('\n')
            else:
                f.write(' ')

        for i in range(switchNum):
            for p in range(hostBase):
                id_server = i * hostBase + p
                id_tor = hostNum + i
                f.write("{} {} {}Gbps {}ns 0\n".format(id_server, id_tor, link_rate, link_latency))

        for i in range(switchNum):
            for j in range(switchNum):
                id_tor = i + hostNum
                id_agg = j + hostNum + switchNum
                # print("{} {} {}Gbps {}ns 0.000000".format(id_tor, id_agg, link_rate, link_latency))
                f.write("{} {} {}Gbps {}ns 0\n".format(id_tor, id_agg, link_rate, link_latency))
except:
    print("出错了！")