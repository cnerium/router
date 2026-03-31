# cnerium/router

HTTP routing for the Cnerium web framework.

**Header-only. Deterministic. Framework-ready.**

---

## Download

https://vixcpp.com/registry/pkg/cnerium/router

---

## Overview

`cnerium/router` provides a lightweight HTTP router built on top of `cnerium/http`.

It is responsible for:

- route definition
- method-based matching
- path matching
- parameter extraction

It is designed to be:

- minimal
- predictable
- independent from transport layers

No server required.
No middleware required.

Just routing.

---

## Why cnerium/router?

Most projects end up re-implementing:

- route matching
- parameter parsing
- method dispatch
- path normalization
- route ordering logic

These implementations are often:

- inconsistent
- tightly coupled to servers
- hard to test
- difficult to reuse

`cnerium/router` fixes this by providing a clean, reusable routing layer.

---

## Dependency

Depends on:

- `cnerium/http`

This ensures:

- shared HTTP method definitions
- consistent request handling
- seamless integration with higher layers

---

## Installation

### Using Vix

```bash
vix add @cnerium/router
vix install
```

### Manual

```bash
git clone https://github.com/cnerium/router.git
```

Add `include/` to your project.

---

## Core Concepts

### Router

```cpp
Router router;

router.get("/");
router.get("/users/:id");
```

### Matching

```cpp
auto result = router.match(Method::Get, "/users/42");

if (result.matched())
{
  auto id = result.params().get("id");
}
```

### Dynamic Parameters

```cpp
router.get("/shops/:shop_id/products/:product_id");

auto result = router.match(Method::Get, "/shops/10/products/200");

auto shop = result.params().get("shop_id");
auto product = result.params().get("product_id");
```

### HTTP Methods

```cpp
router.get("/users");
router.post("/users");
router.put("/users/:id");
router.patch("/users/:id");
router.del("/users/:id");
router.head("/health");
router.options("/health");
```

### Route Priority

Routes are matched in insertion order.

```cpp
router.get("/users/me");
router.get("/users/:id");

auto result = router.match(Method::Get, "/users/me");
```

This matches the static route first.

---

## Typical Flow

```cpp
Router router;

router.get("/");
router.get("/users/:id");

auto result = router.match(Method::Get, "/users/42");

if (result)
{
  auto id = result.params().get("id");
}
```

---

## Example

```cpp
#include <iostream>

#include <cnerium/http/Method.hpp>
#include <cnerium/router/router.hpp>

using namespace cnerium::http;
using namespace cnerium::router;

int main()
{
  Router router;

  router.get("/");
  router.get("/users");
  router.get("/users/:id");

  auto result = router.match(Method::Get, "/users/42");

  if (result)
  {
    std::cout << "User id = "
              << result.params().get("id")
              << "\n";
  }
  else
  {
    std::cout << "No match\n";
  }

  return 0;
}
```

---

## Supported Patterns

| Pattern | Description |
|--------|-------------|
| `/about` | static route |
| `/users/:id` | single parameter |
| `/shops/:shop/products/:id` | multiple parameters |

---

## Matching Rules

- method must match
- segment count must match
- `:param` captures a segment
- static segments must match exactly
- query strings are ignored
- trailing slashes are normalized

---

## Complexity

| Operation | Complexity |
|----------|-----------|
| route matching | O(n * m) |
| param extraction | O(m) |
| route insertion | amortized O(1) |

Where:

- `n` = number of routes
- `m` = number of segments

---

## Design Philosophy

- minimal router core
- no regex
- no hidden behavior
- deterministic matching
- framework-agnostic

---

## Tests

```bash
vix build
vix test
```

---

## License

MIT License
Copyright (c) Gaspard Kirira

