#include <cassert>
#include <string_view>

#include <cnerium/http/Method.hpp>
#include <cnerium/router/router.hpp>

int main()
{
  using namespace cnerium::http;
  using namespace cnerium::router;

  static_assert(version_major == 0);
  static_assert(version_minor == 1);
  static_assert(version_patch == 0);
  static_assert(version_string() == std::string_view("0.1.0"));
  static_assert(version_number() == 100);

  {
    Params params;
    assert(params.empty());
    assert(params.size() == 0);
    assert(!params.contains("id"));
    assert(params.get("id").empty());
    assert(params.get_or("id", "fallback") == "fallback");
  }

  {
    MatchResult result;
    assert(!result.matched());
    assert(result.failed());
    assert(!static_cast<bool>(result));
    assert(result.params().empty());
  }

  {
    Route route(Method::Get, "/");
    assert(route.method() == Method::Get);
    assert(route.pattern() == "/");
    assert(!route.dynamic());

    const auto result = route.match(Method::Get, "/");
    assert(result.matched());
    assert(result.params().empty());
  }

  {
    Router router;
    assert(router.empty());
    assert(router.size() == 0);

    router.get("/");
    assert(!router.empty());
    assert(router.size() == 1);
    assert(router.matches(Method::Get, "/"));
    assert(!router.matches(Method::Post, "/"));
  }

  return 0;
}
