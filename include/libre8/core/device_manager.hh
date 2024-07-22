#pragma once

#include <thread>
#include <atomic>
#include <set>
#include <condition_variable>
#include <filesystem>
#include <memory>

namespace libre8::core {
    struct device_identifier final {
        const std::filesystem::path device_path;
        const uint16_t vendor_id;
        const uint16_t product_id;
        const uint8_t usage;
        const uint8_t usage_page;

        auto operator==(const device_identifier& another) const noexcept -> bool {
            return (
                this->vendor_id == another.vendor_id &&
                this->product_id == another.product_id &&
                this->usage == another.usage &&
                this->usage_page == another.usage_page
            );
        }
    };

    class device_manager final {
        private:
            std::jthread m_device_scanner {};
            std::atomic_bool m_device_scanner_running {};
            std::set<std::pair<uint16_t, uint16_t>> m_last_seen_devices {};

        public:
            explicit device_manager();
            ~device_manager();

            auto start_scanning() -> void;
            auto stop_scanning() -> void;

        private:
            auto scanner_thread_loop() -> void;
    };
}
