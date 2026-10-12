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
constexpr const char* kDefaultLayoutName = "built-in ring_8";

/// Eight loudspeakers in a ring, the same positions as the help patches' circulation_8.yaml,
/// as YAML text so the object works without any file on Max's search path.
constexpr const char* kDefaultLayoutYaml = R"(standard:
  1: { position: { cartesian: { x:  3.265, y: -1.065, z: 0.410 } } }
  2: { position: { cartesian: { x:  0.995, y: -2.268, z: 0.410 } } }
  3: { position: { cartesian: { x: -1.269, y: -2.318, z: 0.410 } } }
  4: { position: { cartesian: { x: -3.124, y: -1.250, z: 0.410 } } }
  5: { position: { cartesian: { x: -3.249, y:  1.433, z: 0.410 } } }
  6: { position: { cartesian: { x: -1.553, y:  2.814, z: 0.410 } } }
  7: { position: { cartesian: { x:  1.254, y:  2.946, z: 0.410 } } }
  8: { position: { cartesian: { x:  3.145, y:  1.474, z: 0.410 } } }
)";

} // namespace zerr_max
