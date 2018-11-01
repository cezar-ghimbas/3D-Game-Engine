function start()
	
end

function update()
	gameObject:GetLight().color = Vec3.new(0, math.cos(os.time()), 0)
end