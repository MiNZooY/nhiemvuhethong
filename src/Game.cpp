#include "Game.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <thread>
#include <cstdlib>
#include <ctime>

Game::Game() : p1(1), p2(2) {
    running = false;
    gameTimeTotal = 300.0;
    gameTimeRemaining = gameTimeTotal;
    score = 0;
    customerSpawnTimer = 2.0; // Khách hàng đầu tiên xuất hiện nhanh
    totalCustomersServed = 0;
    notificationMsg = "";
    notificationTimer = 0.0;
}

void Game::Initialize() {
    Console::Initialize();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Khởi tạo players và thiết bị
    p1 = Player(1);
    p2 = Player(2);
    sharedTable = SharedTable();
    sharedShaker = Shaker();
    sharedBrewer = Brewer();

    score = 0;
    gameTimeRemaining = gameTimeTotal;
    activeCustomers.clear();
    totalCustomersServed = 0;
    notificationMsg = "COOPERATE SMOOTHLY TO SERVE CUSTOMERS!";
    notificationTimer = 4.0;

    // Spawn 2 khách đầu tiên
    SpawnCustomer();
    SpawnCustomer();
}

void Game::SpawnCustomer() {
    if (activeCustomers.size() >= 3) return;

    Customer c;
    std::vector<std::string> names = {
        "Chubby Boba Cat", "Hungry Manager", "Vy Student", 
        "Uncle Tu Rider", "Snacky Grandma", "Trendy Influencer", "Busy Delivery Rider"
    };
    c.name = names[std::rand() % names.size()];
    
    // Ngẫu nhiên món nước
    int drinkChoice = std::rand() % 4;
    switch (drinkChoice) {
        case 0: c.orderType = DrinkType::BOBA_MILK_TEA; break;
        case 1: c.orderType = DrinkType::PEACH_TEA; break;
        case 2: c.orderType = DrinkType::ICED_MILK_COFFEE; break;
        case 3: c.orderType = DrinkType::SALTED_COFFEE; break;
    }

    c.maxPatience = 50.0 + (std::rand() % 20); // 50 đến 70 giây chờ đợi
    c.remainingPatience = c.maxPatience;
    c.points = 100;
    c.bonusTips = 50;

    activeCustomers.push_back(c);
    Beep(1400, 100);
}

void Game::SetNotification(const std::string& msg, double duration) {
    notificationMsg = msg;
    notificationTimer = duration;
}

