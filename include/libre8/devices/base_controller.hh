#pragma once

#define abstract = 0

namespace libre8::devices {
    // i'll get here
    // eventually

    class controller {
        auto get_filenames();
        auto set_filenames() -> void;

        auto get_rumble_data();
        auto set_rumble_data() -> void;

        auto get_analog_deadzones();
        auto set_analog_deadzones() -> void;

        auto get_trigger_configuration();
        auto set_trigger_configuration() -> void;

        auto get_button_mappings(unsigned profile_slot);
        auto set_button_mappings(unsigned profile_slot) -> void;
    };
}
