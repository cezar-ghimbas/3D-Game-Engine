function start()
	a = Vec3.new(2, 5, 8)
end

function update()
	--print("lua update function")
	--go = Engine.gameObject
	--t = gameObject:GetTransform()
	--position = t.position
	--position.y = position.y + 0.1
	--t.postion.y = 0.1
	--t.position = Vec3.new(0, t.position.y + 0.1, 0)
	--t.position = Vec3(0, 5, 0)
	--print(t.position.y)
	gameObject:GetTransform().position.y = gameObject:GetTransform().position.y + 0.1
end