function start()
end

function update()
	if (input:GetKey(KEY_4)) then
		gameObject:GetTransform().position.y = gameObject:GetTransform().position.y - 1
	end

	if (input:GetKey(KEY_3)) then
		gameObject:GetTransform().position.y = gameObject:GetTransform().position.y + 1
	end

	if (input:GetKey(KEY_1)) then
		gameObject:GetTransform().position.x = gameObject:GetTransform().position.x - 1
	end

	if (input:GetKey(KEY_2)) then
		gameObject:GetTransform().position.x = gameObject:GetTransform().position.x + 1
	end

	if (input:GetKey(KEY_5)) then
		gameObject:GetTransform().position.z = gameObject:GetTransform().position.z - 1
	end

	if (input:GetKey(KEY_6)) then
		gameObject:GetTransform().position.z = gameObject:GetTransform().position.z + 1
	end
end