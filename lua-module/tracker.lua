
local function track(t)
    local proxy = {}

    local mt = {
        __index = function (_, k)
            print("getter element " .. tostring(k))
            return t[k]
        end,
        __newindex = function (_, k, v)
            print("setter elements " .. tostring(k) .. " to " .. tostring(v))
            t[k] = v
        end,
        __pairs = function()
            return function (_, k)
                local next_k, next_v = next(t, k)
                if next_k ~= nil then
                    print("traversing element " .. tostring(next_k))
                end
                return next_k, next_v
            end
        end,
        __len = function () return #t end
    }
    return setmetatable(proxy, mt)
end

local tab = {100,2000,"mark",track}

local proxy = track(tab)

local temp = proxy[1]

proxy[2] = "hello"

for k, v in pairs(proxy) do

end

local function read_only(t)
    local proxy = {}
    local mt = {
        __index = t,
        __newindex = function(_, k, v)
            error("attempt to update a read-only table")
        end
    }
    return setmetatable(proxy, mt)
end

local days = read_only({"sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday"})

print(days[1])

days[2] = "noday"

