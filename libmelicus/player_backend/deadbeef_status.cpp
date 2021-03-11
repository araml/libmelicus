#include <task.h>

namespace melicus { 
    status deadbeef_status() { 
        task([](const std::string &) -> void { return; }, "hello");
        return status {};
    }
}