void Game::ProcessInputs(double deltaTime) {
    // --- XỬ LÝ PHÍM BẤM - NGƯỜI CHƠI 1 (Đầu bếp Sơ chế) ---
    p1.UpdateCooldown(deltaTime);
    if (p1.CanInteract()) {
        bool actionTaken = false;
        
        // Q: Lấy ly rỗng
        if (GetAsyncKeyState(p1.keyCup) & 0x8000) {
            if (p1.heldItem.IsEmpty()) {
                p1.heldItem.type = ItemType::CUP;
                p1.heldItem.state = DrinkState::RAW;
                Beep(800, 80);
                SetNotification("Chef 1: Picked up an empty cup!", 1.2);
                actionTaken = true;
            }
        }
        // W: Thêm Trà
        else if (GetAsyncKeyState(p1.keyTea) & 0x8000) {
            if (p1.heldItem.type == ItemType::CUP) {
                p1.heldItem.AddBaseIngredient(ItemType::TEA);
                Beep(900, 80);
                if (p1.heldItem.state == DrinkState::RUINED) {
                    SetNotification("[!] Ruined! Wrong ingredient combination!", 2.0);
                } else {
                    SetNotification("Chef 1: Added Tea Bag to the cup!", 1.2);
                }
                actionTaken = true;
            }
        }
        // E: Thêm Sữa đặc
        else if (GetAsyncKeyState(p1.keyMilk) & 0x8000) {
            if (p1.heldItem.type == ItemType::CUP) {
                p1.heldItem.AddBaseIngredient(ItemType::MILK);
                Beep(900, 80);
                if (p1.heldItem.state == DrinkState::RUINED) {
                    SetNotification("[!] Ruined! Wrong ingredient combination!", 2.0);
                } else {
                    SetNotification("Chef 1: Added Condensed Milk to the cup!", 1.2);
                }
                actionTaken = true;
            }
        }
        // R: Thêm Bột Cà Phê
        else if (GetAsyncKeyState(p1.keyCoffee) & 0x8000) {
            if (p1.heldItem.type == ItemType::CUP) {
                p1.heldItem.AddBaseIngredient(ItemType::COFFEE);
                Beep(900, 80);
                if (p1.heldItem.state == DrinkState::RUINED) {
                    SetNotification("[!] Ruined! Wrong ingredient combination!", 2.0);
                } else {
                    SetNotification("Chef 1: Added Coffee Ground to the cup!", 1.2);
                }
                actionTaken = true;
            }
        }
        // T: Thêm Trân Châu Đen
        else if (GetAsyncKeyState(p1.keyBoba) & 0x8000) {
            if (p1.heldItem.type == ItemType::CUP) {
                p1.heldItem.AddBaseIngredient(ItemType::BOBA);
                Beep(900, 80);
                if (p1.heldItem.state == DrinkState::RUINED) {
                    SetNotification("[!] Ruined! Wrong ingredient combination!", 2.0);
                } else {
                    SetNotification("Chef 1: Added Black Boba to the cup!", 1.2);
                }
                actionTaken = true;
            }
        }
        // A: Thêm Đào Cam Sả
        else if (GetAsyncKeyState(p1.keyPeach) & 0x8000) {
            if (p1.heldItem.type == ItemType::CUP) {
                p1.heldItem.AddBaseIngredient(ItemType::PEACH);
                Beep(900, 80);
                if (p1.heldItem.state == DrinkState::RUINED) {
                    SetNotification("[!] Ruined! Wrong ingredient combination!", 2.0);
                } else {
                    SetNotification("Chef 1: Added Peach Slice to the cup!", 1.2);
                }
                actionTaken = true;
            }
        }
        // S: Thêm Kem Muối Béo
        else if (GetAsyncKeyState(p1.keySalt) & 0x8000) {
            if (p1.heldItem.type == ItemType::CUP) {
                p1.heldItem.AddBaseIngredient(ItemType::SALT_CREAM);
                Beep(900, 80);
                if (p1.heldItem.state == DrinkState::RUINED) {
                    SetNotification("[!] Ruined! Wrong ingredient combination!", 2.0);
                } else {
                    SetNotification("Chef 1: Added Salt Cream to the cup!", 1.2);
                }
                actionTaken = true;
            }
        }
        // Z: Vào Máy Lắc (Shaker)
        else if (GetAsyncKeyState(p1.keyShaker) & 0x8000) {
            if (!p1.heldItem.IsEmpty() && p1.heldItem.type == ItemType::CUP && !sharedShaker.isActive && sharedShaker.item.IsEmpty()) {
                if (p1.heldItem.state == DrinkState::RAW && !p1.heldItem.cupIngredients.empty()) {
                    sharedShaker.Start(p1.heldItem);
                    p1.heldItem.Clear();
                    Beep(600, 120);
                    SetNotification("Chef 1: Placed cup into Shaker!", 1.5);
                    actionTaken = true;
                }
            } else if (p1.heldItem.IsEmpty() && !sharedShaker.isActive && !sharedShaker.item.IsEmpty()) {
                p1.heldItem = sharedShaker.Take();
                Beep(850, 80);
                SetNotification("Chef 1: Retrieved shaken cup from Shaker!", 1.2);
                actionTaken = true;
            }
        }
        // X: Vào Máy Ủ Cà Phê (Brewer)
        else if (GetAsyncKeyState(p1.keyBrewer) & 0x8000) {
            if (!p1.heldItem.IsEmpty() && p1.heldItem.type == ItemType::CUP && !sharedBrewer.isActive && sharedBrewer.item.IsEmpty()) {
                if (p1.heldItem.state == DrinkState::RAW && !p1.heldItem.cupIngredients.empty()) {
                    sharedBrewer.Start(p1.heldItem);
                    p1.heldItem.Clear();
                    Beep(500, 120);
                    SetNotification("Chef 1: Placed cup into Brewer!", 1.5);
                    actionTaken = true;
                }
            } else if (p1.heldItem.IsEmpty() && !sharedBrewer.isActive && !sharedBrewer.item.IsEmpty()) {
                p1.heldItem = sharedBrewer.Take();
                Beep(850, 80);
                SetNotification("Chef 1: Retrieved brewed cup from Brewer!", 1.2);
                actionTaken = true;
            }
        }
        // C: Đặt lên / Lấy từ Bàn gỗ trung chuyển
        else if (GetAsyncKeyState(p1.keySharedTable) & 0x8000) {
            if (!p1.heldItem.IsEmpty() && sharedTable.IsEmpty()) {
                sharedTable.Place(p1.heldItem);
                p1.heldItem.Clear();
                Beep(700, 80);
                SetNotification("Chef 1: Transferred cup to Shared Table!", 1.5);
                actionTaken = true;
            } else if (p1.heldItem.IsEmpty() && !sharedTable.IsEmpty()) {
                p1.heldItem = sharedTable.Take();
                Beep(850, 80);
                SetNotification("Chef 1: Retrieved cup from Shared Table!", 1.5);
                actionTaken = true;
            }
        }
        // V: Đổ bỏ (Thùng rác)
        else if (GetAsyncKeyState(p1.keyTrash) & 0x8000) {
            if (!p1.heldItem.IsEmpty()) {
                p1.heldItem.Clear();
                Beep(300, 180);
                SetNotification("Chef 1: Trash Bin cleared workstation!", 1.5);
                actionTaken = true;
            }
        }

        if (actionTaken) {
            p1.ResetCooldown();
        }
    }

    // --- XỬ LÝ PHÍM BẤM - NGƯỜI CHƠI 2 (Chuyên viên Hoàn thiện & Giao hàng) ---
    p2.UpdateCooldown(deltaTime);
    if (p2.CanInteract()) {
        bool actionTaken = false;

        // I: Tương tác Bàn gỗ trung chuyển (Nhận ly cốt / Trả lại ly)
        if (GetAsyncKeyState(p2.keySharedTable) & 0x8000) {
            if (p2.heldItem.IsEmpty() && !sharedTable.IsEmpty()) {
                p2.heldItem = sharedTable.Take();
                Beep(850, 80);
                SetNotification("Chef 2: Retrieved cup from Shared Table!", 1.5);
                actionTaken = true;
            } else if (!p2.heldItem.IsEmpty() && sharedTable.IsEmpty()) {
                sharedTable.Place(p2.heldItem);
                p2.heldItem.Clear();
                Beep(700, 80);
                SetNotification("Chef 2: Placed cup back to Shared Table!", 1.5);
                actionTaken = true;
            }
        }
        // O: Thêm Đá viên
        else if (GetAsyncKeyState(p2.keyIce) & 0x8000) {
            if (!p2.heldItem.IsEmpty() && p2.heldItem.type == ItemType::CUP) {
                p2.heldItem.AddIce();
                Beep(1000, 80);
                if (p2.heldItem.state == DrinkState::RUINED) {
                    SetNotification("[!] Ruined! Ice added to raw/unprocessed base!", 2.0);
                } else {
                    SetNotification("Chef 2: Added Cold Ice Cube!", 1.2);
                }
                actionTaken = true;
            }
        }
        // P: Thêm Topping đặc biệt (Trân châu hoàng kim, Cacao, Bạc hà...)
        else if (GetAsyncKeyState(p2.keyTopping) & 0x8000) {
            if (!p2.heldItem.IsEmpty() && p2.heldItem.type == ItemType::CUP) {
                p2.heldItem.AddSpecialTopping();
                Beep(1100, 80);
                if (p2.heldItem.state == DrinkState::RUINED) {
                    SetNotification("[!] Ruined! Added incorrect specialty topping!", 2.0);
                } else {
                    SetNotification("Chef 2: Added premium specialty topping!", 1.2);
                }
                actionTaken = true;
            }
        }
        // K: Đổ bỏ ly hỏng (Thùng rác)
        else if (GetAsyncKeyState(p2.keyTrash) & 0x8000) {
            if (!p2.heldItem.IsEmpty()) {
                p2.heldItem.Clear();
                Beep(300, 180);
                SetNotification("Chef 2: Trash Bin cleared workstation!", 1.5);
                actionTaken = true;
            }
        }
        // L: Giao nước cho Khách hàng
        else if (GetAsyncKeyState(p2.keyDeliver) & 0x8000) {
            if (!p2.heldItem.IsEmpty() && p2.heldItem.type == ItemType::CUP) {
                if (p2.heldItem.finishedDrink != DrinkType::NONE) {
                    // Quét danh sách khách hàng khớp đơn
                    bool matched = false;
                    for (auto it = activeCustomers.begin(); it != activeCustomers.end(); ++it) {
                        if (it->orderType == p2.heldItem.finishedDrink) {
                            double pct = it->GetPatiencePercentage();
                            int tip = (pct > 55.0) ? it->bonusTips : 0;
                            int finalPoints = it->points + tip;
                            score += finalPoints;
                            p2.scoreContributed += finalPoints;
                            totalCustomersServed++;

                            Beep(1200, 80);
                            Beep(1600, 180);

                            std::string msg = "DELIVERY SUCCESS: " + GetDrinkName(it->orderType);
                            if (tip > 0) msg += " (+ Tips! +" + std::to_string(finalPoints) + " points)";
                            else msg += " (+" + std::to_string(finalPoints) + " points)";

                            SetNotification(msg, 2.5);
                            activeCustomers.erase(it);
                            p2.heldItem.Clear();
                            matched = true;
                            break;
                        }
                    }
                    if (!matched) {
                        score -= 40;
                        if (score < 0) score = 0;
                        p2.heldItem.Clear();
                        Beep(400, 350);
                        SetNotification("[!] WRONG DRINK! No customer ordered this! (-40 points)", 2.5);
                    }
                } else {
                    Beep(400, 200);
                    if (p2.heldItem.state == DrinkState::RUINED) {
                        SetNotification("[!] CANNOT DELIVER! The cup is ruined! Throw it away!", 2.0);
                    } else {
                        SetNotification("[!] CANNOT DELIVER! Missing ice or specialty toppings!", 2.0);
                    }
                }
                actionTaken = true;
            }
        }

        if (actionTaken) {
            p2.ResetCooldown();
        }
    }
}

