#include <SFML/Graphics.hpp>

#include <atomic>
#include <chrono>
#include <cmath>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <thread>

#include "miniaudio.h"
#include "Note.hpp"

namespace {
    // Data shared between the main thread and the audio thread
    struct ToneState {
        std::atomic<double> freq{440.0};
        double phase = 0.0;
        double sampleRate = 48000.0;
    };

    // Runs on miniaudio's audio thread. Fills `output` with samples.
    void audioCallback(ma_device* device, void* output, const void*, ma_uint32 frameCount) {
        auto* state = static_cast<ToneState*>(device->pUserData);
        auto* out = static_cast<float*>(output);

        const double twoPi = 6.283185307179586;
        const double inc = twoPi * state->freq.load() / state->sampleRate;

        for (ma_uint32 i = 0; i < frameCount; ++i) {
            out[i] = 0.2f * static_cast<float>(std::sin(state->phase));
            state->phase += inc;
            if (state->phase >= twoPi) state->phase -= twoPi;
        }
    }
}

int main() {
    // --- Note class test ---
    for (int m : {0, 60, 61, 69, 127}) {
        Note n(m);
        std::cout << m << " -> " << n.name() << " " << n.frequency() << " Hz\n";
    }

    try {
        Note bad(200);
    } catch (const std::out_of_range& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }

    // --- Audio test: play A4 for 2 seconds ---
    ToneState tone;
    tone.freq = Note(69).frequency();

    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = ma_format_f32;
    config.playback.channels = 1;
    config.sampleRate = 48000;
    config.dataCallback = audioCallback;
    config.pUserData = &tone;

    ma_device device;
    if (ma_device_init(nullptr, &config, &device) != MA_SUCCESS) {
        std::cerr << "Failed to open audio device\n";
        return 1;
    }
    ma_device_start(&device);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    ma_device_uninit(&device);

    // --- Window ---
    sf::RenderWindow window(sf::VideoMode({900, 400}), "Pianova");
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                std::cout << "Key pressed: " << static_cast<int>(key->code) << "\n";
            }
        }

        window.clear(sf::Color(30, 30, 40));
        window.display();
    }
    return 0;
}