#ifndef RECIPE_H
#define RECIPE_H

#include <string>
#include <vector>
#include <algorithm>

enum class ItemType {
    NONE,
    CUP,
    TEA,            // Trà túi lọc
    MILK,           // Sữa đặc
    BOBA,           // Trân châu đen
    COFFEE,         // Bột cà phê
    PEACH,          // Đào cam sả
    SALT_CREAM,     // Kem muối béo
    ICE,            // Đá viên (Người 2 thêm)
    TOPPING         // Topping đặc biệt (Người 2 thêm)
};

enum class DrinkState {
    RAW,
    SHAKING,
    SHAKEN,
    BREWING,
    BREWED,
    RUINED
};

enum class DrinkType {
    NONE,
    BOBA_MILK_TEA,    // Trà Sữa Trân Châu: Cốt (Trà+Sữa+TrânChâu -> Lắc) + Đá + TrânChâuHoàngKim
    PEACH_TEA,        // Trà Đào Cam Sả: Cốt (Trà+Đào -> Lắc) + Đá + Lá Bạc Hà
    ICED_MILK_COFFEE, // Cà Phê Sữa Đá: Cốt (CàPhê+Sữa -> Ủ) + Đá
    SALTED_COFFEE     // Cà Phê Muối: Cốt (CàPhê+Sữa+KemMuối -> Ủ) + Đá + Bột Cacao
};

struct HeldItem {
    ItemType type = ItemType::NONE;
    std::vector<ItemType> cupIngredients; // Danh sách nguyên liệu trong ly
    DrinkState state = DrinkState::RAW;
    DrinkType finishedDrink = DrinkType::NONE;

    // Các thành phần của khâu hoàn thiện (Người 2 thêm)
    bool hasIce = false;
    bool hasSpecialTopping = false;

    void Clear() {
        type = ItemType::NONE;
        cupIngredients.clear();
        state = DrinkState::RAW;
        finishedDrink = DrinkType::NONE;
        hasIce = false;
        hasSpecialTopping = false;
    }

    bool IsEmpty() const {
        return type == ItemType::NONE;
    }

    std::string GetName() const {
        switch (type) {
            case ItemType::NONE: return "Empty Hand";
            case ItemType::TEA: return "Tea Bag";
            case ItemType::MILK: return "Condensed Milk";
            case ItemType::BOBA: return "Black Boba";
            case ItemType::COFFEE: return "Coffee Ground";
            case ItemType::PEACH: return "Peach Slice";
            case ItemType::SALT_CREAM: return "Salt Cream";
            case ItemType::ICE: return "Ice Cube";
            case ItemType::TOPPING: return "Specialty Topping";
            case ItemType::CUP: {
                if (finishedDrink != DrinkType::NONE) {
                    switch (finishedDrink) {
                        case DrinkType::BOBA_MILK_TEA: return "Perfect Boba Milk Tea! *";
                        case DrinkType::PEACH_TEA: return "Refreshing Peach Tea! *";
                        case DrinkType::ICED_MILK_COFFEE: return "Bold Iced Milk Coffee! *";
                        case DrinkType::SALTED_COFFEE: return "Premium Salted Coffee! *";
                        default: return "Strange Drink";
                    }
                }
                if (state == DrinkState::RUINED) {
                    return "Ruined Drink [!] (Throw away!)";
                }
                if (cupIngredients.empty()) {
                    return "Empty Cup";
                }

                // Core base cup description
                std::string desc = "Base Cup (";
                for (size_t i = 0; i < cupIngredients.size(); ++i) {
                    switch (cupIngredients[i]) {
                        case ItemType::TEA: desc += "Tea"; break;
                        case ItemType::MILK: desc += "Milk"; break;
                        case ItemType::BOBA: desc += "Boba"; break;
                        case ItemType::COFFEE: desc += "Coffee"; break;
                        case ItemType::PEACH: desc += "Peach"; break;
                        case ItemType::SALT_CREAM: desc += "SaltCream"; break;
                        default: break;
                    }
                    if (i < cupIngredients.size() - 1) desc += "+";
                }
                desc += ")";

                // Processing states
                if (state == DrinkState::SHAKING) desc += " [Shaking...]";
                else if (state == DrinkState::BREWING) desc += " [Brewing...]";
                else if (state == DrinkState::SHAKEN) desc += " [Shaken]";
                else if (state == DrinkState::BREWED) desc += " [Brewed]";

                // Finishing toppings
                if (hasIce || hasSpecialTopping) {
                    desc += " + Extra:(";
                    if (hasIce) desc += "Ice";
                    if (hasSpecialTopping) {
                        if (hasIce) desc += "+";
                        desc += "Topping";
                    }
                    desc += ")";
                }

                return desc;
            }
        }
        return "Item";
    }

