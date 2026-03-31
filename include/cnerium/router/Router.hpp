/**
 * @file Router.hpp
 * @brief cnerium::router — HTTP router container
 *
 * @version 0.1.0
 * @author Gaspard Kirira
 * @copyright (c) 2026 Gaspard Kirira
 * @license MIT
 *
 * @details
 * Defines the main router container responsible for storing routes
 * and resolving incoming HTTP method/path pairs.
 *
 * A router is responsible for:
 *   - Registering routes
 *   - Preserving route insertion order
 *   - Matching incoming requests against stored routes
 *   - Returning the first matching route result
 *
 * Design goals:
 *   - Header-only
 *   - Simple and predictable behavior
 *   - Good performance for a V1 router
 *   - Clean API for future handler integration
 *
 * Notes:
 *   - Matching is performed in insertion order
 *   - The first matching route wins
 *   - This V1 router stores only route definitions
 *   - Handler binding can be added later without changing the core matching API
 *
 * Usage:
 * @code
 *   using namespace cnerium::http;
 *   using namespace cnerium::router;
 *
 *   Router router;
 *   router.add(Method::Get, "/");
 *   router.add(Method::Get, "/users/:id");
 *
 *   auto result = router.match(Method::Get, "/users/42");
 *   if (result.matched())
 *   {
 *     auto id = result.params().get("id"); // "42"
 *   }
 * @endcode
 */

#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <cnerium/http/Method.hpp>
#include <cnerium/router/MatchResult.hpp>
#include <cnerium/router/Route.hpp>

namespace cnerium::router
{
  /**
   * @brief Ordered collection of routes with matching utilities.
   *
   * Stores routes in insertion order and resolves the first match.
   */
  class Router
  {
  public:
    using method_type = cnerium::http::Method;
    using storage_type = std::vector<Route>;
    using iterator = storage_type::iterator;
    using const_iterator = storage_type::const_iterator;

    /// @brief Default constructor.
    Router() = default;

    /**
     * @brief Add a route to the router.
     *
     * Routes are evaluated in the same order they are added.
     *
     * @param method HTTP method
     * @param pattern Route pattern
     * @return Route& Reference to the stored route
     */
    Route &add(method_type method, std::string pattern)
    {
      routes_.emplace_back(method, std::move(pattern));
      return routes_.back();
    }

    /**
     * @brief Add an already constructed route.
     *
     * @param route Route instance
     * @return Route& Reference to the stored route
     */
    Route &add(Route route)
    {
      routes_.push_back(std::move(route));
      return routes_.back();
    }

    /**
     * @brief Convenience helper for registering a GET route.
     *
     * @param pattern Route pattern
     * @return Route& Reference to the stored route
     */
    Route &get(std::string pattern)
    {
      return add(method_type::Get, std::move(pattern));
    }

    /**
     * @brief Convenience helper for registering a POST route.
     *
     * @param pattern Route pattern
     * @return Route& Reference to the stored route
     */
    Route &post(std::string pattern)
    {
      return add(method_type::Post, std::move(pattern));
    }

    /**
     * @brief Convenience helper for registering a PUT route.
     *
     * @param pattern Route pattern
     * @return Route& Reference to the stored route
     */
    Route &put(std::string pattern)
    {
      return add(method_type::Put, std::move(pattern));
    }

    /**
     * @brief Convenience helper for registering a PATCH route.
     *
     * @param pattern Route pattern
     * @return Route& Reference to the stored route
     */
    Route &patch(std::string pattern)
    {
      return add(method_type::Patch, std::move(pattern));
    }

    /**
     * @brief Convenience helper for registering a DELETE route.
     *
     * @param pattern Route pattern
     * @return Route& Reference to the stored route
     */
    Route &del(std::string pattern)
    {
      return add(method_type::Delete, std::move(pattern));
    }

    /**
     * @brief Convenience helper for registering a HEAD route.
     *
     * @param pattern Route pattern
     * @return Route& Reference to the stored route
     */
    Route &head(std::string pattern)
    {
      return add(method_type::Head, std::move(pattern));
    }

    /**
     * @brief Convenience helper for registering an OPTIONS route.
     *
     * @param pattern Route pattern
     * @return Route& Reference to the stored route
     */
    Route &options(std::string pattern)
    {
      return add(method_type::Options, std::move(pattern));
    }

    /**
     * @brief Match an incoming request against registered routes.
     *
     * Returns the first matching route result.
     *
     * @param method Incoming HTTP method
     * @param path Incoming request path
     * @return MatchResult Successful or failed match result
     */
    [[nodiscard]] MatchResult match(method_type method,
                                    std::string_view path) const
    {
      for (const auto &route : routes_)
      {
        auto result = route.match(method, path);
        if (result.matched())
          return result;
      }

      return MatchResult::failure();
    }

    /**
     * @brief Returns true if at least one route matches.
     *
     * @param method Incoming HTTP method
     * @param path Incoming request path
     * @return true if a route matches
     */
    [[nodiscard]] bool matches(method_type method,
                               std::string_view path) const
    {
      return match(method, path).matched();
    }

    /**
     * @brief Returns the number of registered routes.
     *
     * @return std::size_t Route count
     */
    [[nodiscard]] std::size_t size() const noexcept
    {
      return routes_.size();
    }

    /**
     * @brief Returns true if no routes are registered.
     *
     * @return true if empty
     */
    [[nodiscard]] bool empty() const noexcept
    {
      return routes_.empty();
    }

    /**
     * @brief Reserve storage for routes.
     *
     * @param n Number of routes to reserve
     */
    void reserve(std::size_t n)
    {
      routes_.reserve(n);
    }

    /**
     * @brief Remove all registered routes.
     */
    void clear() noexcept
    {
      routes_.clear();
    }

    /**
     * @brief Returns const access to internal route storage.
     *
     * @return const storage_type& Registered routes
     */
    [[nodiscard]] const storage_type &routes() const noexcept
    {
      return routes_;
    }

    /**
     * @brief Returns mutable access to internal route storage.
     *
     * @return storage_type& Registered routes
     */
    [[nodiscard]] storage_type &routes() noexcept
    {
      return routes_;
    }

    /// @brief Mutable begin iterator.
    [[nodiscard]] iterator begin() noexcept
    {
      return routes_.begin();
    }

    /// @brief Mutable end iterator.
    [[nodiscard]] iterator end() noexcept
    {
      return routes_.end();
    }

    /// @brief Const begin iterator.
    [[nodiscard]] const_iterator begin() const noexcept
    {
      return routes_.begin();
    }

    /// @brief Const end iterator.
    [[nodiscard]] const_iterator end() const noexcept
    {
      return routes_.end();
    }

    /// @brief Const begin iterator.
    [[nodiscard]] const_iterator cbegin() const noexcept
    {
      return routes_.cbegin();
    }

    /// @brief Const end iterator.
    [[nodiscard]] const_iterator cend() const noexcept
    {
      return routes_.cend();
    }

  private:
    storage_type routes_{};
  };

} // namespace cnerium::router
