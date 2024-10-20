
local tab = {x=1,y=2}

local function set_default(t, default)
    local mt = {__index = function () return default end}
    setmetatable(t, mt)
end

print(tab.x, tab.y, tab.z)
set_default(tab, 0)

print(tab.x, tab.y, tab.z)

local mt = {__index = function (t) return t.___ end}

local function set_default_v2(t, default)
    t.___ = default
    setmetatable(t, mt)
end

local tab2 = {x=100, y=200}
set_default_v2(tab2, 100)

print(tab2.x, tab2.y, tab2.z)