    bool HasIngredient(ItemType ing) const {
        return std::find(cupIngredients.begin(), cupIngredients.end(), ing) != cupIngredients.end();
    }

    void AddBaseIngredient(ItemType ing) {
        if (type != ItemType::CUP || state == DrinkState::RUINED || finishedDrink != DrinkType::NONE) return;
        // Người 1 chỉ thêm được nguyên liệu cơ bản, không thêm được đá/topping ở khâu 1
        if (ing == ItemType::ICE || ing == ItemType::TOPPING) return;
        if (HasIngredient(ing)) return; // Không trùng lặp
        
        cupIngredients.push_back(ing);
        CheckRecipeValidity();
    }

    void CheckRecipeValidity() {
        if (cupIngredients.size() > 4) {
            state = DrinkState::RUINED;
            return;
        }

        bool hasTea = HasIngredient(ItemType::TEA);
        bool hasMilk = HasIngredient(ItemType::MILK);
        bool hasBoba = HasIngredient(ItemType::BOBA);
        bool hasCoffee = HasIngredient(ItemType::COFFEE);
        bool hasPeach = HasIngredient(ItemType::PEACH);
        bool hasSalt = HasIngredient(ItemType::SALT_CREAM);

        // Hủy ly nếu kết hợp sai nền
        if (hasTea && hasCoffee) { state = DrinkState::RUINED; return; }
        if (hasBoba && hasCoffee) { state = DrinkState::RUINED; return; }
        if (hasPeach && hasCoffee) { state = DrinkState::RUINED; return; }
        if (hasTea && hasSalt) { state = DrinkState::RUINED; return; }
        if (hasPeach && hasMilk) { state = DrinkState::RUINED; return; }

        // Kiểm tra các hướng công thức tiềm năng
        bool potentialBoba = (!hasCoffee && !hasPeach && !hasSalt);
        bool potentialPeach = (!hasMilk && !hasBoba && !hasCoffee && !hasSalt);
        bool potentialIcedCoffee = (!hasTea && !hasBoba && !hasPeach && !hasSalt);
        bool potentialSalted = (!hasTea && !hasBoba && !hasPeach);

        if (!potentialBoba && !potentialPeach && !potentialIcedCoffee && !potentialSalted) {
            state = DrinkState::RUINED;
        }
    }

    // Kiểm tra xem ly nước cốt đã sẵn sàng để gửi qua bàn trung chuyển chưa
    bool IsCorePrepared() const {
        if (type != ItemType::CUP || state == DrinkState::RUINED) return false;
        
        bool hasTea = HasIngredient(ItemType::TEA);
        bool hasMilk = HasIngredient(ItemType::MILK);
        bool hasBoba = HasIngredient(ItemType::BOBA);
        bool hasCoffee = HasIngredient(ItemType::COFFEE);
        bool hasPeach = HasIngredient(ItemType::PEACH);
        bool hasSalt = HasIngredient(ItemType::SALT_CREAM);

        if (state == DrinkState::SHAKEN) {
            if (hasTea && hasMilk && hasBoba && cupIngredients.size() == 3) return true; // Trà sữa
            if (hasTea && hasPeach && cupIngredients.size() == 2) return true;           // Trà đào
        } else if (state == DrinkState::BREWED) {
            if (hasCoffee && hasMilk && cupIngredients.size() == 2) return true;         // Cà phê sữa đá
            if (hasCoffee && hasMilk && hasSalt && cupIngredients.size() == 3) return true; // Cà phê muối
        }
        return false;
    }

    // Người 2 thêm Đá
    void AddIce() {
        if (type != ItemType::CUP || state == DrinkState::RUINED || finishedDrink != DrinkType::NONE) return;
        if (!IsCorePrepared()) {
            // Chưa xong cốt mà thêm đá là hỏng cốc!
            state = DrinkState::RUINED;
            return;
        }
        hasIce = true;
        CheckFinishingStatus();
    }

