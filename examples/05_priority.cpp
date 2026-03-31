#include <iostream>

#include <cnerium/http/Method.hpp>
#include <cnerium/router/router.hpp>

int main()
{
  using namespace cnerium::http;
  using namespace cnerium::router;

  Router router;

  // Static route FIRST
  router.get("/users/me");
  router.get("/users/:id");

  auto result = router.match(Method::Get, "/users/me");

  if (result)
  {
    if (result.params().empty())
      std::cout << "Matched static route /users/me\n";
    else
      std::cout << "Matched dynamic route id=" << result.params().get("id") << "\n";
  }

  return 0;
}
