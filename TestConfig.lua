--test confg
width = 10
height = 20

--test table
--background = "BLUE"
background = {r = 0.3, g = 0.1, b = 0}

--test lua function in c
function f(x,y)
	return (x^2 * math.sin(y))/(1 - x) --luaL_openlibs(L) must be called in c 
end
