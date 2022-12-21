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
print("--------------------------------")
print(data.firstName, data.lastName, data.isAlive, data.age, data.spouse)
print("--------------------------------")
for key, value in pairs(data.address) do
  print(key, value)
end
print("--------------------------------")
for key, value in pairs(data.phoneNumbers) do
  print(key, value)
end
print("--------------------------------")
for key, value in pairs(data.children) do
  print(key, value)
end
print("--------------------------------")

local indent = 2
local datastr = json.stringify(data, indent)
-- local datastr = json.stringify(data)
print("stringify result:", datastr)