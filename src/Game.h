#ifndef GAME_H
#define GAME_H

#include "Console.h"
#include "Player.h"
#include "Customer.h"
#include "Appliance.h"
#include <vector>
#include <string>
#include <chrono>

class Game {
private:
    bool running;
    double gameTimeTotal;       // 120 giây
    double gameTimeRemaining;
    int score;

    Player p1; // Đầu bếp 1
    Player p2; // Đầu bếp 2

    SharedTable sharedTable;
    Shaker sharedShaker;
    Brewer sharedBrewer;

    std::vector<Customer> activeCustomers;
    double customerSpawnTimer;
    int totalCustomersServed;
    
    // Tin tức thông báo chạy chữ
    std::string notificationMsg;
    double notificationTimer;

    // Quản lý thời gian thực
    std::chrono::steady_clock::time_point lastFrameTime;

    void ProcessInputs(double deltaTime);
    void UpdateEntities(double deltaTime);
    void SpawnCustomer();
    void SetNotification(const std::string& msg, double duration = 2.5);
    void DrawFrame();

public:
    Game();
    void Initialize();
    void Run();
    void ShowMenu();
    void ShowGameOver();
};

#endif // GAME_H
