#include <iostream>

#include <cnerium/http/Method.hpp>
#include <cnerium/router/router.hpp>

int main()
{
  using namespace cnerium::http;
  using namespace cnerium::router;

  Router router;
  router.get("/users/:id");

  auto result = router.match(Method::Get, "/users/42");

  if (result)
  {
    std::cout << "User id = " << result.params().get("id") << "\n";
  }
  else
  {
    std::cout << "No match\n";
  }

  return 0;
}
