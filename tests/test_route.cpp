#include <cassert>

#include <cnerium/http/Method.hpp>
#include <cnerium/router/Route.hpp>

int main()
{
  using namespace cnerium::http;
  using namespace cnerium::router;

  {
    Route route;
    assert(route.method() == Method::Get);
    assert(route.pattern() == "/");
    assert(!route.dynamic());
    assert(route.matches(Method::Get, "/"));
    assert(!route.matches(Method::Post, "/"));
  }

  {
    Route route(Method::Get, "/");
    assert(route.method() == Method::Get);
    assert(route.pattern() == "/");
    assert(!route.dynamic());

    auto result = route.match(Method::Get, "/");
    assert(result.matched());
    assert(result.params().empty());
  }

  {
    Route route(Method::Get, "/about");
    assert(route.pattern() == "/about");
    assert(!route.dynamic());

    assert(route.matches(Method::Get, "/about"));
    assert(route.matches(Method::Get, "/about/"));
    assert(route.matches(Method::Get, "/about?x=1"));
    assert(!route.matches(Method::Post, "/about"));
    assert(!route.matches(Method::Get, "/contact"));

    auto result = route.match(Method::Get, "/about");
    assert(result.matched());
    assert(result.params().empty());
  }

  {
    Route route(Method::Get, "/users/:id");
    assert(route.dynamic());

    auto result = route.match(Method::Get, "/users/42");
    assert(result.matched());
    assert(result.params().size() == 1);
    assert(result.params().contains("id"));
    assert(result.params().get("id") == "42");
  }

  {
    Route route(Method::Get, "/users/:id");
    assert(route.matches(Method::Get, "/users/42"));
    assert(route.matches(Method::Get, "/users/42/"));
    assert(route.matches(Method::Get, "/users/42?tab=profile"));
    assert(!route.matches(Method::Post, "/users/42"));
    assert(!route.matches(Method::Get, "/users"));
    assert(!route.matches(Method::Get, "/users/42/edit"));
  }

  {
    Route route(Method::Get, "/shops/:shop_id/products/:product_id");
    assert(route.dynamic());

    auto result = route.match(Method::Get, "/shops/10/products/200");
    assert(result.matched());
    assert(result.params().size() == 2);
    assert(result.params().get("shop_id") == "10");
    assert(result.params().get("product_id") == "200");
  }

  {
    Route route(Method::Get, "/blog/:slug/comments/:comment_id");

    auto result = route.match(Method::Get, "/blog/hello-world/comments/99");
    assert(result.matched());
    assert(result.params().size() == 2);
    assert(result.params().get("slug") == "hello-world");
    assert(result.params().get("comment_id") == "99");
  }

  {
    Route route(Method::Get, "/users/:id");
    auto result = route.match(Method::Get, "/users/");
    assert(!result.matched());
    assert(result.failed());
  }

  {
    Route route(Method::Get, "/users/:id");
    auto result = route.match(Method::Get, "/users");
    assert(!result.matched());
    assert(result.failed());
  }

  {
    Route route(Method::Get, "/users/:id");
    auto result = route.match(Method::Get, "/posts/42");
    assert(!result.matched());
    assert(result.failed());
  }

  {
    Route route(Method::Get, "/users/:id");
    auto result = route.match(Method::Post, "/users/42");
    assert(!result.matched());
    assert(result.failed());
  }

  {
    Route route(Method::Get, "/users/:id/profile");

    auto result = route.match(Method::Get, "/users/42/profile");
    assert(result.matched());
    assert(result.params().size() == 1);
    assert(result.params().get("id") == "42");
  }

  {
    Route route(Method::Get, "users/:id");
    assert(route.pattern() == "/users/:id");

    auto result = route.match(Method::Get, "users/42");
    assert(result.matched());
    assert(result.params().get("id") == "42");
  }

  {
    Route route(Method::Get, "/:");
    assert(route.pattern() == "/:");
    assert(!route.matches(Method::Get, "/test"));
  }

  {
    Route route(Method::Get, "/users/:id");
    route.clear();

    assert(route.method() == Method::Get);
    assert(route.pattern() == "/");
    assert(!route.dynamic());
    assert(route.matches(Method::Get, "/"));
    assert(!route.matches(Method::Get, "/users/42"));
  }

  return 0;
}
