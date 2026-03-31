#include <iostream>

#include <cnerium/http/Method.hpp>
#include <cnerium/router/router.hpp>

int main()
{
  using namespace cnerium::http;
  using namespace cnerium::router;

  Router router;

  router.get("/users");
  router.post("/users");

  auto get_res = router.match(Method::Get, "/users");
  auto post_res = router.match(Method::Post, "/users");

  if (get_res)
    std::cout << "GET /users matched\n";

  if (post_res)
    std::cout << "POST /users matched\n";

  return 0;
}
