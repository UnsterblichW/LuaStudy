
local meta = {}

function meta.sum(t)
    local s = 0
    for _, v in ipairs(t) do
        s = s + v
    end
    return s
end

local _M = {}

function _M.new(t)
    setmetatable(t, nil)
    return setmetatable(t, {__index = meta})
end

return _M
