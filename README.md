# ImGuiDock
Ugly docking code for ImGui

For now with most of the features missing including undocking to a platform window, saving layout and closing tabs.
Will add all that later when i find some time...

Use like this:

	Dock dock1;
	dock1.initialize("Dock1", true, ImVec2(), [](ImVec2 area) {
		ImGui::Text("Hello :)");
	});
	
	Dock dock2;
	dock2.initialize("Dock2", true, ImVec2(250,300), [](ImVec2 area) {
		ImGui::Text("Hello From Dock2 :)");
	});
	
	Dockspace dockspace;
	dockspace.dock(&dock1, ImGuiDock::DockSlot::None, 0, true);
	dockspace.dock(&dock2, ImGuiDock::DockSlot::Right, 250, true);


In render loop call:

  dockspace.updateAndDraw(workAreaSize);
  
Under MIT Licence
