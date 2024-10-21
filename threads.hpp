#ifndef UPDATE_THREAD_H
#define UPDATE_THREAD_H

#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include "../Class/playerClass.h"


extern std::atomic<bool> thread_running;
extern std::atomic<bool> heartbeat_received;
extern std::mutex heartbeat_mutex;
extern std::vector<Player> playerList;


void threadLoop();
void monitorThread(std::thread& funcThread, HANDLE& threadHandle);

#endif