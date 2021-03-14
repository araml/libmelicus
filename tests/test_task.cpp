#include <task.h>
#include <cassert>
#include <iostream>

int main() {
    int ctr = 0;
    task<int> t([&ctr]() -> int {
        ctr = ctr + 1;
        return ctr;
    });

    size_t sum = 0;
    int i = 0;
    while (i < 100) { 
        if (!t.is_running()) {
            t.query();
            int s = t.get();
            std::cout << s << std::endl;
            sum += static_cast<size_t>(s);
            i++;
        }
    }
        
    std::cout << sum << std::endl;

    return 0;
}
