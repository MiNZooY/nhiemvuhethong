#include <iostream>
#include <string>
#include <vector>
#include "AVL.hpp"
#include "BST.hpp"
#include "Queue.hpp"
#include "Stack.hpp"
#include "Algorithms.hpp"

using namespace std;

// --- TRINH SINH SO NGAU NHIEN DON GIAN (LCG PRNG) ---
// Thay the cho <cstdlib> rand() va <ctime> time()
unsigned int prngState = 12345;
unsigned int getPRNG() {
    prngState = prngState * 1103515245 + 12345;
    return (prngState / 65536) % 32768;
}

// --- HAM TIM GIA TRI NHO NHAT (MIN) ---
// Thay the cho <algorithm> min
int getMin(int a, int b) {
    return (a < b) ? a : b;
}


// --- 1. CAU TRUC DU LIEU BO TRO ---

// Struct bieu dien Mon nuoc trong Menu (Luu vao AVL Tree)
struct MenuItem {
    string name;
    int price;
    int milkRequired;
    int teaRequired;

    MenuItem() : name(""), price(0), milkRequired(0), teaRequired(0) {}
    MenuItem(string n, int p, int m, int t) : name(n), price(p), milkRequired(m), teaRequired(t) {}

    // Overload cac toan tu so sanh de AVL Tree sap xep theo bang chu cai cua ten mon
    bool operator<(const MenuItem& other) const {
        return name < other.name;
    }
    bool operator>(const MenuItem& other) const {
        return name > other.name;
    }
    bool operator==(const MenuItem& other) const {
        return name == other.name;
    }
};

// Nut in an cho MenuItem khi duyet Inorder AVL Tree
ostream& operator<<(ostream& os, const MenuItem& item) {
    os << "\n  + " << item.name << " | Gia: " << item.price << "k (Can: " << item.milkRequired << "ml Sua, " << item.teaRequired << "g Tra)";
    return os;
}

// Struct bieu dien Khach hang trong hang doi (Luu vao Queue)
struct Customer {
    string drinkName;
    int price;
    int milkRequired;
    int teaRequired;

    Customer() : drinkName(""), price(0), milkRequired(0), teaRequired(0) {}
    Customer(string n, int p, int m, int t) : drinkName(n), price(p), milkRequired(m), teaRequired(t) {}
    Customer(const MenuItem& item) : drinkName(item.name), price(item.price), milkRequired(item.milkRequired), teaRequired(item.teaRequired) {}
};

// Nut in an cho Customer trong hang doi Queue
ostream& operator<<(ostream& os, const Customer& c) {
    os << "\"" << c.drinkName << " (" << c.price << "k)\"";
    return os;
}

// Struct bieu dien Hanh dong de thuc hien Hoan tac (Luu vao Stack)
struct Action {
    string description;
    int milkDiff;
    int teaDiff;
    int iceDiff;
    int moneyDiff;
    bool isServed;
    Customer customer;

    Action() : description(""), milkDiff(0), teaDiff(0), iceDiff(0), moneyDiff(0), isServed(false), customer() {}
    Action(string desc, int m, int t, int i, int money, bool served = false, Customer cust = Customer())
        : description(desc), milkDiff(m), teaDiff(t), iceDiff(i), moneyDiff(money), isServed(served), customer(cust) {}
};

// Struct bieu dien trang thai pha che cua tung mon dung cho thuat toan sap xep (bubbleSort)
struct MenuItemServings {
    string name;
    int servingsLeft;

    MenuItemServings() : name(""), servingsLeft(0) {}
    MenuItemServings(string n, int s) : name(n), servingsLeft(s) {}

    // Overload toan tu so sanh de sap xep tang dan so luong phuc vu (mon sap het len truoc)
    bool operator<(const MenuItemServings& other) const {
        return servingsLeft < other.servingsLeft;
    }
    bool operator>(const MenuItemServings& other) const {
        return servingsLeft > other.servingsLeft;
    }
    bool operator<=(const MenuItemServings& other) const {
        return servingsLeft <= other.servingsLeft;
    }
    bool operator>=(const MenuItemServings& other) const {
        return servingsLeft >= other.servingsLeft;
    }
};


// --- 2. HONG TRA VONG LAP TRO CHOI ---

