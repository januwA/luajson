## luajson

## Build
```sh
$ git clone https://github.com/januwA/luajson
$ cd luajson
$ cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
$ cmake --build build --config Release
```

Provide lua include directory and lib directory
```
$ cmake -DCMAKE_BUILD_TYPE=Release -DLUA_INCLUDE_DIR="D:\program\lua" -DLUA_LIB_DIR="D:\program\lua\build\Release" -S. -Bbuild
```

## Example
```lua
local json = require "luajson"

local data = json.parse(
  [[
  {
    "firstName": "John 😍 🥰 😘",
    "lastName": "Smith",
    "isAlive": true,
    "age": 27,
    "address": {
      "streetAddress": "21 2nd Street",
      "city": "New York",
      "state": "NY",
      "postalCode": "10021-3100"
    },
    "phoneNumbers": [
      {
        "type": "home",
        "number": "212 555-1234"
      },
      {
        "type": "office",
        "number": "646 555-4567"
      }
    ],
    "children": [
        "Catherine",
        "Thomas",
        "Trevor"
    ],
    "spouse": null
  }
]]
)

local indent = 2
local datastr = json.stringify(data, indent)
print(datastr)
```