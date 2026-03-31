#include <iostream>

#include <cnerium/http/Method.hpp>
#include <cnerium/router/router.hpp>

int main()
{
  using namespace cnerium::http;
  using namespace cnerium::router;

  Router router;
  router.get("/");

  auto result = router.match(Method::Get, "/");

  if (result)
  {
    std::cout << "Matched root route\n";
  }
  else
  {
    std::cout << "No match\n";
  }

  return 0;
}
