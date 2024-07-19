#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace libre8::products {
    static constexpr uint16_t general_vendor_id = 0x2DC8;

    enum class product_type {
        pro_2, pro_2_wired, arcade_stick, sn30
    };

    struct product {
        const product_type type;
        const std::string name;
        const std::vector<uint16_t> product_ids;
    };

    static const std::vector<product> all_products {
        product { .type = product_type::pro_2, .name = "Pro 2", .product_ids = { 0x6006, 0x6003 } },
        // i dont have these
        product { .type = product_type::pro_2_wired, .name = "Pro 2 Wired", .product_ids = { 0x3010 } },
        product { .type = product_type::arcade_stick, .name = "Arcade Stick", .product_ids = { 0x901A } },
        product { .type = product_type::sn30, .name = "SN30 Pro / SN30 Plus", .product_ids = { 0x6002 } }
    };
}
