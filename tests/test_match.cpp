#include <cassert>

#include <cnerium/router/MatchResult.hpp>

int main()
{
  using namespace cnerium::router;

  {
    MatchResult result;
    assert(!result.matched());
    assert(result.failed());
    assert(!static_cast<bool>(result));
    assert(result.params().empty());
  }

  {
    auto result = MatchResult::failure();
    assert(!result.matched());
    assert(result.failed());
    assert(!static_cast<bool>(result));
    assert(result.params().empty());
  }

  {
    auto result = MatchResult::success();
    assert(result.matched());
    assert(!result.failed());
    assert(static_cast<bool>(result));
    assert(result.params().empty());
  }

  {
    Params params;
    params.set("id", "42");
    params.set("slug", "hello-world");

    auto result = MatchResult::success(std::move(params));
    assert(result.matched());
    assert(!result.failed());
    assert(static_cast<bool>(result));
    assert(result.params().size() == 2);
    assert(result.params().contains("id"));
    assert(result.params().contains("slug"));
    assert(result.params().get("id") == "42");
    assert(result.params().get("slug") == "hello-world");
  }

  {
    Params params;
    params.set("shop_id", "10");

    MatchResult result(true, std::move(params));
    assert(result.matched());
    assert(!result.failed());
    assert(static_cast<bool>(result));
    assert(result.params().size() == 1);
    assert(result.params().get("shop_id") == "10");
  }

  {
    MatchResult result = MatchResult::success();
    result.params().set("id", "99");

    assert(result.matched());
    assert(result.params().contains("id"));
    assert(result.params().get("id") == "99");
  }

  {
    MatchResult result = MatchResult::success();
    result.params().set("id", "42");
    result.clear();

    assert(!result.matched());
    assert(result.failed());
    assert(!static_cast<bool>(result));
    assert(result.params().empty());
  }

  return 0;
}
