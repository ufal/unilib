Version 4.2.1-dev
-----------------

Version 4.2.0 [10 Sep 2025]
---------------------------
- Update Unicode data to 17.0.0.


Version 4.1.2 [27 Mar 2025]
---------------------------
- Add forgotten `inline` specifier in the `utf::encode` methods.


Version 4.1.1 [4 Nov 2024]
--------------------------
- Silence new Clang warning by not using comma operator in returns.


Version 4.1.0 [27 Sep 2024]
---------------------------
- Update Unicode data to 16.0.0.


Version 4.0.0 [29 Sep 2023]
----------------------------
- Increase the minimum requirements to C++ 17 [incompatible change].
- Change the namespace from `ufal::unilib` to `unilib` [incompatible change].
- Start using `std::string_view` in the API [incompatible change].
- Merge `utf8` and `utf16` classes to just `utf` [incompatible change].
- Add also full casing mappings (where the casing of a single codepoint
  is longer than 1 codepoint).
- Replace the system generating the API documentation.
- Update Unicode data to 15.1.0.


Version 3.3.1 [12 Sep 2023]
---------------------------
- Stop using the `std::iterator` deprecated in C++17.


Version 3.3.0 [13 Sep 2022]
---------------------------
- Update Unicode data to 15.0.0.


Version 3.2.0 [03 Mar 2020]
---------------------------
- Update Unicode to 13.0.0.
- Update tests to work also under Windows.
  We now use C++ Builtem http://github.com/ufal/cpp_builtem
  as build system and require Python3 to run the tests.


Version 3.1.1 [15 Jun 2016]
---------------------------
- Fix warnings of G++ 6.


Version 3.1.0 [29 Jun 2015]
---------------------------
- Update Unicode to 8.0.0.


Version 3.0.1 [06 Mar 2015]
---------------------------
- Move `version::current` from header to object file.


Version 3.0.0 [26 Feb 2015]
---------------------------
- Start using Semantic Versioning http://semver.org/.
- Use t2t_docsys http://github.com/ufal/t2t_docsys as documentation system.
- Renamed the `utf{8,16}::{string,buffer}_decoder_helper` helper classes to
  `utf{8,16}::{string,buffer}_decoder`.
- Add version structure.
- Fix compilation on Visual C++ 2013.


Version 2.1 [22 Dec 2014]
-------------------------
- Changed license to MPL 2.0.


Version 2.0 [22 Jul 2014]
-------------------------
- Updated Unicode to 7.0.0.
- Changed license to 3-clause BSD.
- Added ustrip with combining mark stripping.


Version 1.0 [12 May 2014]
-------------------------
- Initial public release.
- Unicode 6.3.
