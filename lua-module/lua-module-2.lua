local meta = require "meta"

local calc = meta.new({1,2,3,4,5})

print(calc.sum(calc))

print(calc:sum())

local sumer = meta.new({10,20,30})

print(sumer:sum())
