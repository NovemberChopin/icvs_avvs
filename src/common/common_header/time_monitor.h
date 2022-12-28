#ifndef __AOVS_COMMON_TIME_MONITOR_H__
#define __AOVS_COMMON_TIME_MONITOR_H__

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <ctime>
#include <ratio>
#include <chrono>
#include <mutex>
#include "common_msgs/Header.h"
#include "ros/ros.h"

using namespace std::chrono;
namespace avos {
namespace common {
namespace TimeMonitor {

#define MONITOR_VECTOR_SIZE 100
#ifndef TMTHREAD
#define ADDMUTEX(dev_mutex) std::unique_lock<std::mutex> lock(dev_mutex)
#else
#define ADDMUTEX(dev_mutex)
#endif

#define PUSH_FUN(source, fun)                \
  double t1 = ros::Time::now().toSec();      \
  fun;                                       \
  double dt = ros::Time::now().toSec() - t1; \
  avos::common::TimeMonitor::PushRaw(dt, source);

typedef common_msgs::TimeStatus Tdev;
class GlobalDate {
public:
  static GlobalDate &Instance() {
    static GlobalDate *instance = NULL;
    if (!instance) {
      instance = new GlobalDate();
    }
    return *instance;
  }
  std::vector<Tdev> dev_;
  std::mutex dev_mutex_;

private:
  GlobalDate() {}
};

template <class T>
void PushRaw(T dt, std::string source, std::string dest = "") {
  Tdev dev;
  dev.dtime = dt;
  dev.source_node_name = source;
  dev.destination_node_name = dest;
  {
    ADDMUTEX(GlobalDate::Instance().dev_mutex_);
    if (GlobalDate::Instance().dev_.size() < MONITOR_VECTOR_SIZE)
      GlobalDate::Instance().dev_.push_back(dev);
  }
}

template <class T>
void Push(const T &ms, std::string source, std::string dest) {
  for (int i = 0; i < ms.header.time_statistics.dev_time_status_msg.size(); i++) {
    ADDMUTEX(GlobalDate::Instance().dev_mutex_);
    if (GlobalDate::Instance().dev_.size() < MONITOR_VECTOR_SIZE)
      GlobalDate::Instance().dev_.push_back(ms.header.time_statistics.dev_time_status_msg[i]);
  }
  Tdev dev;
  dev.dtime = ros::Time::now().toSec() - ms.header.time_statistics.sending_timestamp;
  dev.source_node_name = source;
  dev.destination_node_name = dest;
  {
    ADDMUTEX(GlobalDate::Instance().dev_mutex_);
    if (GlobalDate::Instance().dev_.size() < MONITOR_VECTOR_SIZE)
      GlobalDate::Instance().dev_.push_back(dev);
  }
}
template <class T>
void Pull(T &msg) {
  ADDMUTEX(GlobalDate::Instance().dev_mutex_);
  msg.header.time_statistics.dev_time_status_msg.clear();
  for (auto &ms : GlobalDate::Instance().dev_) {
    msg.header.time_statistics.dev_time_status_msg.push_back(ms);
  }
  msg.header.time_statistics.sending_timestamp = ros::Time::now().toSec();
  GlobalDate::Instance().dev_.clear();
}


#define TIME_MONITOR_INIT(timer)         \
  static double timer##time_monitor = 0; \
  static int timer##count_monitor = 0;   \
  steady_clock::time_point timer##init_monitor = steady_clock::now();

#define TIME_MONITOR_USE(timer) \
  steady_clock::time_point timer##init_monitor = steady_clock::now();

#define TIME_MONITOR_OUT(nodename, timer)                                   \
  double timer##duration = (duration_cast<duration<double>>(                \
                                steady_clock::now() - timer##init_monitor)) \
                               .count();                                    \
  timer##time_monitor += timer##duration;                                   \
  ++timer##count_monitor;                                                   \
  avos::common::TimeMonitor::PushRaw<double>(timer##duration, nodename);

}  // namespace TimeMonitor
}  // namespace common
}  // namespace avos
#endif
