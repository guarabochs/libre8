#pragma once

#include <memory>
#include <optional>
#include <string>
#include <algorithm>

namespace libre8::util {
    inline auto wstring_to_string(const wchar_t* field) -> const std::optional<const std::string> {
        if (field == nullptr)
            return std::nullopt;

        const auto cur_locale = std::setlocale(LC_ALL, nullptr);
        std::setlocale(LC_ALL, "C");

        const auto c_actual_len =
            std::wcstombs(nullptr, field, 0);

        if (c_actual_len < 1) {
            std::setlocale(LC_ALL, cur_locale);
            return std::nullopt;
        }

        // no
        if (c_actual_len > 512) {
            std::setlocale(LC_ALL, cur_locale);
            return std::nullopt;
        }

        std::unique_ptr<char[]> c_device_name { new char[c_actual_len] };
        std::wcstombs(c_device_name.get(), field, c_actual_len);

        std::setlocale(LC_ALL, cur_locale);
        return std::string { c_device_name.get(), c_actual_len };
    }

    template<typename T>
    concept standard_layout_type = requires {
        std::is_standard_layout_v<T> == true;
    };

    inline auto serialize(const standard_layout_type auto* input) -> const std::array<uint8_t, sizeof(decltype(*input))> {
        constexpr size_t output_size = sizeof(*input);

        const auto char_ptr = reinterpret_cast<const uint8_t*>(input);
        std::array<uint8_t, output_size> ret {};

        std::copy_n(char_ptr, output_size, ret.begin());
        return ret;
    }
}
