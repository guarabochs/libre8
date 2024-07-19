#pragma once

#include <concepts>
#include <cstdint>
#include <endian.h>
#include <print>
#include <utility>

namespace libre8::util {
    enum class byte_order {
        little,
        big
    };

    template<std::unsigned_integral UnsignedNumericType, const byte_order ByteOrder = byte_order::little>
    class fixed_endian final {
        private:
            UnsignedNumericType held_value {};

        public:
            inline fixed_endian() : held_value() {}
            inline fixed_endian(const UnsignedNumericType&& value) {
                if constexpr (std::same_as<UnsignedNumericType, uint16_t> == true) {
                    switch (ByteOrder) {
                        case byte_order::little:
                            held_value = htole16(value);
                            break;
                        case byte_order::big:
                            held_value = htobe16(value);
                            break;
                        default:
                            std::unreachable();
                    }
                } else if constexpr (std::same_as<UnsignedNumericType, uint32_t> == true) {
                    switch (ByteOrder) {
                        case byte_order::little:
                            held_value = htole32(value);
                            break;
                        case byte_order::big:
                            held_value = htobe32(value);
                            break;
                        default:
                            std::unreachable();
                    }
                } else if constexpr (std::same_as<UnsignedNumericType, uint64_t> == true) {
                    switch (ByteOrder) {
                        case byte_order::little:
                            held_value = htole64(value);
                            break;
                        case byte_order::big:
                            held_value = htobe64(value);
                            break;
                        default:
                            std::unreachable();
                    }
                }
            }

            inline operator UnsignedNumericType() const {
                return as_host();
            }

            inline auto as_stored() const -> const UnsignedNumericType {
                return held_value;
            }

            inline auto as_host() const -> const UnsignedNumericType {
                if constexpr (std::same_as<UnsignedNumericType, uint16_t> == true) {
                    switch (ByteOrder) {
                        case byte_order::little:
                            return le16toh(held_value);
                        case byte_order::big:
                            return be16toh(held_value);
                        default:
                            std::unreachable();
                    }
                } else if constexpr (std::same_as<UnsignedNumericType, uint32_t> == true) {
                    switch (ByteOrder) {
                        case byte_order::little:
                            return le32toh(held_value);
                        case byte_order::big:
                            return be32toh(held_value);
                        default:
                            std::unreachable();
                    }
                } else if constexpr (std::same_as<UnsignedNumericType, uint64_t> == true) {
                    switch (ByteOrder) {
                        case byte_order::little:
                            return le64toh(held_value);
                        case byte_order::big:
                            return be64toh(held_value);
                        default:
                            std::unreachable();
                    }
                }

                std::unreachable();
            }
    };
}
