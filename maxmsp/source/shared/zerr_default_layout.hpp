/**
 * @file    zerr_default_layout.hpp
 * @brief   The speaker layout a plain [zerr.envelopes~] uses when no file is given
 *
 * @copyright  Copyright (c) 2023-2026 Zeyu Yang
 * @license    MIT license
 */
#pragma once

namespace zerr_max {

/// Label shown in messages for the built-in layout (it is not a file on disk).
constexpr const char* kDefaultLayoutName = "built-in ring";

/// Eight loudspeakers in a regular ring, the same positions as configs/ring_8.yaml (shipped in
/// the package's misc/ folder), as YAML text so the object works without any file on Max's
/// search path. Keep the two in step.
constexpr const char* kDefaultLayoutYaml = R"(standard:
  1: { position: { spherical: { azimuth:   0.0, elevation: 0.0, distance: 1.0 } } }
  2: { position: { spherical: { azimuth:  45.0, elevation: 0.0, distance: 1.0 } } }
  3: { position: { spherical: { azimuth:  90.0, elevation: 0.0, distance: 1.0 } } }
  4: { position: { spherical: { azimuth: 135.0, elevation: 0.0, distance: 1.0 } } }
  5: { position: { spherical: { azimuth: 180.0, elevation: 0.0, distance: 1.0 } } }
  6: { position: { spherical: { azimuth: 225.0, elevation: 0.0, distance: 1.0 } } }
  7: { position: { spherical: { azimuth: 270.0, elevation: 0.0, distance: 1.0 } } }
  8: { position: { spherical: { azimuth: 315.0, elevation: 0.0, distance: 1.0 } } }
)";

} // namespace zerr_max
