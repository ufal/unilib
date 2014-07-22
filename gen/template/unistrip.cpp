#include "unistrip.h"

namespace ufal {
namespace unilib {

const char32_t unistrip::CHARS;

const uint8_t unistrip::combining_mark_index[unistrip::CHARS >> 8] = $COMBINING_MARK_INDICES;

const uint8_t unistrip::combining_mark_block[][32] = $COMBINING_MARK_BLOCKS;

const uint8_t unistrip::stripped_index[unistrip::CHARS >> 8] = $STRIPPED_INDICES;

const uint16_t unistrip::stripped_block[][256] = $STRIPPED_BLOCKS;

const char32_t unistrip::stripped_data[] = $STRIPPED_DATA;

} // namespace unilib
} // namespace ufal
