struct VArg {
    int type;
    std::string name;
};

struct Arg {
    int type;
    union {
        char c;
        App app;
    } u;
};

struct App {
    VState *name;
    std::vector<Arg> args;
};

struct Branch {
    vector<char> ind;
    App app;
};

class VState {
private:
    const std::string name;
    std::vector<VArg> vargs;
    std::vector<Branch> branches;
    App deflt;
};
