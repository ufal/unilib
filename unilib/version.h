// This file is part of UniLib <http://github.com/ufal/unilib/>.
//
// Copyright 2014-2023 Institute of Formal and Applied Linguistics, Faculty
// of Mathematics and Physics, Charles University in Prague, Czech Republic.
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// UniLib version: 3.3.2-dev
// Unicode version: 15.1.0

#pragma once

#include <string>

namespace ufal {
namespace unilib {

// The `version` class represents the UniLib version according to
// the [Semantic Versioning](http://semver.org/).
class version {
 public:
  unsigned major;
  unsigned minor;
  unsigned patch;
  std::string prerelease;

  // Returns the current version.
  static version current();
};

} // namespace unilib
} // namespace ufal
