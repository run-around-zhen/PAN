/*
auto it = pathSet.begin();    // 先选择第一个
tx_md.goodPath = *it;
it ++;
while (it != pathSet.end()) {
    uint32_t path = *it;
    uint32_t tmpOutDev = GetOutPortFromPath(path, 0);
    //std::cout << "pathId为:" << path << " 拥塞程度为:" << m_txPathCongestion[path] << " 端口为:" << tmpOutDev
    //<< "是否被暂停" << paused[tmpOutDev][qIndex] << std::endl;
    it ++;
    if (paused[tmpOutDev][qIndex]) {
        continue;
    }
    uint32_t nowOutDev = GetOutPortFromPath(tx_md.goodPath, 0);
    if (paused[nowOutDev][qIndex] || m_txPathCongestion[path] < m_txPathCongestion[tx_md.goodPath]) {
    //if (m_txPathCongestion[path] < m_txPathCongestion[tx_md.goodPath]) {
        tx_md.goodPath = path;
    }
}

// 发送方重新路由的条件
if ((paused[outDev][qIndex] || m_txPathCongestion[tx_md.currPath] > m_reroutedRank) && 
    now > m_pathInvaildTime[tx_md.pkt_flowkey]) {     // 端口的队列被暂停了且允许重新路由
    
    //std::cout << "拥堵的这个路径的分数为:" << pathScore << std::endl;
// if ((paused[outDev][qIndex] || m_txPathCongestion[tx_md.currPath] > m_reroutedRank)) { 

    // if (now < m_pathInvaildTime[tx_md.currPath]) {
    //     //std::cout << "当前的路径有ecn标记" << std::endl;
    // }
// if (paused[outDev][qIndex]) {
// if (m_txPathCongestion[tx_md.currPath] > m_reroutedRank) { 
    // if (paused[outDev][qIndex]) {
    //     std::cout << "当前路径被pfc暂停:" << paused[outDev][qIndex] << std::endl;
    // }
    // if (m_txPathCongestion[tx_md.currPath] > m_reroutedRank) {
    //     std::cout << "太拥堵了,重新路由,端口为:" << outDev << " 原始路径拥塞程度为:" 
    //     << m_txPathCongestion[tx_md.currPath] << std::endl;
    // }
    
    // GemmaRouting::m_PortPausedNum ++; 
    // tx_md.goodPath = txEntry._pathId;
    // auto it = pathSet.begin();
    // while (it != pathSet.end()) {
    //     uint32_t path = *it;
    //     uint32_t tmpOutDev = GetOutPortFromPath(path, 0);
    //     it ++;
    //     //std::cout << "pathId为:" << path << " 拥塞程度为:" << m_txPathCongestion[path] << " 端口为:" << tmpOutDev
    //     //<< "是否被暂停" << paused[tmpOutDev][qIndex] << std::endl;
    //     if (paused[tmpOutDev][qIndex]) {
    //         continue;
    //     }
    //     uint32_t nowOutDev = GetOutPortFromPath(tx_md.goodPath, 0);
    //     if (paused[nowOutDev][qIndex] || m_txPathCongestion[path] < m_txPathCongestion[tx_md.goodPath]) {
    //     //if (m_txPathCongestion[path] < m_txPathCongestion[tx_md.goodPath]) {
    //         tx_md.goodPath = path;
    //     }
    // }
    // uint32_t tmpOutDev = GetOutPortFromPath(tx_md.goodPath, 0);
    // if (paused[outDev][qIndex] && !paused[tmpOutDev][qIndex]) {
    //     isFoundNewPath = true;
    // }else if (!paused[outDev][qIndex] && (m_txPathCongestion[txEntry._pathId] - m_txPathCongestion[tx_md.goodPath] > 20 ||
    //     m_txPathCongestion[tx_md.goodPath] < m_reroutedRank)) {   // 当前随机的路径没有被暂停且拥塞程度低
    //     isFoundNewPath = true;
    // }
    // // if (!paused[tmpOutDev][qIndex] && m_txPathCongestion[tx_md.goodPath] <= m_reroutedRank) {
    // //     isFoundNewPath = true;
    // // }
    // if (isFoundNewPath) {
    //     std::cout << tx_md.pkt_flowkey << " 找寻新路径成功,找寻到的新端口为 " << tmpOutDev << " 新的路径拥塞程度为:" << m_txPathCongestion[tx_md.goodPath] << std::endl;
    //     uint32_t score = GetPathScore(srcToRId, tx_md.goodPath);
    //     std::cout << "原路径与新路径的分数为:" << pathScore << " " << score << std::endl;
    //     txEntry._prePathId = txEntry._pathId;   //记录原来的路径
    //     txEntry._pathId = tx_md.goodPath;   // 最新的路径
    //     txEntry._phase = txEntry._phase + 1;  // 阶段+1
    //     m_pathInvaildTime[tx_md.pkt_flowkey] = now + m_entryPauseTime;
    //     GemmaRouting::m_RerouteNum ++;
    // } else {
    //    //std::cout << "找寻新路径失败" << std::endl;
    // }
}

*/