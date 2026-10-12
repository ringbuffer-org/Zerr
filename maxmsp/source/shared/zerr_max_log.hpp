/**
 * @file    zerr_max_log.hpp
 * @brief   Routes the core library's Logger output to the Max Console
 *
 * @copyright  Copyright (c) 2023-2026 Zeyu Yang
 * @license    MIT license
 */
#pragma once

#include <string>

#include "ext.h"
#include "ext_obex.h"

#include "logger.h"

namespace zerr_max {

/**
 * @brief A Logger::PrintStrategy that posts to the Max Console on behalf of an object
 * @param owner The Max object the messages belong to (they are attributed to it and
 *              a click on the error line selects it)
 *
 * The core formats every line as "[LEVEL] message". The prefix is mapped to the Max
 * call of matching severity and stripped, and the body is always passed as a "%s"
 * argument: the core's messages can carry text that came from the user (speaker
 * layout names, action words), so they must never be used as a format string.
 */
inline zerr::Logger::PrintStrategy consolePrinter(t_object* owner)
{
    return [owner](const std::string& line) {
        std::string level, body = line;
        if (!line.empty() && line.front() == '[') {
            const auto close = line.find("] ");
            if (close != std::string::npos) {
                level = line.substr(1, close - 1);
                body  = line.substr(close + 2);
            }
        }
        if (level == "ERROR") {
            object_error(owner, "%s", body.c_str());
        }
        else if (level == "WARNING") {
            object_warn(owner, "%s", body.c_str());
        }
        else {
            object_post(owner, "%s", body.c_str());
        }
    };
}

} // namespace zerr_max
