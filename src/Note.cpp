#include "Note.hpp"

#include <cmath>
#include <stdexcept>
#include <string>

namespace {
    constexpr const char* kNoteNames[12] = {
        "C", "C#", "D", "D#", "E", "F",
        "F#", "G", "G#", "A", "A#", "B"
    };
}

Note::Note(int midi) : midi_(midi) {
    if (midi < 0 || midi > 127) {
        throw std::out_of_range("MIDI note out of range (0-127): " + std::to_string(midi));
    }
}

int Note::midi() const {
    return midi_;
}

double Note::frequency() const {
    return 440.0 * std::pow(2.0, (midi_ - 69) / 12.0);
}

std::string Note::name() const {
    int octave = midi_ / 12 - 1;
    return std::string(kNoteNames[midi_ % 12]) + std::to_string(octave);
}