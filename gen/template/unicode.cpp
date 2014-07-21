#include "unicode.h"

namespace ufal {
namespace unilib {

const char32_t unicode::CHARS;

const int32_t unicode::DEFAULT_CAT;

const uint8_t unicode::category_index[unicode::CHARS >> 8] = $CATEGORY_INDICES;

const uint8_t unicode::category_block[][256] = $CATEGORY_BLOCKS;

const uint8_t unicode::othercase_index[unicode::CHARS >> 8] = $OTHERCASE_INDICES;

const char32_t unicode::othercase_block[][256] = $OTHERCASE_BLOCKS;

} // namespace unilib
} // namespace ufal
