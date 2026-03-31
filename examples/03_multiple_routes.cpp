#include <iostream>

#include <cnerium/http/Method.hpp>
#include <cnerium/router/router.hpp>

int main()
{
  using namespace cnerium::http;
  using namespace cnerium::router;

  Router router;

  router.get("/");
  router.get("/about");
  router.get("/users/:id");

  auto r1 = router.match(Method::Get, "/about");
  auto r2 = router.match(Method::Get, "/users/99");

  if (r1)
    std::cout << "Matched /about\n";

  if (r2)
    std::cout << "User id = " << r2.params().get("id") << "\n";

  return 0;
}
