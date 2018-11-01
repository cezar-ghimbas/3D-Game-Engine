function start()
	--meshRenderer = gameObject:GetMeshRenderer()
	--ambient = meshRenderer.material.ambient
	--meshRenderer.material.ambient = vec3(1.2, 3.4, 5.6)
	--meshRenderer.material.shininess = 100
	--meshRenderer.material = Material(vec3(1.2, 3.4, 5.6), vec3(0, 0, 0), vec3(0, 0, 0), 100)
	gameObject:GetMeshRenderer().material = Material.new(Vec3.new(1.2, 3.4, 5.6), Vec3.new(0, 0, 0), Vec3.new(0, 0, 0), 100)
	--print(meshRenderer.material.shininess)
end

function update()

end