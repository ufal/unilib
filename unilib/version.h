// This file is part of UniLib <http://github.com/ufal/unilib/>.
//
// Copyright 2014-2024 Institute of Formal and Applied Linguistics, Faculty
// of Mathematics and Physics, Charles University in Prague, Czech Republic.
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// UniLib version: 4.1.2-dev
// Unicode version: 16.0.0

#pragma once

#include <string>

namespace unilib {

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

//
// Definitions
//

std::string version::to_string() {
  return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch) +
      (!prerelease.empty() ? "-" + prerelease : "");
}

version version::current() {
  return {4, 1, 2, "dev"};
}

} // namespace unilib
