#include <iostream>

class Fighter {
private:
    const char* name;
    int health;
    int maxHealth;
    int damage;

public:
    Fighter(const char* name, int health, int damage)
        : name(name), health(health), maxHealth(health), damage(damage) {}

    void TakeDamage(int amount) {
        health -= amount;
        if (health < 0)
            health = 0;
    }

    bool IsAlive() const {
        return health > 0;
    }

    void Attack(Fighter& target) const {
        target.TakeDamage(damage);
    }

    void PrintStatus() const {
        std::cout << name << ": " << health << " хп\n";
    }
};

void PrintFighter(const Fighter& fighter) {
    fighter.PrintStatus();
}

class Game {
private:
    Fighter player;
    Fighter enemies[3];
    Fighter* currentTarget = nullptr;

    bool TrySelectTarget(int number, Fighter*& outTarget) {
        outTarget = nullptr;

        if (number < 1 || number > 3)
            return false;

        if (!enemies[number - 1].IsAlive())
            return false;

        outTarget = &enemies[number - 1];
        return true;
    }

    bool AllEnemiesDefeated() const {
        for (int i = 0; i < 3; i++) {
            if (enemies[i].IsAlive())
                return false;
        }

        return true;
    }

public:
    Game()
        : player("Hero", 30, 8),
          enemies{
              {"Enemy 1", 12, 3},
              {"Enemy 2", 18, 4},
              {"Enemy 3", 24, 5}
          } {}

    void Run() {
        while (true) {
            PrintFighter(player);

            for (int i = 0; i < 3; i++)
                PrintFighter(enemies[i]);

            int command;

            std::cout << "1 - выбрать цель\n";
            std::cout << "2 - атака\n";
            std::cout << "0 - выход\n";

            std::cin >> command;

            if (command == 0)
                break;

            if (command == 1) {
                int number;

                std::cout << "выбери врага 1-3: ";
                std::cin >> number;

                if (!TrySelectTarget(number, currentTarget))
                    std::cout << "неверная цель\n";
            }

            if (command == 2) {
                if (currentTarget == nullptr) {
                    std::cout << "цель не выбрана\n";
                    continue;
                }

                player.Attack(*currentTarget);

                if (!currentTarget->IsAlive()) {
                    currentTarget = nullptr;
                }
                else {
                    currentTarget->Attack(player);
                }

                if (!player.IsAlive()) {
                    std::cout << "ты проиграл!\n";
                    break;
                }

                if (AllEnemiesDefeated()) {
                    std::cout << "ты победил!\n";
                    break;
                }
            }
        }
    }
};

int main() {
    Game game;
    game.Run();
}