#include "../classes/classes.hpp"
#include "../globals/globals.hpp"
#include "../Class/playerClass.h"
#include <iostream>
#include <thread>
#include <vector>
#include <Windows.h>
#include <chrono>
#include <atomic>
#include <mutex>
#include <unordered_map>
#include <algorithm>

#include "../games/phantomforces.hpp"
#include "../games/mvsduels.hpp"
#include "../games/universal.hpp"
#include "../games/badbusiness.hpp"
#include "../games/scorchedearth.hpp"


void threadLoop() {
    if (globals::game_id == 4348829796) { 
        std::cout << "mvsDuels Loaded\n";
        mvsDuelsUpdateThread();
    }
    else if (globals::game_id == 113491250) {
        std::cout << "Phantom Loaded\n";
        std::thread(phantomForcesPositionUpdateThread).detach();
        std::thread(phantomForcesHeartbeatThread).detach();
        phantomForcesUpdateThread();
    }
    else if (globals::game_id == 1168263273) {
        std::cout << "Business Loaded\n";
        badBusinessUpdateThread();
    }
    else if (globals::game_id == 4785126950) {
        std::cout << "Scorched Earth Loaded\n";
        std::thread(scorchedEarthPositionUpdateThread).detach(); // Not working currently
        std::thread(scorchedEarthHeartbeatThread).detach();
        scorchedEarthUpdateThread();
    }
    else {
        std::cout << "Universal Loaded\n";
        std::thread(positionUpdateThreadUniversal).detach();
        std::thread(heartbeatThread).detach();
        updateThreadUniversal();
    }
}


void monitorThread(std::thread& funcThread, HANDLE& threadHandle) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        bool heartbeat_ok = false;
        {
            std::lock_guard<std::mutex> lock(heartbeat_mutex);
            heartbeat_ok = heartbeat_received;
            heartbeat_received = false;
            globals::threadcrash = false;
        }

        if (!heartbeat_ok) {
            std::cout << "[Watchdog] -> Seen crash on esp, Restarting" << std::endl;
            heartbeat_ok = true;
            globals::threadcrash = true;
            if (funcThread.joinable()) {
                CloseHandle(threadHandle);
                TerminateThread(threadHandle, 0);
                funcThread.join();
            }


            funcThread = std::thread(threadLoop);
            threadHandle = funcThread.native_handle();
            funcThread.detach();
        }
    }
}