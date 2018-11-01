a = true
sliderVal = 2
checkboxVal = false
radioVal = 0
inputText = "ceva"
inputIntVal = 0
inputFloatVal = 0.0

in21 = 0
in22 = 0

listCurrentItem = 0

arr = {"unu", "doi"}

function start()
end

function update()
	if (a == true) then
		GUI.NewFrame()
		a = GUI.Begin("Window", a)
		sliderVal = GUI.SliderInt("slider", sliderVal, 0, 5)
		if (GUI.Button("moveXNegative") == true) then
			gameObject:GetTransform().position.x = gameObject:GetTransform().position.x - 1
		end
		if (GUI.Button("moveXPositive") == true) then
			gameObject:GetTransform().position.x = gameObject:GetTransform().position.x + 1
		end

		checkboxVal = GUI.Checkbox("Checkbox", checkboxVal)
		radioVal = GUI.RadioButton("Radio 1", radioVal, 1)
		radioVal = GUI.RadioButton("Radio 2", radioVal, 2)
		radioVal = GUI.RadioButton("Radio 3", radioVal, 3)

		inputIntVal = GUI.InputInt("Input int", inputIntVal)
		inputFloatVal = GUI.InputFloat("Input float", inputFloatVal)

		listCurrentItem = GUI.ListBox("List box", listCurrentItem, {"ceva", "altceva", "inca ceva"})
		if GUI.BeginMenu("Menu") == true then
			if GUI.MenuItem("Ceva", "Hint") then
				print("clicked Ceva menu item")
			end
			if GUI.MenuItem("Altceva", "Hint") then
				print("clicked Altceva menu item")
			end
			GUI.EndMenu()
		end

		popupArr = {"el1", "el2", "el3", "el4"} 
		if GUI.Button("Open Popup") == true then
			GUI.OpenPopup("Popup")
		end
		if GUI.BeginPopup("Popup") == true then
			for k, v in pairs(popupArr) do
				if GUI.Selectable(v) == true then
					print("selected ", v)
				end
			end
			GUI.EndPopup()
		end


		GUI.End()
		GUI.Render()

		
	end
end