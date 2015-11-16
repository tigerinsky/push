#include <iostream>
#include <fstream>
#include <unistd.h>
#include <queue>

#include "monitor.h"
#include "google/gflags.h"
#include "common.h"

namespace monitor {

DEFINE_string(monitor_file, "", "monitor result output file");
DEFINE_int32(monitor_interval, 5, "monitor result output interval");

Monitor::Monitor() {
}

Monitor::~Monitor() {
}

std::unique_ptr<Monitor> Monitor::_instance;

void Monitor::add_statistic(StatisticIf* s) {
    _statistics.push_back(s);
}

void Monitor::add_stat_collector(stat_collector func) {
    _stat_collectors.push_back(func);
}

void Monitor::start() {
    if (0 == _statistics.size()) {
        return;
    }
    for (auto& ite : _statistics) {
        ite->init(); 
    }
    _stop = false;;
}

void Monitor::print_stat() {
    std::ofstream myfile(FLAGS_monitor_file);
    if(!myfile.is_open()) {
        LOG_ERROR << "open monitor output file error: "
            << FLAGS_monitor_file;
        return;
    }
    std::vector<StatisticIf*>::iterator ite;
    std::string info;
    for (ite = _statistics.begin(); ite != _statistics.end(); ++ite) {
        (*ite)->get_info(info);
        myfile << info;
    }
    for (auto& func : _stat_collectors) {
        (*func)(&info);
        myfile << info;
    }
    myfile.close();
}

bool Monitor::is_stop() {
    return _stop;
}

void Monitor::stop() {
    _stop = false;
}

void Monitor::forward() {
    std::vector<StatisticIf*>::iterator ite;
    for (ite = _statistics.begin(); ite != _statistics.end(); ++ite) {
        (*ite) -> forward();
    }
}

void* print_stat(void *arg) {
    g_common_monitor->start();
    while(!g_common_monitor->is_stop()) {
        sleep(FLAGS_monitor_interval);
        if (0 != FLAGS_monitor_file.size()) {
            g_common_monitor->print_stat();
        }
        g_common_monitor->forward();
    }
}

pthread_t start_monitor() {
    pthread_t pid;
    pthread_create(&pid, NULL, print_stat, NULL);
    return pid;
}

void end_monitor() {
    g_common_monitor->stop();//TODO 这里是不是得加锁
}

}//namespace monitor
