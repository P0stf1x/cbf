#pragma once

enum class BFOperationType {
    POINTER_INCREMENT = 0,
    POINTER_DECREMENT = 1,
    DATA_INCREMENT = 2,
    DATA_DECREMENT = 3,
    OUTPUT = 4,
    INPUT = 5,
};

struct BFOperation {
    enum BFOperationType type;
    size_t count;
    char c;
    //   ^ temp for now, just to make printing block structure easier
    // TODO: remove after implementing actual BF runner
};
