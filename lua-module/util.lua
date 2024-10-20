
-- 相当于 c/c++ 中的单例模式
local _M = {}

--[[
_M.add = function(a, b)
    return a+b
end
]]

local function add(a,b)
    return a+b
end

function _M.add(a, b)
    return add(a,b)
end

return _M
