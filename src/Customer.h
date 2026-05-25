#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "Recipe.h"
#include <string>

struct Customer {
    std::string name;
    DrinkType orderType = DrinkType::NONE;
    double maxPatience = 50.0; // Seconds
    double remainingPatience = 50.0;
    int points = 100;
    int bonusTips = 50;

    bool Update(double deltaTime) {
        remainingPatience -= deltaTime;
        return remainingPatience <= 0.0;
    }

    double GetPatiencePercentage() const {
        double pct = (remainingPatience / maxPatience) * 100.0;
        return pct < 0.0 ? 0.0 : pct;
    }

    std::string GetProgressBar(int width = 10) const {
        double pct = GetPatiencePercentage();
        int filled = static_cast<int>((pct / 100.0) * width);
        std::string bar = "";
        for (int i = 0; i < width; ++i) {
            if (i < filled) bar += "#";
            else bar += "-";
        }
        return bar;
    }
};

#endif // CUSTOMER_H
