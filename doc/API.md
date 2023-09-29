# UniLib 3.3.2-dev

The UniLib library is contained in `ufal::unilib` namespace.
It consists of several independent files, each containing a class with
the same name. All methods are thread-safe, unless stated otherwise.

The `char32_t` type represents Unicode code points and `u32string` type
represents sequences of Unicode code points. For UTF-8, the `char` and
`string` types represent code units and sequences of code units; for UTF-16,
the `char16_t` and `u16string` types are used to represent code units and
sequence of code units.

## Class `unicode`

```cpp
// The `unicode` class contains basic information from
// [Unicode Character Database](http://www.unicode.org/reports/tr44/), notably
// - general categories
// - simple and full uppercase/lowercase/titlecase mappings
class unicode {
 public:
  // The `category_t` type represents general category from
  // [Unicode Character Database](http://www.unicode.org/reports/tr44/).
  // Its values are:
  // - abbreviated property value aliases for general categories,
  // - abbreviated aliases for groupings of related general categories.
  //
  // Note that the `category_t` is a bitmask indicating general categories,
  // which makes it easy to define arbitrary subsets of general categories
  // similarly to how groupings of general categories are defined.
  typedef uint32_t category_t;
  enum : category_t {
    Lu, Ll, Lt, Lut = Lu | Lt, LC = Lu | Ll | Lt, Lm, Lo, L = Lu | Ll | Lt | Lm | Lo,
    Mn, Mc, Me, M = Mn | Mc | Me,
    Nd, Nl, No, N = Nd | Nl | No,
    Pc, Pd, Ps, Pe, Pi, Pf, Po, P = Pc | Pd | Ps | Pe | Pi | Pf | Po,
    Sm, Sc, Sk, So, S = Sm | Sc | Sk | So,
    Zs, Zl, Zp, Z = Zs | Zl | Zp,
    Cc, Cf, Cs, Co, Cn, C = Cc | Cf | Cs | Co | Cn
  };

  // Returns general category of the given Unicode code point as a value of
  // `category_t` enumeration. The `Cn` category is returned for code points
  // outside of valid range.
  static inline category_t category(char32_t chr);

  // Returns the simple lowercase/uppercase/titlecase mapping of the given code
  // point from [Unicode Character Database](http://www.unicode.org/reports/tr44/).
  // If no such mapping is defined or the given code point is outside of valid
  // range, the original code point is returned.
  static inline char32_t lowercase(char32_t chr);
  static inline char32_t uppercase(char32_t chr);
  static inline char32_t titlecase(char32_t chr);
};
```

## Class `uninorms`

```cpp
// The `uninorms` class performs normalization to [Unicode Normalization
// Forms](http://unicode.org/reports/tr15/). All normalization forms are
// supported and the normalization is performed in-place on a `u32string`
// representing a string of Unicode code points.
class uninorms {
 public:
  // Convert given string to [Normalization Form C](http://unicode.org/reports/tr15/#Norm_Forms).
  static void nfc(std::u32string& str);

  // Convert given string to [Normalization Form D](http://unicode.org/reports/tr15/#Norm_Forms).
  static void nfd(std::u32string& str);

  // Convert given string to [Normalization Form KC](http://unicode.org/reports/tr15/#Norm_Forms).
  static void nfkc(std::u32string& str);

  // Convert given string to [Normalization Form KD](http://unicode.org/reports/tr15/#Norm_Forms).
  static void nfkd(std::u32string& str);
};
```

## Class `unistrip`

```cpp
// The `unistrip` class performs combining marks stripping. The same
// functionality can be achieved by combining `unicode` and `uninorms`
// classes, but `unistrip` class performs the task much faster and
// needs less embedded data.
class unistrip {
 public:
  // Returns `true` if the category of the given Unicode code point is `M`
  // (i.e., nonspacing mark `Mn` or spacing mark `Mc` or enclosing `Me`).
  static inline bool is_combining_mark(char32_t chr);

  // Returns Unicode code point which has all combining marks removed. If the
  // given code point is a combining mark itself, or there are no combining
  // marks presents, `strip_combining_marks` returns the original code point.
  //
  // This method is equivalent to the following process:
  // - If the given code point is outside the valid Unicode range, return it.
  // - If the given code points has general category `M`, return it.
  // - Othervise, convert it to NFD.
  // - If there are no code points with general category `M`, return
  //   the original code point.
  // - Otherwise, remove all code points with general cagetory `M`.
  // - Convert the result to NFC.
  // - Return the resulting code point.
  static inline char32_t strip_combining_marks(char32_t chr);
};
```

## Class `version`

```cpp
// The `version` class represents the UniLib version according to
// the [Semantic Versioning](http://semver.org/).
class version {
 public:
  unsigned major;
  unsigned minor;
  unsigned patch;
  std::string prerelease;

  // Returns the version as a single string.
  inline std::string to_string();

  // Returns the current version.
  inline static version current();
};
```
