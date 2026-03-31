#include <cassert>
#include <string_view>

#include <cnerium/router/Params.hpp>

int main()
{
  using namespace cnerium::router;

  {
    Params params;
    assert(params.empty());
    assert(params.size() == 0);
    assert(!params.contains("id"));
    assert(params.get("id").empty());
    assert(params.get_or("id", "fallback") == "fallback");
  }

  {
    Params params;
    params.set("id", "42");

    assert(!params.empty());
    assert(params.size() == 1);
    assert(params.contains("id"));
    assert(!params.contains("slug"));
    assert(params.get("id") == "42");
    assert(params.get("slug").empty());
    assert(params.get_or("id", "fallback") == "42");
    assert(params.get_or("slug", "fallback") == "fallback");
  }

  {
    Params params;
    params.set("id", "42");
    params.set("slug", "hello-world");

    assert(params.size() == 2);
    assert(params.contains("id"));
    assert(params.contains("slug"));
    assert(params.get("id") == "42");
    assert(params.get("slug") == "hello-world");
  }

  {
    Params params;
    params.set("id", "42");
    params.set("id", "99");

    assert(params.size() == 1);
    assert(params.contains("id"));
    assert(params.get("id") == "99");
  }

  {
    Params params;
    params.reserve(4);
    params.set("shop_id", "10");
    params.set("product_id", "200");

    assert(params.size() == 2);

    const auto &data = params.data();
    assert(data.size() == 2);
    assert(data[0].first == "shop_id");
    assert(data[0].second == "10");
    assert(data[1].first == "product_id");
    assert(data[1].second == "200");
  }

  {
    Params params;
    params.set("id", "42");
    params.set("slug", "post-title");

    assert(params.erase("id"));
    assert(!params.contains("id"));
    assert(params.size() == 1);
    assert(!params.erase("id"));
    assert(params.contains("slug"));
    assert(params.get("slug") == "post-title");
  }

  {
    Params params;
    params.set("id", "42");
    params.set("slug", "abc");

    std::size_t count = 0;
    for (const auto &[key, value] : params)
    {
      if (count == 0)
      {
        assert(key == "id");
        assert(value == "42");
      }
      else if (count == 1)
      {
        assert(key == "slug");
        assert(value == "abc");
      }
      ++count;
    }

    assert(count == 2);
  }

  {
    Params params;
    params.set("id", "42");
    params.set("slug", "abc");
    params.clear();

    assert(params.empty());
    assert(params.size() == 0);
    assert(!params.contains("id"));
    assert(!params.contains("slug"));
    assert(params.get("id").empty());
  }

  return 0;
}
