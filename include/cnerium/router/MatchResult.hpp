/**
 * @file MatchResult.hpp
 * @brief cnerium::router — Route match result
 *
 * @version 0.1.0
 * @author Gaspard Kirira
 * @copyright (c) 2026 Gaspard Kirira
 * @license MIT
 *
 * @details
 * Defines the result of a route matching operation.
 *
 * A match result tells whether a route successfully matched
 * an incoming HTTP method and path, and exposes any extracted
 * route parameters.
 *
 * Design goals:
 *   - Lightweight result object
 *   - Clear success/failure state
 *   - Cheap to move and return by value
 *   - Direct access to extracted route parameters
 *
 * Notes:
 *   - A failed match contains no parameters
 *   - A successful match may still contain zero parameters
 *     for fully static routes
 *
 * Usage:
 * @code
 *   using namespace cnerium::router;
 *
 *   MatchResult result = MatchResult::success();
 *
 *   if (result.matched())
 *   {
 *     // route matched
 *   }
 *
 *   MatchResult with_params = MatchResult::success({
 *     {"id", "42"}
 *   });
 *
 *   auto id = with_params.params().get("id");
 * @endcode
 */

#pragma once

#include <utility>

#include <cnerium/router/Params.hpp>

namespace cnerium::router
{
  /**
   * @brief Result of matching a route against an incoming request.
   *
   * Stores whether the route matched and the extracted parameters.
   */
  class MatchResult
  {
  public:
    /// @brief Default constructor creates a failed match.
    MatchResult() = default;

    /**
     * @brief Construct a match result with explicit state and parameters.
     *
     * @param matched Whether the route matched
     * @param params Extracted route parameters
     */
    MatchResult(bool matched, Params params)
        : matched_(matched), params_(std::move(params))
    {
    }

    /**
     * @brief Create a successful match with no parameters.
     *
     * @return MatchResult Successful result
     */
    [[nodiscard]] static MatchResult success()
    {
      return MatchResult(true, {});
    }

    /**
     * @brief Create a successful match with parameters.
     *
     * @param params Extracted route parameters
     * @return MatchResult Successful result
     */
    [[nodiscard]] static MatchResult success(Params params)
    {
      return MatchResult(true, std::move(params));
    }

    /**
     * @brief Create a failed match.
     *
     * @return MatchResult Failed result
     */
    [[nodiscard]] static MatchResult failure()
    {
      return MatchResult(false, {});
    }

    /**
     * @brief Returns true if the route matched.
     *
     * @return true if matched
     */
    [[nodiscard]] bool matched() const noexcept
    {
      return matched_;
    }

    /**
     * @brief Returns true if the route did not match.
     *
     * @return true if not matched
     */
    [[nodiscard]] bool failed() const noexcept
    {
      return !matched_;
    }

    /**
     * @brief Returns const access to extracted parameters.
     *
     * @return const Params& Route parameters
     */
    [[nodiscard]] const Params &params() const noexcept
    {
      return params_;
    }

    /**
     * @brief Returns mutable access to extracted parameters.
     *
     * @return Params& Route parameters
     */
    [[nodiscard]] Params &params() noexcept
    {
      return params_;
    }

    /**
     * @brief Explicit boolean conversion.
     *
     * Allows usage such as:
     * @code
     *   if (result) { ... }
     * @endcode
     *
     * @return true if matched
     */
    explicit operator bool() const noexcept
    {
      return matched_;
    }

    /**
     * @brief Reset the match result to failure state.
     *
     * Clears parameters and marks the result as not matched.
     */
    void clear() noexcept
    {
      matched_ = false;
      params_.clear();
    }

  private:
    bool matched_{false};
    Params params_{};
  };

} // namespace cnerium::router
