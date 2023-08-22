#include "Registry.h"
#include <chrono>

struct Transform {
    int x;
    int y;
    int a;
    int b;
};

int main() {
    Registry registry(20000);
    std::vector<Entity> entities;

    for (int i = 0; i < 20000; ++i) {
        entities.emplace_back(registry.createEntity());
    }

    for (int i = 5000; i < 10000; ++i) {
        registry.addComponent<Transform>(entities[i], 10, 20);
    }

    auto begin = std::chrono::steady_clock::now();
    auto transforms = registry.getView<Transform>();

    for (Index i = 0; i < transforms.size(); ++i) {
        transforms[i].a = transforms[i].b;
    }
    auto end = std::chrono::steady_clock::now();
    auto duration = (end - begin).count();

    std::cout << "Duration = " << duration << '\n';

    begin = std::chrono::steady_clock::now();

    for (Entity& e : entities) {
        if (registry.hasComponent<Transform>(e)) {
            auto& transform = registry.getComponent<Transform>(e);
        }
    }

    end = std::chrono::steady_clock::now();
    duration = (end - begin).count();
    std::cout << "Duration = " << duration << '\n';
}
