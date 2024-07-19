#include <libre8/util/util.hh>
#include <libre8/core/device_manager.hh>

#include <print>
#include <functional>
#include <ranges>
#include <algorithm>

#include <hidapi.h>

namespace libre8::core {
    device_manager::device_manager() {
        hid_init();
    }

    device_manager::~device_manager() {
        stop_scanning();
        hid_exit();
    }

    auto device_manager::start_scanning() -> void {
        m_device_scanner_running = true;
        m_device_scanner = std::jthread { [this] { scanner_thread_loop(); } };
    }

    auto device_manager::stop_scanning() -> void {
        m_device_scanner_running = false;
        std::println("[device_manager] goodbye");
    }

    auto device_manager::scanner_thread_loop() -> void {
        using namespace std::literals::chrono_literals;

        bool is_first_run { true };
        while (m_device_scanner_running) {
            auto hid_devices = hid_enumerate(0, 0);
            auto cur_hid_device = hid_devices;

            while (cur_hid_device != nullptr) {
                if constexpr (false) {
                    std::println("hid device path {}; mf str {}; p str {}; s/n {}; vid {:#06x}; pid {:#06x}; usage: {}; usage page: {}",
                        cur_hid_device->path,
                        libre8::util::wstring_to_string(cur_hid_device->manufacturer_string).value_or("?"),
                        libre8::util::wstring_to_string(cur_hid_device->product_string).value_or("?"),
                        libre8::util::wstring_to_string(cur_hid_device->serial_number).value_or("?"),
                        cur_hid_device->vendor_id,
                        cur_hid_device->product_id,
                        cur_hid_device->usage,
                        cur_hid_device->usage_page
                    );
                }

                cur_hid_device = cur_hid_device->next;
            }

            hid_free_enumeration(hid_devices);

            std::this_thread::sleep_for(2s);
            is_first_run = false;
        }
    }
}