    // Người 2 thêm Topping đặc biệt
    void AddSpecialTopping() {
        if (type != ItemType::CUP || state == DrinkState::RUINED || finishedDrink != DrinkType::NONE) return;
        if (!IsCorePrepared()) {
            state = DrinkState::RUINED;
            return;
        }

        // Tự động nhận diện topping hợp lệ dựa trên cốt nước
        bool hasTea = HasIngredient(ItemType::TEA);
        bool hasBoba = HasIngredient(ItemType::BOBA);
        bool hasPeach = HasIngredient(ItemType::PEACH);
        bool hasSalt = HasIngredient(ItemType::SALT_CREAM);

        // Trà sữa trân châu cần Trân châu Hoàng Kim, Trà đào cần Lá bạc hà, Cafe muối cần bột Cacao.
        // Cà phê sữa đá thường không cần topping đặc biệt, nếu thêm topping là bị Ruined!
        if (hasTea && hasBoba) {
            hasSpecialTopping = true;
        } else if (hasTea && hasPeach) {
            hasSpecialTopping = true;
        } else if (hasSalt) {
            hasSpecialTopping = true;
        } else {
            state = DrinkState::RUINED; // Sai topping hỏng nước!
            return;
        }

        CheckFinishingStatus();
    }

    void CheckFinishingStatus() {
        if (state == DrinkState::RUINED) return;

        bool hasTea = HasIngredient(ItemType::TEA);
        bool hasMilk = HasIngredient(ItemType::MILK);
        bool hasBoba = HasIngredient(ItemType::BOBA);
        bool hasCoffee = HasIngredient(ItemType::COFFEE);
        bool hasPeach = HasIngredient(ItemType::PEACH);
        bool hasSalt = HasIngredient(ItemType::SALT_CREAM);

        // Khớp công thức hoàn chỉnh
        if (hasTea && hasMilk && hasBoba && state == DrinkState::SHAKEN) {
            if (hasIce && hasSpecialTopping) finishedDrink = DrinkType::BOBA_MILK_TEA;
        } else if (hasTea && hasPeach && state == DrinkState::SHAKEN) {
            if (hasIce && hasSpecialTopping) finishedDrink = DrinkType::PEACH_TEA;
        } else if (hasCoffee && hasMilk && state == DrinkState::BREWED) {
            // Cà phê sữa đá chỉ cần thêm đá là hoàn hảo!
            if (hasIce && !hasSpecialTopping) finishedDrink = DrinkType::ICED_MILK_COFFEE;
        } else if (hasCoffee && hasMilk && hasSalt && state == DrinkState::BREWED) {
            if (hasIce && hasSpecialTopping) finishedDrink = DrinkType::SALTED_COFFEE;
        }
    }
};

inline std::string GetDrinkName(DrinkType d) {
    switch (d) {
        case DrinkType::BOBA_MILK_TEA: return "Boba Milk Tea";
        case DrinkType::PEACH_TEA: return "Peach Iced Tea";
        case DrinkType::ICED_MILK_COFFEE: return "Iced Milk Coffee";
        case DrinkType::SALTED_COFFEE: return "Salted Cream Coffee";
        default: return "Beverage";
    }
}

// Recipe steps displayed on the HUD
inline std::vector<std::string> GetRecipeSteps(DrinkType d) {
    switch (d) {
        case DrinkType::BOBA_MILK_TEA: 
            return {"[Chef 1]: Cup+Tea+Milk+Boba -> Shake", "[Chef 2]: Add Ice+Golden Boba"};
        case DrinkType::PEACH_TEA: 
            return {"[Chef 1]: Cup+Tea+Peach -> Shake", "[Chef 2]: Add Ice+Mint Leaf"};
        case DrinkType::ICED_MILK_COFFEE: 
            return {"[Chef 1]: Cup+Coffee+Milk -> Brew", "[Chef 2]: Add Ice"};
        case DrinkType::SALTED_COFFEE: 
            return {"[Chef 1]: Cup+Coffee+Milk+SaltCream -> Brew", "[Chef 2]: Add Ice+Cocoa Powder"};
        default: return {};
    }
}

#endif // RECIPE_H
