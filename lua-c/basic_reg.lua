

package.cpath = package.cpath .. ";./?.so;"

local uv = require "uv"

local ud = require "ud"

local log1 = ud.new()
local log2 = ud.new()

function Init(...)
    print("init called", ...)
    uv.echo("hello mark")
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