void Game::UpdateEntities(double deltaTime) {
    // 1. Cập nhật thông báo sự kiện
    if (notificationTimer > 0.0) {
        notificationTimer -= deltaTime;
        if (notificationTimer <= 0.0) {
            notificationMsg = "";
        }
    }

    // 2. Cập nhật độ kiên nhẫn của khách hàng
    for (auto it = activeCustomers.begin(); it != activeCustomers.end();) {
        if (it->Update(deltaTime)) {
            score -= 30;
            if (score < 0) score = 0;
            Beep(250, 450);
            SetNotification("[-] " + it->name + " left out of patience! (-30 points)", 2.5);
            it = activeCustomers.erase(it);
        } else {
            ++it;
        }
    }

    // 3. Spawn khách hàng mới định kỳ
    customerSpawnTimer -= deltaTime;
    if (customerSpawnTimer <= 0.0) {
        if (activeCustomers.size() < 3) {
            SpawnCustomer();
        }
        customerSpawnTimer = 10.0 + (std::rand() % 10); // 10 đến 20 giây một khách mới
    }

    // 4. Cập nhật tiến trình của Thiết bị dùng chung (Shaker / Brewer)
    bool shakeDone = false;
    sharedShaker.Update(deltaTime, shakeDone);
    if (shakeDone) {
        Beep(1200, 150);
        SetNotification("[!] ALERT: Shaker finished mixing! Chef 1 can retrieve it.", 2.0);
    }

    bool brewDone = false;
    sharedBrewer.Update(deltaTime, brewDone);
    if (brewDone) {
        Beep(1200, 150);
        SetNotification("[!] ALERT: Brewer finished brewing! Chef 1 can retrieve it.", 2.0);
    }
}