int main() {
    // 1. Khoi tao Trang thai Tiem
    int day = 1;
    int milk = 500;  // ml
    int tea = 200;   // g
    int ice = 10;    // vien
    int money = 100; // k

    // 2. Khoi tao AVL Tree chua Menu quan
    AVL<MenuItem> menuAVL;
    menuAVL.insert(MenuItem("Tra Sua", 30, 100, 50));
    menuAVL.insert(MenuItem("Hong Tra", 20, 0, 60));
    menuAVL.insert(MenuItem("Sua Nong", 15, 150, 0));
    menuAVL.insert(MenuItem("Tra Sua Dac Biet", 45, 120, 80));

    // 3. Khoi tao BSTree de quan ly doanh thu theo hoa don trong ngay
    BST<int> revenueBST;

    // 4. Khoi tao Queue hang doi khach hang
    Queue<Customer> customerQueue(100);

    // 5. Khoi tao Stack luu lich su thao tac de co the Hoan tac (Undo)
    Stack<Action> actionStack(100);

    // Sinh khach hang ngau nhien moi ngay moi
    MenuItem availableItems[] = {
        {"Tra Sua", 30, 100, 50},
        {"Hong Tra", 20, 0, 60},
        {"Sua Nong", 15, 150, 0},
        {"Tra Sua Dac Biet", 45, 120, 80}
    };

    // Sinh khach cho Ngay 1
    for (int i = 0; i < 3; i++) {
        int idx = getPRNG() % 4;
        customerQueue.enqueue(Customer(availableItems[idx]));
    }

    // Vong lap chinh cua tro choi
    while (true) {
        cout << "\n==================================================" << endl;
        cout << "        --- TIEM NUOC SIMULATOR (Ngay " << day << ") ---" << endl;
        cout << "==================================================" << endl;
        cout << "[KHO] Sua: " << milk << "ml | Tra: " << tea << "g | Da: " << ice << " vien | Tien: " << money << "k" << endl;
        cout << "--------------------------------------------------" << endl;

        // Hien thi khach hang tiep theo va hang doi khach hang
        if (!customerQueue.empty()) {
            Customer currentCust = customerQueue.front();
            cout << "KHACH HANG MOI: \"Mot ly " << currentCust.drinkName << " gia " << currentCust.price << "k!\"" << endl;
            cout << "Hang doi khach: ";
            customerQueue.print();
        } else {
            cout << "KHACH HANG MOI: Da phuc vu het khach trong ngay! Hay ket thuc ngay de qua ngay moi." << endl;
        }
        cout << "--------------------------------------------------" << endl;

        cout << "BAN MUON LAM GI?" << endl;
        if (!customerQueue.empty()) {
            Customer currentCust = customerQueue.front();
            cout << "1. Pha che mon \"" << currentCust.drinkName << "\" (Ton: " << currentCust.milkRequired << "ml Sua, " << currentCust.teaRequired << "g Tra, 1 vien Da)" << endl;
        } else {
            cout << "1. Pha che mon nuoc (Hien khong co khach nao)" << endl;
        }
        cout << "2. Xem Menu quan (Duyet AVL Tree)" << endl;
        cout << "3. Di cho mua nguyen lieu" << endl;
        cout << "4. Kiem tra mon sap het (Chay Thuat toan Sort)" << endl;
        cout << "5. Ket thuc ngay va xem doanh thu (Duyet BSTree)" << endl;
        cout << "6. Hoan tac hanh dong vua thuc hien (Su dung Stack)" << endl;
        cout << "7. Thoat game" << endl;
        cout << "\nNhap lua chon cua ban (1-7): ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            string dummy;
            cin >> dummy;
            cout << "Lua chon khong hop le! Vui long nhap so tu 1 den 7." << endl;
            continue;
        }

        // --- XU LY LUA CHON ---

        if (choice == 1) {
            // Option 1: Pha che mon cho khach hang hien tai
            if (customerQueue.empty()) {
                cout << "Hang doi trong! Khong co khach hang nao de phuc vu." << endl;
                continue;
            }

            Customer currentCust = customerQueue.front();

            // Kiem tra nguyen lieu
            if (milk < currentCust.milkRequired || tea < currentCust.teaRequired || ice < 1) {
                cout << "That bai: Khong du nguyen lieu de pha che mon nay!" << endl;
                if (milk < currentCust.milkRequired) cout << " - Thieu Sua: " << (currentCust.milkRequired - milk) << "ml nua." << endl;
                if (tea < currentCust.teaRequired) cout << " - Thieu Tra: " << (currentCust.teaRequired - tea) << "g nua." << endl;
                if (ice < 1) cout << " - Thieu Da: Can it nhat 1 vien." << endl;
                continue;
            }

            // Tru nguyen lieu va cong tien
            milk -= currentCust.milkRequired;
            tea -= currentCust.teaRequired;
            ice -= 1;
            money += currentCust.price;

            // Them hoa don vao BSTree doanh thu ngay
            revenueBST.insert(currentCust.price);

            // Day vao Stack de hoan tac
            actionStack.push(Action("Pha che " + currentCust.drinkName, -currentCust.milkRequired, -currentCust.teaRequired, -1, currentCust.price, true, currentCust));

            // Loai bo khach hang da phuc vu ra khoi Queue
            customerQueue.dequeue();

            cout << "Pha che thanh cong mon \"" << currentCust.drinkName << "\". Ban nhan duoc " << currentCust.price << "k!" << endl;
        }
        else if (choice == 2) {
            // Option 2: Xem Menu (Duyet AVL Tree Inorder)
            cout << "\n==================================================" << endl;
            cout << "        --- MENU QUAN (AVL TREE INORDER) ---" << endl;
            cout << "==================================================" << endl;
            menuAVL.inorder();
            cout << "==================================================" << endl;
        }
        else if (choice == 3) {
            // Option 3: Di cho mua nguyen lieu
            cout << "\n==================================================" << endl;
            cout << "        --- CUA HANG NGUYEN LIEU (DI CHO) ---" << endl;
            cout << "==================================================" << endl;
            cout << "Tien mat hien co: " << money << "k" << endl;
            cout << "1. Mua Sua (+200ml) - Gia: 15k" << endl;
            cout << "2. Mua Tra (+100g) - Gia: 20k" << endl;
            cout << "3. Mua Da (+10 vien) - Gia: 5k" << endl;
            cout << "4. Tro ve" << endl;
            cout << "Nhap lua chon cua ban (1-4): ";

            int shopChoice;
            if (!(cin >> shopChoice)) {
                cin.clear();
                string dummy;
                cin >> dummy;
                cout << "Lua chon khong hop le!" << endl;
                continue;
            }

            if (shopChoice == 1) {
                if (money < 15) {
                    cout << "Khong du tien de mua Sua!" << endl;
                } else {
                    money -= 15;
                    milk += 200;
                    actionStack.push(Action("Mua Sua", 200, 0, 0, -15));
                    cout << "Da mua 200ml Sua! Tien mat con lai: " << money << "k." << endl;
                }
            }
            else if (shopChoice == 2) {
                if (money < 20) {
                    cout << "Khong du tien de mua Tra!" << endl;
                } else {
                    money -= 20;
                    tea += 100;
                    actionStack.push(Action("Mua Tra", 0, 100, 0, -20));
                    cout << "Da mua 100g Tra! Tien mat con lai: " << money << "k." << endl;
                }
            }
            else if (shopChoice == 3) {
                if (money < 5) {
                    cout << "Khong du tien de mua Da!" << endl;
                } else {
                    money -= 5;
                    ice += 10;
                    actionStack.push(Action("Mua Da", 0, 0, 10, -5));
                    cout << "Da mua 10 vien Da! Tien mat con lai: " << money << "k." << endl;
                }
            }
            else if (shopChoice == 4) {
                cout << "Dang quay tro lai tiem nuoc..." << endl;
            }
            else {
                cout << "Lua chon khong hop le!" << endl;
            }
        }
        else if (choice == 4) {
            // Option 4: Kiem tra mon sap het (Bubble Sort)
            MenuItemServings itemsToBest[4];

            // 1. Tra Sua (Can 100ml Sua, 50g Tra)
            int tsServings = getMin(milk / 100, tea / 50);
            itemsToBest[0] = MenuItemServings("Tra Sua", tsServings);

            // 2. Hong Tra (Can 0ml Sua, 60g Tra)
            int htServings = tea / 60;
            itemsToBest[1] = MenuItemServings("Hong Tra", htServings);

            // 3. Sua Nong (Can 150ml Sua, 0g Tra)
            int snServings = milk / 150;
            itemsToBest[2] = MenuItemServings("Sua Nong", snServings);

            // 4. Tra Sua Dac Biet (Can 120ml Sua, 80g Tra)
            int tsdbServings = getMin(milk / 120, tea / 80);
            itemsToBest[3] = MenuItemServings("Tra Sua Dac Biet", tsdbServings);

            // Su dung thuat toan bubbleSort tu Algorithms.hpp de sap xep tang dan so luong ly pha duoc
            bubbleSort(itemsToBest, 4);

            cout << "\n==================================================" << endl;
            cout << "    --- KIEM TRA MON SAP HET (BUBBLE SORT) ---" << endl;
            cout << "==================================================" << endl;
            cout << "Sap xep theo so luong ly co the pha che (Tang dan):" << endl;
            for (int i = 0; i < 4; i++) {
                cout << "  " << i + 1 << ". Mon \"" << itemsToBest[i].name 
                     << "\" -> Con pha che duoc: " << itemsToBest[i].servingsLeft << " ly." << endl;
            }
            cout << "==================================================" << endl;
        }
        else if (choice == 5) {
            // Option 5: Ket thuc ngay va xem doanh thu (Duyet BSTree)
            cout << "\n==================================================" << endl;
            cout << "       --- KET THUC NGAY " << day << " ---" << endl;
            cout << "==================================================" << endl;
            cout << "Tong ket doanh thu cac hoa don da ban hom nay (BSTree Inorder):" << endl;
            if (revenueBST.root == nullptr) {
                cout << "  Hom nay tiem khong ban duoc ly nuoc nao! Doanh thu: 0k." << endl;
            } else {
                cout << "  Gia cac ly nuoc da ban (duyet tang dan): ";
                revenueBST.inorder(); // In ra cac hoa don doanh thu
            }
            cout << "--------------------------------------------------" << endl;

            // Xoa bo dem dau vao va cho nhan Enter
            cin.ignore(10000, '\n');
            cout << "Nhan Enter de don dep quan va sang Ngay " << day + 1 << "...";
            cin.get();

            // Chuyen ngay
            day++;

            // Don dep hang doi cu va sinh khach hang ngay moi
            while (!customerQueue.empty()) {
                customerQueue.dequeue();
            }
            for (int i = 0; i < 3; i++) {
                int idx = getPRNG() % 4;
                customerQueue.enqueue(Customer(availableItems[idx]));
            }

            // Khoi tao lai BSTree doanh thu cho ngay moi
            revenueBST.root = nullptr;

            // Giai phong stack lich su hoan tac cua ngay cu (khong cho phep hoan tac xuyen ngay)
            while (!actionStack.empty()) {
                actionStack.pop();
            }

            cout << "\nChao ngay moi! Ngay " << day << " da bat dau." << endl;
        }
        else if (choice == 6) {
            // Option 6: Hoan tac hanh dong truoc (Su dung Stack)
            if (actionStack.empty()) {
                cout << "Khong co hanh dong nao gan day de hoan tac!" << endl;
                continue;
            }

            Action act = actionStack.pop();

            // Khui phuc tai nguyen
            milk -= act.milkDiff;
            tea -= act.teaDiff;
            ice -= act.iceDiff;
            money -= act.moneyDiff;

            // Neu hanh dong truoc do la pha che khach hang, ta hoan tra khach hang ve hang dau Queue
            if (act.isServed) {
                if (customerQueue.frontIndex > 0) {
                    customerQueue.frontIndex--;
                    customerQueue.data[customerQueue.frontIndex] = act.customer;
                } else {
                    customerQueue.frontIndex--;
                    customerQueue.data[customerQueue.frontIndex] = act.customer;
                }
            }

            cout << "Hoan tac thanh cong hanh dong: \"" << act.description << "\"." << endl;
        }
        else if (choice == 7) {
            // Option 7: Thoat
            cout << "Cam on ban da choi Tiem Nuoc Simulator! Tam biet." << endl;
            break;
        }
        else {
            cout << "Lua chon khong hop le! Vui long nhap so tu 1 den 7." << endl;
        }
    }

    return 0;
}
