/**
 * @file router.hpp
 * @brief cnerium::router — Main public header for the Router module
 *
 * @version 0.1.0
 * @author Gaspard Kirira
 * @copyright (c) 2026 Gaspard Kirira
 * @license MIT
 *
 * @details
 * Aggregates the main public headers of the Cnerium Router module.
 *
 * Include this file when you want access to the full high-level router API:
 *   - version information
 *   - route parameters container
 *   - route match result
 *   - route definition
 *   - router container
 *
 * Usage:
 * @code
 *   #include <cnerium/router/router.hpp>
 *
 *   using namespace cnerium::http;
 *   using namespace cnerium::router;
 *
 *   Router router;
 *   router.get("/");
 *   router.get("/users/:id");
 *
 *   auto result = router.match(Method::Get, "/users/42");
 *   if (result.matched())
 *   {
 *     auto id = result.params().get("id");
 *   }
 * @endcode
 */

#pragma once

#include <cnerium/router/version.hpp>
#include <cnerium/router/Params.hpp>
#include <cnerium/router/MatchResult.hpp>
#include <cnerium/router/Route.hpp>
#include <cnerium/router/Router.hpp>