void Game::DrawFrame() {
    Console::ResetCursor();
    std::stringstream ss;

    // --- GAME HEADER ---
    ss << Console::BOLD << Console::GOLD << "  === CO-OP DRINK MASTERS ===" << Console::RESET << "\n";
    ss << "  Time: " << Console::YELLOW << static_cast<int>(gameTimeRemaining) << "s" << Console::RESET 
       << "   Score: " << Console::GREEN << score << Console::RESET 
       << "   Served: " << totalCustomersServed << " cups\n";
    ss << " -------------------------------------------------------------\n";

    // --- CUSTOMER ORDERS PANEL ---
    ss << "  " << Console::BOLD << "[ACTIVE ORDERS]" << Console::RESET << "\n";
    if (activeCustomers.empty()) {
        ss << "    " << Console::GRAY << "(No customers waiting)" << Console::RESET << "\n";
    } else {
        for (const auto& c : activeCustomers) {
            std::string patColor = Console::GREEN;
            double patPct = c.GetPatiencePercentage();
            if (patPct < 30.0) patColor = Console::RED;
            else if (patPct < 60.0) patColor = Console::YELLOW;

            ss << "    • " << Console::BOLD << c.name << Console::RESET << " : " 
               << Console::MAGENTA << GetDrinkName(c.orderType) << Console::RESET 
               << " [" << patColor << c.GetProgressBar(10) << Console::RESET << "] "
               << Console::GRAY << "(" << static_cast<int>(c.remainingPatience) << "s)" << Console::RESET << "\n";
        }
    }
    ss << " -------------------------------------------------------------\n";

    // --- SHARED APPLIANCES AREA ---
    ss << "  " << Console::BOLD << "[SHARED APPLIANCES]" << Console::RESET << "\n";
    
    // Shared Table
    ss << "  * Shared Table : ";
    if (sharedTable.IsEmpty()) {
        ss << Console::GRAY << "Empty" << Console::RESET << "\n";
    } else {
        ss << Console::YELLOW << sharedTable.item.GetName() << Console::RESET << "\n";
    }

    // Shaker
    ss << "  * Shaker       : ";
    if (!sharedShaker.item.IsEmpty()) {
        if (sharedShaker.isActive) {
            int progress = static_cast<int>(sharedShaker.GetProgressPercentage());
            ss << Console::CYAN << "Shaking " << progress << "% [";
            int bars = progress / 10;
            for (int b = 0; b < 10; ++b) {
                if (b < bars) ss << "=";
                else ss << "-";
            }
            ss << "]" << Console::RESET << "\n";
        } else {
            ss << Console::GREEN << "Shaking Done! (Chef 1 retrieve: " << sharedShaker.item.GetName() << ")" << Console::RESET << "\n";
        }
    } else {
        ss << Console::GRAY << "Empty" << Console::RESET << "\n";
    }

    // Brewer
    ss << "  * Brewer       : ";
    if (!sharedBrewer.item.IsEmpty()) {
        if (sharedBrewer.isActive) {
            int progress = static_cast<int>(sharedBrewer.GetProgressPercentage());
            ss << Console::CYAN << "Brewing " << progress << "% [";
            int bars = progress / 10;
            for (int b = 0; b < 10; ++b) {
                if (b < bars) ss << "=";
                else ss << "-";
            }
            ss << "]" << Console::RESET << "\n";
        } else {
            ss << Console::GREEN << "Brewing Done! (Chef 1 retrieve: " << sharedBrewer.item.GetName() << ")" << Console::RESET << "\n";
        }
    } else {
        ss << Console::GRAY << "Empty" << Console::RESET << "\n";
    }
    ss << " -------------------------------------------------------------\n";

    // --- PLAYER WORKSTATIONS ---
    std::string p1ItemDesc = p1.heldItem.IsEmpty() ? "Empty" : p1.heldItem.GetName();
    std::string p2ItemDesc = p2.heldItem.IsEmpty() ? "Empty" : p2.heldItem.GetName();

    ss << "  " << Console::BOLD << Console::RED << "[CHEF 1 - Prep]   " << Console::RESET << ": " << Console::YELLOW << p1ItemDesc << Console::RESET << "\n";
    ss << "  " << Console::BOLD << Console::BLUE << "[CHEF 2 - Barista]" << Console::RESET << ": " << Console::YELLOW << p2ItemDesc << Console::RESET << "\n";
    ss << " -------------------------------------------------------------\n";

    // --- EVENT BAR ---
    if (!notificationMsg.empty()) {
        ss << "  [EVENT] " << Console::BOLD << Console::CYAN << notificationMsg << Console::RESET << "\n";
    } else {
        ss << "\n";
    }

    std::string frame = ss.str();
    std::string output;
    output.reserve(frame.size() + 256);
    for (char c : frame) {
        if (c == '\n') {
            output += "\033[K\n";
        } else {
            output += c;
        }
    }
    // Erase everything from the cursor to the end of the screen
    output += "\033[J";
    std::cout << output << std::flush;
}

