#include <cassert>

#include <cnerium/http/Method.hpp>
#include <cnerium/router/Router.hpp>

int main()
{
  using namespace cnerium::http;
  using namespace cnerium::router;

  {
    Router router;
    assert(router.empty());
    assert(router.size() == 0);
    assert(!router.matches(Method::Get, "/"));
  }

  {
    Router router;
    router.get("/");

    assert(!router.empty());
    assert(router.size() == 1);
    assert(router.matches(Method::Get, "/"));
    assert(!router.matches(Method::Post, "/"));

    auto result = router.match(Method::Get, "/");
    assert(result.matched());
    assert(result.params().empty());
  }

  {
    Router router;
    router.get("/about");
    router.get("/users/:id");

    assert(router.size() == 2);
    assert(router.matches(Method::Get, "/about"));
    assert(router.matches(Method::Get, "/users/42"));
    assert(!router.matches(Method::Get, "/contact"));
    assert(!router.matches(Method::Post, "/about"));

    auto result = router.match(Method::Get, "/users/42");
    assert(result.matched());
    assert(result.params().size() == 1);
    assert(result.params().get("id") == "42");
  }

  {
    Router router;
    router.post("/users");
    router.put("/users/:id");
    router.patch("/users/:id");
    router.del("/users/:id");
    router.head("/health");
    router.options("/health");

    assert(router.size() == 6);
    assert(router.matches(Method::Post, "/users"));
    assert(router.matches(Method::Put, "/users/42"));
    assert(router.matches(Method::Patch, "/users/42"));
    assert(router.matches(Method::Delete, "/users/42"));
    assert(router.matches(Method::Head, "/health"));
    assert(router.matches(Method::Options, "/health"));

    assert(!router.matches(Method::Get, "/users"));
    assert(!router.matches(Method::Get, "/health"));
  }

  {
    Router router;
    router.get("/users/me");
    router.get("/users/:id");

    auto result = router.match(Method::Get, "/users/me");
    assert(result.matched());
    assert(result.params().empty());
  }

  {
    Router router;
    router.get("/users/:id");
    router.get("/users/me");

    auto result = router.match(Method::Get, "/users/me");
    assert(result.matched());
    assert(result.params().size() == 1);
    assert(result.params().get("id") == "me");
  }

  {
    Router router;
    router.get("/shops/:shop_id/products/:product_id");

    auto result = router.match(Method::Get, "/shops/10/products/200");
    assert(result.matched());
    assert(result.params().size() == 2);
    assert(result.params().get("shop_id") == "10");
    assert(result.params().get("product_id") == "200");
  }

  {
    Router router;
    router.add(Route(Method::Get, "/blog/:slug"));

    assert(router.size() == 1);
    assert(router.matches(Method::Get, "/blog/hello-world"));

    auto result = router.match(Method::Get, "/blog/hello-world");
    assert(result.matched());
    assert(result.params().get("slug") == "hello-world");
  }

  {
    Router router;
    router.reserve(8);
    router.get("/");
    router.get("/a");
    router.get("/b");

    std::size_t count = 0;
    for (const auto &route : router)
    {
      (void)route;
      ++count;
    }

    assert(count == 3);
  }

  {
    Router router;
    router.get("/");
    router.get("/users/:id");

    const auto &routes = router.routes();
    assert(routes.size() == 2);
    assert(routes[0].pattern() == "/");
    assert(routes[1].pattern() == "/users/:id");
  }

  {
    Router router;
    router.get("/");
    router.post("/submit");

    assert(router.size() == 2);
    router.clear();
    assert(router.empty());
    assert(router.size() == 0);
    assert(!router.matches(Method::Get, "/"));
    assert(!router.matches(Method::Post, "/submit"));
  }

  {
    Router router;
    auto result = router.match(Method::Get, "/missing");
    assert(!result.matched());
    assert(result.failed());
    assert(result.params().empty());
  }

  return 0;
}
