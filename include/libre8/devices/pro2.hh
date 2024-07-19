#pragma once

#include <libre8/util/fixed_endian.hh>

#include <vector>

namespace libre8::devices {
    class pro2 final {
        /*
         * most of this is referenced from here:
         * https://github.com/TheJayMann/8bitdo-spec/tree/master/Pro2
         *
         * a massive thank you for your efforts, since i lack rather basic re skills
         */

        using uint16_le = util::fixed_endian<uint16_t, util::byte_order::little>;
        using uint32_le = util::fixed_endian<uint32_t, util::byte_order::little>;

        struct [[gnu::packed]] output_packet {
            constexpr static size_t header_size =
                sizeof(uint8_t) * 3;

            constexpr static size_t request_size =
                (sizeof(uint16_t) * 4) + (sizeof(uint32_t) * 2);

            constexpr static size_t max_data_size =
                64z - header_size - request_size;

            /* header */
            const uint8_t magic { 0x81 };
            uint8_t data_size {};
            uint8_t operation {}; // 0x04 is configuration data.... what about the others?

            /* request data - everything is little endian from this point and on */
            uint16_t request_type {}; // specified by caller
            uint16_t subrequest_type {}; // specified by caller
            uint16_t sequence_size {}; // how much are we reading / writing on
                                       // the next packets
            uint16_t checksum {}; // crc16/modbus, all lights on if no data is
                                  // present. always 0 for devices using older
                                  // revisions of the pro2 protocol.
                                  //
                                  // where older revisions of the pro2 protocol
                                  // might be for pids other than 0x3010, 0x3011,
                                  // 0x3019, 0x6006 and 0x6007.
            uint32_t overall_data_size {}; // both are for data chunking.
            uint32_t data_offset {};

            /* subsequent data */
            uint8_t data[max_data_size] {};
        };

        using input_data_type = std::span<uint8_t>;
        using raw_data_type = std::array<uint8_t, sizeof(output_packet)>;
        using chunked_req_type = std::vector<raw_data_type>;

        private:
            auto prep_dataless(
                const uint8_t operation,
                const uint16_le request_type,
                const uint16_le subrequest_type,
                const uint16_le expected_sequence_size
            ) -> const output_packet;

        public:
            pro2();
            ~pro2();

            pro2(const pro2& other) = delete;
            pro2(const pro2&& other) = delete;

            auto prep_request(
                const uint8_t operation,
                const uint16_le request_type,
                const uint16_le subrequest_type,
                const uint16_le expected_sequence_size,
                const input_data_type& data
            ) -> const chunked_req_type;
    };
}