void Game::Run() {
    running = true;
    lastFrameTime = std::chrono::steady_clock::now();

    while (running && gameTimeRemaining > 0.0) {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = now - lastFrameTime;
        double deltaTime = diff.count();
        lastFrameTime = now;

        gameTimeRemaining -= deltaTime;

        ProcessInputs(deltaTime);
        UpdateEntities(deltaTime);
        DrawFrame();

        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    running = false;
    ShowGameOver();
}

void Game::ShowMenu() {
    Console::Clear();
    std::cout << "\n\n";
    std::cout << Console::BOLD << Console::GOLD;
    std::cout << "   ===================================================================\n";
    std::cout << "   *                                                                 *\n";
    std::cout << "   *        OVERCOOKED: DRINK MASTERS (TEXT ASYMMETRIC 2P)           *\n";
    std::cout << "   *                                                                 *\n";
    std::cout << "   ===================================================================\n\n" << Console::RESET;
    
    std::cout << "   Welcome, Chefs, to the ultimate co-op beverage assembly line!\n";
    std::cout << "   You must cooperate smoothly, each having a unique specialized role:\n\n";

    std::cout << "   " << Console::BOLD << Console::RED << "[CHEF 1 - Prep Chef (Left Side)]" << Console::RESET << ":\n";
    std::cout << "      Task: Grab empty cups, mix raw base ingredients (Tea, Milk, Coffee...)\n";
    std::cout << "      and operate the Shaker / Brewer. Once the drink base is completed,\n";
    std::cout << "      place the cup onto the Shared Table.\n";
    std::cout << "      *You cannot add ice, specialty toppings, or deliver drinks.*\n\n";

    std::cout << "   " << Console::BOLD << Console::BLUE << "[CHEF 2 - Barista & Deliverer (Right Side)]" << Console::RESET << ":\n";
    std::cout << "      Task: Retrieve the processed drink base from the Shared Table,\n";
    std::cout << "      add Cold Ice, add Premium Specialty Toppings (per recipe),\n";
    std::cout << "      and DELIVER the completed drink to the waiting customers.\n";
    std::cout << "   " << Console::BOLD << "WORKSTATION HOTKEYS:" << Console::RESET << "\n";
    std::cout << "   +-------------------------------------+-------------------------------------+\n";
    std::cout << "   | " << Console::BOLD << Console::RED << "RED CHEF 1 (Prep Workstation)" << Console::RESET << "       | " << Console::BOLD << Console::BLUE << "BLUE CHEF 2 (Barista Workstation)" << Console::RESET << "   |\n";
    std::cout << "   +-------------------------------------+-------------------------------------+\n";
    std::cout << "   |  Q : Grab empty cup                 |  I : Toggle cup with Shared Table   |\n";
    std::cout << "   |  W : Add Tea Bag                    |  O : Add Cold Ice Cube (required)   |\n";
    std::cout << "   |  E : Add Condensed Milk             |  P : Add Specialty Topping (auto)   |\n";
    std::cout << "   |  R : Add Coffee Ground              |  L : DELIVER COMPLETED DRINK        |\n";
    std::cout << "   |  T : Add Black Boba                 |  K : Discard ruined cup (Trash)     |\n";
    std::cout << "   |  A : Add Peach Slice                |                                     |\n";
    std::cout << "   |  S : Add Salt Cream                 |                                     |\n";
    std::cout << "   |  Z : Place into/Get from Shaker     |                                     |\n";
    std::cout << "   |  X : Place into/Get from Brewer     |                                     |\n";
    std::cout << "   |  C : Place on/Get from Shared Table |                                     |\n";
    std::cout << "   |  V : Discard ruined cup (Trash)     |                                     |\n";
    std::cout << "   +-------------------------------------+-------------------------------------+\n\n";

    std::cout << Console::BOLD << "   CO-OP ASSEMBLY LINE EXAMPLE (Boba Milk Tea):\n" << Console::RESET;
    std::cout << "   1. Chef 1: Grab cup (Q) -> Add Tea (W) -> Add Milk (E) -> Add Boba (T) -> Place in Shaker (Z).\n";
    std::cout << "   2. Shaker finishes: Chef 1 retrieves cup and places it on the Shared Table (C).\n";
    std::cout << "   3. Chef 2: Grab cup from Table (I) -> Add Ice (O) -> Add Golden Boba topping (P) -> Deliver (L)!\n\n";

    std::cout << Console::GOLD << "   >> PRESS SPACE TO START THE GAME! <<\n" << Console::RESET;

    while (true) {
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            Beep(800, 100);
            Beep(1200, 150);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    Console::Clear();
    Initialize();
    Run();
}

void Game::ShowGameOver() {
    Console::Clear();
    std::cout << "\n\n";
    std::cout << Console::BOLD << Console::RED;
    std::cout << "   ===================================================================\n";
    std::cout << "   *                           TIME IS UP!                           *\n";
    std::cout << "   ===================================================================\n\n" << Console::RESET;

    std::cout << "   Cooperative Performance Summary:\n";
    std::cout << "   --------------------------------------------\n";
    std::cout << "   [-] Total Score Achieved: " << Console::BOLD << Console::GOLD << score << " points" << Console::RESET << "\n";
    std::cout << "   [-] Completed Deliveries: " << totalCustomersServed << " cups\n\n";

    std::cout << "   Individual Contributions:\n";
    std::cout << "   - Chef 1: Prepared the core base ingredients for all recipes.\n";
    std::cout << "   - Chef 2: " << Console::BLUE << p2.scoreContributed << " points" << Console::RESET << " from finishing toppings & delivery.\n\n";

    if (score >= 600) {
        std::cout << Console::GREEN << "   * EVALUATION: Legendary Duo! Flawless assembly speed! *\n\n" << Console::RESET;
    } else if (score >= 350) {
        std::cout << Console::YELLOW << "   * EVALUATION: Great teamwork! The beverage stand is thriving! *\n\n" << Console::RESET;
    } else {
        std::cout << Console::RED << "   * EVALUATION: Try communicating more to plan which base to prep first! *\n\n" << Console::RESET;
    }

    std::cout << "   >> PRESS SPACE TO RETURN TO MAIN MENU <<\n";
    std::cout << "   >> PRESS ESC TO EXIT GAME <<\n";

    Beep(800, 200);
    Beep(600, 200);
    Beep(900, 400);

    while (true) {
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            Beep(800, 100);
            ShowMenu();
            break;
        }
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            Beep(400, 100);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
