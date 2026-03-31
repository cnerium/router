#include <iostream>

#include <cnerium/http/Method.hpp>
#include <cnerium/router/router.hpp>

int main()
{
  using namespace cnerium::http;
  using namespace cnerium::router;

  Router router;

  router.get("/");
  router.get("/users");
  router.get("/users/:id");
  router.post("/users");
  router.get("/shops/:shop_id/products/:product_id");

  auto test = [&](Method method, std::string_view path)
  {
    auto result = router.match(method, path);

    std::cout << to_string(method) << " " << path << " -> ";

    if (!result)
    {
      std::cout << "NO MATCH\n";
      return;
    }

    if (result.params().empty())
    {
      std::cout << "MATCH (no params)\n";
      return;
    }

    std::cout << "MATCH params: ";
    for (const auto &[k, v] : result.params())
    {
      std::cout << k << "=" << v << " ";
    }
    std::cout << "\n";
  };

  test(Method::Get, "/");
  test(Method::Get, "/users");
  test(Method::Get, "/users/42");
  test(Method::Post, "/users");
  test(Method::Get, "/shops/10/products/200");
  test(Method::Get, "/unknown");

  return 0;
}
