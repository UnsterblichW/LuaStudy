
for k, _ in pairs(_G) do
    print(k)
end

mark = nil

-- local z = 10

-- _ENV.x = _ENV.y + z

local print = print
local sin = math.sin

-- _ENV = {}
print(30)
print(sin(30))
-- print(math.cos(30))

setmetatable(_G, {
    __newindex = function(_, k, _)
        error("尝试写一个未声明的全局变量 " .. k, 2)
    end,
    __index = function(_, k)
        error("尝试读一个未声明的全局变量 " .. k, 2)
    end,
})

local function declave( name, initual)
    rawset(_G, name, initual or false)    
end

declave("mark")


mark = nil


-- local z = 10

-- _ENV.x = _ENV.y + z