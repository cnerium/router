#include <router/router.hpp>
#include <iostream>

int main()
{
  auto nodes = router::make_chain(5);
  std::cout << "nodes=" << nodes.size() << "\n";
  return nodes.size() == 5 ? 0 : 1;
}
