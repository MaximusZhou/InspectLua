function printf(...) 
	io.write(string.format(unpack(arg)))
end

--expand the metatable Account,it is metatable of all instance of registed class
function Account:show()
	printf("Account balance = $%0.02f\n", self:balance())
end

parent = {}

function parent:rob(amount)
	amount = amount or self:balance()
	self:withdraw(amount)
	return amount
end

getmetatable(Account).__index = parent

function main()
	print('a =', a)
	print('b =', b)
	print('metatable =', getmetatable(a))-- the metatable of a is Account,Not metatable table
	print('Account =', Account)  
	table.foreach(Account, print)

	a:show() a:deposit(50.30) a:show() a:withdraw(25.10) a:show()
	a:rob(10) a:show()
	--debug.debug()
end

--script_obj = Account() --error:bad argument #1 to 'Account' (number expected, got no value)
script_obj1 = Account(10)
script_obj2 = Account:new(20) 

print("script_obj1 = ",script_obj1)
print("script_obj2 = ",script_obj2)
