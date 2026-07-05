#include <cstdint>

enum class Side : std::uint8_t
{
    Buy,
    Sell
};

struct BenchOrder
{
    std::uint64_t orderId;
    std::uint64_t timestampNs;

    std::int32_t priceTicks;
    std::int32_t quantity;

    Side side;
    std::uint8_t flags;

    // Padding-ish fields so it is not just a tiny int benchmark
    std::uint16_t venueId;
    std::uint32_t symbolId;

    BenchOrder(
        std::uint64_t id,
        std::uint64_t timestamp,
        std::int32_t price,
        std::int32_t qty,
        Side orderSide,
        std::uint32_t symbol
    )
        : orderId(id),
          timestampNs(timestamp),
          priceTicks(price),
          quantity(qty),
          side(orderSide),
          flags(0),
          venueId(1),
          symbolId(symbol)
    {}
};