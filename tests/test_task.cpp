#include <task.h>
#include <cassert>
#include <iostream>

auto foo() -> int { 
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
            //std::cout << s << std::endl;
            sum += static_cast<size_t>(s);
            i++;
        }
    }
    

    return sum;
}

int main() {
    for (size_t i = 0; i < 100000; i++) { 
        std::cout << i << std::endl;
        assert(foo() == 5050);
    }

    return 0;
}
