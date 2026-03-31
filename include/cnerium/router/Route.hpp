/**
 * @file Route.hpp
 * @brief cnerium::router — Route definition and matching logic
 *
 * @version 0.1.0
 * @author Gaspard Kirira
 * @copyright (c) 2026 Gaspard Kirira
 * @license MIT
 *
 * @details
 * Defines a single HTTP route with method, pattern, and matching logic.
 *
 * A route is responsible for:
 *   - Storing the HTTP method it accepts
 *   - Storing the route pattern
 *   - Matching an incoming method and path
 *   - Extracting dynamic parameters from the path
 *
 * Supported patterns:
 *   - Static route: /about
 *   - Dynamic route: /users/:id
 *   - Multiple params: /shops/:shop_id/products/:product_id
 *
 * Design goals:
 *   - Header-only
 *   - Simple and predictable matching
 *   - No regex
 *   - Good performance for a V1 router
 *
 * Notes:
 *   - Matching is exact on segment count
 *   - Dynamic segments start with ':'
 *   - Query strings are ignored during matching
 *   - Trailing slashes are normalized
 *
 * Usage:
 * @code
 *   using namespace cnerium::http;
 *   using namespace cnerium::router;
 *
 *   Route route(Method::Get, "/users/:id");
 *
 *   auto result = route.match(Method::Get, "/users/42");
 *   if (result.matched())
 *   {
 *     auto id = result.params().get("id"); // "42"
 *   }
 * @endcode
 */

#pragma once

#include <string>
#include <string_view>
#include <vector>

#include <cnerium/http/Method.hpp>
#include <cnerium/router/MatchResult.hpp>

namespace cnerium::router
{
  /**
   * @brief Represents a single route pattern and its HTTP method.
   *
   * Example:
   *   GET /users/:id
   */
  class Route
  {
  public:
    using method_type = cnerium::http::Method;

    /// @brief Default constructor.
    Route() = default;

    /**
     * @brief Construct a route from method and pattern.
     *
     * @param method HTTP method
     * @param pattern Route pattern
     */
    Route(method_type method, std::string pattern)
        : method_(method),
          pattern_(normalize_path(pattern)),
          segments_(split_segments(pattern_))
    {
    }

    /**
     * @brief Returns the route HTTP method.
     *
     * @return method_type Route method
     */
    [[nodiscard]] method_type method() const noexcept
    {
      return method_;
    }

    /**
     * @brief Returns the original normalized route pattern.
     *
     * @return std::string_view Route pattern
     */
    [[nodiscard]] std::string_view pattern() const noexcept
    {
      return pattern_;
    }

    /**
     * @brief Returns true if the route pattern contains dynamic parameters.
     *
     * @return true if at least one segment starts with ':'
     */
    [[nodiscard]] bool dynamic() const noexcept
    {
      for (const auto &segment : segments_)
      {
        if (is_param_segment(segment))
          return true;
      }
      return false;
    }

    /**
     * @brief Match this route against an incoming method and path.
     *
     * @param method Incoming HTTP method
     * @param path Incoming request path
     * @return MatchResult Match success/failure with extracted params
     */
    [[nodiscard]] MatchResult match(method_type method,
                                    std::string_view path) const
    {
      if (method_ != method)
        return MatchResult::failure();

      const std::string normalized = normalize_path(path);
      const auto path_segments = split_segments(normalized);

      if (segments_.size() != path_segments.size())
        return MatchResult::failure();

      Params params;
      params.reserve(segments_.size());

      for (std::size_t i = 0; i < segments_.size(); ++i)
      {
        const auto &route_segment = segments_[i];
        const auto &path_segment = path_segments[i];

        if (is_param_segment(route_segment))
        {
          const auto name = param_name(route_segment);
          if (name.empty())
            return MatchResult::failure();

          params.set(std::string(name), std::string(path_segment));
          continue;
        }

        if (route_segment != path_segment)
          return MatchResult::failure();
      }

      return MatchResult::success(std::move(params));
    }

    /**
     * @brief Returns true if the route matches the incoming method and path.
     *
     * This is a convenience wrapper around match().
     *
     * @param method Incoming HTTP method
     * @param path Incoming request path
     * @return true if matched
     */
    [[nodiscard]] bool matches(method_type method,
                               std::string_view path) const
    {
      return match(method, path).matched();
    }

    /**
     * @brief Reset the route to an empty GET route.
     */
    void clear()
    {
      method_ = method_type::Get;
      pattern_ = "/";
      segments_.clear();
    }

  private:
    method_type method_{method_type::Get};
    std::string pattern_ = "/";
    std::vector<std::string> segments_{};

    /**
     * @brief Remove query string and normalize trailing slash usage.
     *
     * Rules:
     *   - Empty path becomes "/"
     *   - Query string is ignored
     *   - Trailing slash is removed except for root "/"
     *
     * Examples:
     *   ""              -> "/"
     *   "/"             -> "/"
     *   "/users/"       -> "/users"
     *   "/users?id=42"  -> "/users"
     *
     * @param path Input path
     * @return std::string Normalized path
     */
    [[nodiscard]] static std::string normalize_path(std::string_view path)
    {
      std::string out(path);

      const auto query_pos = out.find('?');
      if (query_pos != std::string::npos)
      {
        out.erase(query_pos);
      }

      if (out.empty())
      {
        return "/";
      }

      if (out.front() != '/')
      {
        out.insert(out.begin(), '/');
      }

      while (out.size() > 1 && out.back() == '/')
      {
        out.pop_back();
      }

      return out;
    }

    /**
     * @brief Split a normalized path into non-empty segments.
     *
     * Examples:
     *   "/"              -> {}
     *   "/users"         -> {"users"}
     *   "/users/42"      -> {"users", "42"}
     *   "/users/:id"     -> {"users", ":id"}
     *
     * @param path Normalized path
     * @return std::vector<std::string> Path segments
     */
    [[nodiscard]] static std::vector<std::string> split_segments(std::string_view path)
    {
      std::vector<std::string> result;

      std::size_t start = 0;
      while (start < path.size())
      {
        while (start < path.size() && path[start] == '/')
        {
          ++start;
        }

        if (start >= path.size())
          break;

        std::size_t end = start;
        while (end < path.size() && path[end] != '/')
        {
          ++end;
        }

        result.emplace_back(path.substr(start, end - start));
        start = end;
      }

      return result;
    }

    /**
     * @brief Returns true if a segment is a dynamic parameter.
     *
     * Example:
     *   ":id" -> true
     *   "id"  -> false
     *   ":"   -> false
     *
     * @param segment Route segment
     * @return true if dynamic
     */
    [[nodiscard]] static bool is_param_segment(std::string_view segment) noexcept
    {
      return segment.size() > 1 && segment.front() == ':';
    }

    /**
     * @brief Extract parameter name from a dynamic segment.
     *
     * Example:
     *   ":id" -> "id"
     *
     * @param segment Route segment
     * @return std::string_view Parameter name
     */
    [[nodiscard]] static std::string_view param_name(std::string_view segment) noexcept
    {
      if (!is_param_segment(segment))
        return {};
      return segment.substr(1);
    }
  };

} // namespace cnerium::router
