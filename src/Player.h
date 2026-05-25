#ifndef PLAYER_H
#define PLAYER_H

#include "Recipe.h"
#include <windows.h>

class Player {
public:
    int id;               // 1 hoặc 2
    HeldItem heldItem;    // Ly nước đang giữ trên bàn pha chế cá nhân
    int scoreContributed;

    // Phím bấm điều khiển
    int keyCup;           // Lấy ly
    int keyTea;           // Thêm Trà
    int keyMilk;          // Thêm Sữa đặc
    int keyCoffee;        // Thêm Cà Phê
    int keyBoba;          // Thêm Trân Châu Đen
    int keyPeach;         // Thêm Đào Cam Sả
    int keySalt;          // Thêm Kem Muối Béo
    
    // Thiết bị chung
    int keyShaker;        // Tương tác máy lắc
    int keyBrewer;        // Tương tác máy ủ
    int keySharedTable;   // Tương tác bàn chung
    int keyTrash;         // Thùng rác (Đổ bỏ)
    int keyDeliver;       // Giao nước cho khách (Chỉ Player 2 hoặc cả hai)

    // Khâu hoàn thiện của Player 2
    int keyIce;           // Thêm Đá
    int keyTopping;       // Thêm Topping đặc biệt

    // Cooldown phím bấm để tránh bị trùng lặp khi đè phím
    double keyCooldown = 0.0;

    Player(int playerId);

    void UpdateCooldown(double deltaTime);
    bool CanInteract() const { return keyCooldown <= 0.0; }
    void ResetCooldown() { keyCooldown = 0.20; } // 200ms cooldown giữa các thao tác
};

#endif // PLAYER_H
