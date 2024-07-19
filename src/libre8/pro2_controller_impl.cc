#include <libre8/util/util.hh>
#include <libre8/util/fixed_endian.hh>
#include <libre8/devices/pro2.hh>

#include <ranges>
#include <print>

namespace libre8::devices {
    pro2::pro2() {

    }

    pro2::~pro2() {

    }

    auto pro2::prep_dataless(
        const uint8_t operation,
        const uint16_le request_type,
        const uint16_le subrequest_type,
        const uint16_le expected_sequence_size
    ) -> const pro2::output_packet {
        uint8_t packet_size = output_packet::header_size;
        if (request_type > 0)
            packet_size += sizeof(uint16_t);

        if (subrequest_type > 0)
            packet_size += sizeof(uint16_t);

        // data size should always contain a value that is less than 63
        const pro2::output_packet out {
            // TODO: different empty requests specify different data sizes. figure out why.
            .data_size = packet_size,
            .operation = operation,
            .request_type = request_type.as_stored(),
            .subrequest_type = subrequest_type.as_stored(),
            .sequence_size = expected_sequence_size.as_stored(),
            // TODO: some empty requests have checksum 0x0000 regardless. figure out why.
            .checksum = 0xFFFF
        };

        return out;
    }

    auto pro2::prep_request(
        const uint8_t operation,
        const uint16_le request_type,
        const uint16_le subrequest_type,
        const uint16_le expected_sequence_size,
        const pro2::input_data_type& data
    ) -> const pro2::chunked_req_type {
        if (data.size() == 0) {
            const auto single_out = prep_dataless(operation, request_type, subrequest_type, expected_sequence_size);
            return { util::serialize(&single_out) };
        }

        const auto overall_data_size = pro2::uint32_le { static_cast<uint32_t>(data.size()) };
        const auto data_chunks = data
            | std::views::chunk(output_packet::max_data_size);

        chunked_req_type ret {};
        uint32_le data_ofs {};
        for (const auto& chunk : data_chunks) {
            const pro2::output_packet next_packet {
                .data_size = static_cast<uint8_t>(output_packet::header_size + output_packet::request_size + chunk.size() - 1),
                .operation = operation,
                .request_type = request_type.as_stored(),
                .subrequest_type = subrequest_type.as_stored(),
                .sequence_size = expected_sequence_size.as_stored(),
                .checksum = 0, /* TODO */
                .overall_data_size = overall_data_size.as_stored(),
                .data_offset = data_ofs.as_stored()
            };

            ret.push_back(util::serialize(&next_packet));

            data_ofs = data_ofs + static_cast<uint32_t>(chunk.size());
            std::println("size: {}; ofs: {}", chunk.size(), data_ofs.as_host());
        }

        return ret;
    }
}
