#include "Player.h"

Player::Player(int playerId) {
    id = playerId;
    scoreContributed = 0;
    heldItem.Clear();
    keyCooldown = 0.0;

    // Thiết lập phím nóng bất đối xứng
    if (id == 1) {
        // --- ĐẦU BẾP 1 (Prep Chef - Trái) ---
        keyCup         = 0x51; // Q
        keyTea         = 0x57; // W
        keyMilk        = 0x45; // E
        keyCoffee      = 0x52; // R
        keyBoba        = 0x54; // T
        keyPeach       = 0x41; // A
        keySalt        = 0x53; // S
        
        keyShaker      = 0x5A; // Z
        keyBrewer      = 0x58; // X
        keySharedTable = 0x43; // C
        keyTrash       = 0x56; // V
        keyDeliver     = -1;   // Không thể giao hàng

        keyIce         = -1;   // Không có đá
        keyTopping     = -1;   // Không có topping
    } else {
        // --- ĐẦU BẾP 2 (Barista/Finisher - Phải) ---
        keyCup         = -1;
        keyTea         = -1;
        keyMilk        = -1;
        keyCoffee      = -1;
        keyBoba        = -1;
        keyPeach       = -1;
        keySalt        = -1;
        
        keyShaker      = -1;
        keyBrewer      = -1;
        keySharedTable = 0x49; // I (Tương tác Bàn gỗ chung)
        keyTrash       = 0x4B; // K (Thùng rác)
        keyDeliver     = 0x4C; // L (Giao hàng)

        keyIce         = 0x4F; // O (Thêm Đá Viên)
        keyTopping     = 0x50; // P (Thêm Topping đặc biệt)
    }
}

void Player::UpdateCooldown(double deltaTime) {
    if (keyCooldown > 0.0) {
        keyCooldown -= deltaTime;
    }
}
