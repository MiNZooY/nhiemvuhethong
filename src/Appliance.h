#ifndef APPLIANCE_H
#define APPLIANCE_H

#include "Recipe.h"
#include <string>

// Bàn Gỗ Trung Chuyển (Shared Table)
struct SharedTable {
    HeldItem item;

    SharedTable() {
        item.Clear();
    }

    bool IsEmpty() const {
        return item.IsEmpty();
    }

    void Place(const HeldItem& newItem) {
        item = newItem;
    }

    HeldItem Take() {
        HeldItem temp = item;
        item.Clear();
        return temp;
    }
};

// Máy Lắc (Shaker)
struct Shaker {
    HeldItem item;
    double progressTime = 0.0;
    double maxTime = 3.0; // 3 giây lắc nước
    bool isActive = false;

    Shaker() {
        item.Clear();
        progressTime = 0.0;
        isActive = false;
    }

    void Start(const HeldItem& newItem) {
        item = newItem;
        item.state = DrinkState::SHAKING;
        progressTime = 0.0;
        isActive = true;
    }

    void Update(double deltaTime, bool& beepTrigger) {
        if (!isActive) return;

        progressTime += deltaTime;
        if (progressTime >= maxTime) {
            isActive = false;
            item.state = DrinkState::SHAKEN;
            beepTrigger = true; // Báo hiệu đã lắc xong
        }
    }

    double GetProgressPercentage() const {
        if (!isActive) return 0.0;
        double pct = (progressTime / maxTime) * 100.0;
        return pct > 100.0 ? 100.0 : pct;
    }

    HeldItem Take() {
        HeldItem temp = item;
        item.Clear();
        progressTime = 0.0;
        isActive = false;
        return temp;
    }
};

// Máy Ủ Cà Phê (Brewer)
struct Brewer {
    HeldItem item;
    double progressTime = 0.0;
    double maxTime = 4.0; // 4 giây ủ cà phê
    bool isActive = false;

    Brewer() {
        item.Clear();
        progressTime = 0.0;
        isActive = false;
    }

    void Start(const HeldItem& newItem) {
        item = newItem;
        item.state = DrinkState::BREWING;
        progressTime = 0.0;
        isActive = true;
    }

    void Update(double deltaTime, bool& beepTrigger) {
        if (!isActive) return;

        progressTime += deltaTime;
        if (progressTime >= maxTime) {
            isActive = false;
            item.state = DrinkState::BREWED;
            beepTrigger = true; // Báo hiệu đã ủ xong
        }
    }

    double GetProgressPercentage() const {
        if (!isActive) return 0.0;
        double pct = (progressTime / maxTime) * 100.0;
        return pct > 100.0 ? 100.0 : pct;
    }

    HeldItem Take() {
        HeldItem temp = item;
        item.Clear();
        progressTime = 0.0;
        isActive = false;
        return temp;
    }
};

#endif // APPLIANCE_H
