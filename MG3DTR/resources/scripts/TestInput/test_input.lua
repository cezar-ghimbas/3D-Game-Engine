function start()
end

function update()
	--print (KEY_A)
	if (input:GetKey(KEY_0) == true) then
		gameObject:GetTransform().position.y = gameObject:GetTransform().position.y + 20
	end
end