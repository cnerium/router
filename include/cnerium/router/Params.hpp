/**
 * @file Params.hpp
 * @brief cnerium::router — Route parameters container
 *
 * @version 0.1.0
 * @author Gaspard Kirira
 * @copyright (c) 2026 Gaspard Kirira
 * @license MIT
 *
 * @details
 * Defines a lightweight container for route parameters extracted
 * during URL matching.
 *
 * Example:
 *   Route pattern: /users/:id
 *   Path:          /users/42
 *
 *   Extracted params:
 *     id -> "42"
 *
 * Design goals:
 *   - Simple API
 *   - Zero dependencies
 *   - Good cache locality
 *   - Small-size optimization (few params typical)
 *
 * Notes:
 *   - Internally uses a flat vector of key/value pairs
 *   - Lookup is linear (acceptable for small param sets)
 *
 * Usage:
 * @code
 *   using namespace cnerium::router;
 *
 *   Params params;
 *   params.set("id", "42");
 *
 *   if (params.contains("id"))
 *   {
 *     auto id = params.get("id"); // "42"
 *   }
 * @endcode
 */

#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace cnerium::router
{
  /**
   * @brief Container for route parameters.
   *
   * Stores key/value pairs extracted from dynamic routes.
   */
  class Params
  {
  public:
    using value_type = std::pair<std::string, std::string>;
    using storage_type = std::vector<value_type>;
    using iterator = storage_type::iterator;
    using const_iterator = storage_type::const_iterator;

    /// @brief Default constructor.
    Params() = default;

    /**
     * @brief Insert or overwrite a parameter.
     *
     * @param key Parameter name
     * @param value Parameter value
     */
    void set(std::string key, std::string value)
    {
      if (auto *existing = find_value(key))
      {
        *existing = std::move(value);
        return;
      }

      data_.emplace_back(std::move(key), std::move(value));
    }

    /**
     * @brief Check if a parameter exists.
     *
     * @param key Parameter name
     * @return true if found
     */
    [[nodiscard]] bool contains(std::string_view key) const noexcept
    {
      return find_index(key) != npos();
    }

    /**
     * @brief Get a parameter value.
     *
     * @param key Parameter name
     * @return std::string_view Value or empty if missing
     */
    [[nodiscard]] std::string_view get(std::string_view key) const noexcept
    {
      const auto idx = find_index(key);
      if (idx == npos())
        return {};
      return data_[idx].second;
    }

    /**
     * @brief Get a parameter value or fallback.
     *
     * @param key Parameter name
     * @param fallback Value returned if missing
     * @return std::string_view Value or fallback
     */
    [[nodiscard]] std::string_view get_or(std::string_view key,
                                          std::string_view fallback) const noexcept
    {
      const auto idx = find_index(key);
      if (idx == npos())
        return fallback;
      return data_[idx].second;
    }

    /**
     * @brief Remove a parameter.
     *
     * @param key Parameter name
     * @return true if removed
     */
    bool erase(std::string_view key)
    {
      const auto idx = find_index(key);
      if (idx == npos())
        return false;

      data_.erase(data_.begin() + static_cast<std::ptrdiff_t>(idx));
      return true;
    }

    /// @brief Remove all parameters.
    void clear() noexcept
    {
      data_.clear();
    }

    /// @brief Reserve storage.
    void reserve(std::size_t n)
    {
      data_.reserve(n);
    }

    /// @brief Number of parameters.
    [[nodiscard]] std::size_t size() const noexcept
    {
      return data_.size();
    }

    /// @brief Returns true if empty.
    [[nodiscard]] bool empty() const noexcept
    {
      return data_.empty();
    }

    /// @brief Mutable begin iterator.
    [[nodiscard]] iterator begin() noexcept
    {
      return data_.begin();
    }

    /// @brief Mutable end iterator.
    [[nodiscard]] iterator end() noexcept
    {
      return data_.end();
    }

    /// @brief Const begin iterator.
    [[nodiscard]] const_iterator begin() const noexcept
    {
      return data_.begin();
    }

    /// @brief Const end iterator.
    [[nodiscard]] const_iterator end() const noexcept
    {
      return data_.end();
    }

    /// @brief Const begin iterator.
    [[nodiscard]] const_iterator cbegin() const noexcept
    {
      return data_.cbegin();
    }

    /// @brief Const end iterator.
    [[nodiscard]] const_iterator cend() const noexcept
    {
      return data_.cend();
    }

    /// @brief Direct access to storage.
    [[nodiscard]] const storage_type &data() const noexcept
    {
      return data_;
    }

    /// @brief Mutable access to storage.
    [[nodiscard]] storage_type &data() noexcept
    {
      return data_;
    }

  private:
    storage_type data_{};

    static constexpr std::size_t npos() noexcept
    {
      return static_cast<std::size_t>(-1);
    }

    [[nodiscard]] std::size_t find_index(std::string_view key) const noexcept
    {
      for (std::size_t i = 0; i < data_.size(); ++i)
      {
        if (data_[i].first == key)
          return i;
      }
      return npos();
    }

    [[nodiscard]] std::string *find_value(std::string_view key) noexcept
    {
      for (auto &kv : data_)
      {
        if (kv.first == key)
          return &kv.second;
      }
      return nullptr;
    }
  };

} // namespace cnerium::router
