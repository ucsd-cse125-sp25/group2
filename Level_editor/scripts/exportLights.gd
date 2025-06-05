@tool
extends EditorScript

var level = "NONE"
var object_nodes := []

func _run():
	var scenes = ["Main.tscn", "Silo.tscn", "Windmill.tscn", "Pens.tscn"]
	var file = FileAccess.open("res://jsons/lights.json", FileAccess.WRITE)
	for scene_path in scenes:
		var scene = load(scene_path)
		var root = scene.instantiate()
		
		get_editor_interface().get_edited_scene_root().add_child(root)
		export(root)
		root.queue_free()

	var export_data := {
		"lights": object_nodes
	}
	
	file.store_string(JSON.stringify(export_data, "\t")) # Pretty print with tab
	file.close()

	print("Export completed to jsons/lights.json")
		

func export(root):
	if not root:
		printerr("No scene root!")
		return
		
	level = root.name
	if (root.name != "BARN") and (root.name != "PENS") and (root.name != "WINDMILL") and (root.name != "SILO"):
		printerr("level must be one of the four levels")
		return

	for child in root.get_children():
		if child is OmniLight3D:
			var object_json := _process_node(child)
			if object_json:
				object_nodes.append(object_json)
		

func _process_node(node: OmniLight3D) -> Dictionary:
			
	var data := {
		"level": level,
		"position": _vec3_to_json(node.global_position),
		"color": _get_color(node),
		"radius": node.omni_range,
		"useAttenuation": true
	}

	return data


func round_decimal(value: float, decimals: int = 3) -> float:
	var factor = pow(10, decimals)
	return round(value * factor) / factor
	
func _vec3_to_json(vec: Vector3) -> Dictionary:
	return {
		"x": round_decimal(vec.x),
		"y": round_decimal(vec.y),
		"z": round_decimal(vec.z)
	}

func _get_color(node: OmniLight3D) -> Dictionary:
	return {
		"x": node.light_energy,
		"y": node.light_energy,
		"z": node.light_energy,
	}
	
