#include <app/app.hh>
#include <app/util.hh>

#include <print>

#include <libre8/core/device_manager.hh>
#include <libre8/util/fixed_endian.hh>
#include <libre8/util/util.hh>
#include <libre8/devices/pro2.hh>

namespace app {
    auto stuff() {
        libre8::devices::pro2 pro2;
        std::array<uint8_t, 16> data = {
        };

        const auto packets = pro2.prep_request(0, 0x0121, 0, data);

        std::println("=== hid out ===");
        for (const auto& packet : packets)  {
            for (const auto& byte : packet)
                std::print("{:#04x} ", static_cast<uint8_t>(byte));
        }
    }

    app::app(const program_arguments&& args) : launch_arguments { std::move(args) } {
        // program constructor
    }

    auto app::run() -> void {
        stuff();
        return;
    }
}
