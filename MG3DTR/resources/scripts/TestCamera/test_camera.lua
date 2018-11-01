function start()

end

function update()
	camera = gameObject:GetCamera()
	camera:TranslateForward(-1.0)
	camera:TranslateRight(0.3)
	camera:TranslateUp(0.5)
end