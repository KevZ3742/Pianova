#pragma once
#include <string>

class Note {
public:
    explicit Note(int midi);

    int midi() const;
    double frequency() const;
    std::string name() const;

private:
    int midi_;
};