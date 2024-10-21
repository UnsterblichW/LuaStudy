

package.cpath = package.cpath .. ";./?.so;"

local userdata_reg = require "userdata_reg"

local closure_reg = require "closure_reg"

local log1 = userdata_reg.new()
local log2 = userdata_reg.new()

function Init(...)
    print("init called", ...)
    closure_reg.echo("hello mark")
end

function Loop(...)
    print("loop called", ...)
    log1:echo("hello mark")
    log1:echo("hello mark")
    log1:echo("hello mark")
    log1:echo("hello mark")
    log1:history()
end

function DeInit(...)
    print("deinit called", ...)
    log2:echo("hello mark")
    log2:echo("hello mark")
    log2:echo("hello mark")
    log2:echo("hello mark")
    log2:echo("hello mark")
    log2:echo("hello mark")
    log2:echo("hello mark")
    log2:echo("hello mark")
    log2:echo("hello mark")
    log2:echo("hello mark")
    log2:echo("hello mark")
    log2:echo("hello mark")
    log2:echo("hello mark")
    log2:echo("hello mark")
    log2:echo("hello mark")
    log2:echo("hello mark")
    log2:echo("hello mark")
    log2:history()
end
