
package.cpath = package.cpath .. ";./?.so;"

local ud = require "ud"

local log1 = ud.new()

log1.echo(log1, "hello mark1")
log1:echo("hello mark2")
log1:echo("hello mark3")
log1:echo("hello mark4")
log1:echo("hello mark5")
log1:echo("hello mark6")
log1:echo("hello mark7")
log1:echo("hello mark8")
log1:echo("hello mark9")
log1:echo("hello marka")
log1:echo("hello markb")
log1:echo("hello markc")
log1:echo("hello markd")
log1:echo("hello marke")

print("\n")
log1:history()
log1:echo("hello markf")

print("\n")
log1:history()

-- local log2 = ud.new()

-- log2.echo(log2, "hello king1")
-- log2:echo("hello king2")
-- log2:history()
