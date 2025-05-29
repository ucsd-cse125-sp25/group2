@tool
extends EditorScript

func _run():
	var export_data := {
		"_comment": "DO NOT CHANGE THE ORDER OF CHARACTERS. ANIMALS MUST REMAIN AT TOP OF JSON AND IN ORDER",
		"objects": []
	}

	var root = get_editor_interface().get_edited_scene_root()
	if not root:
		printerr("No scene root!")
		return

	var object_nodes := []

	for child in root.get_children():
		if child is Node3D:
			var object_json := _process_node(child)
			if object_json:
				object_nodes.append(object_json)

	# Sort animals to top if needed, based on their names
	var animals = ["Chicken", "Sheep", "Pig", "Cow"]
	#var animals = ["Cow", "Pig", "Sheep", "Chicken"]
	var animal_objects = []
	var other_objects = []

	for obj in object_nodes:
		if obj["name"] in animals:
			animal_objects.append(obj)
		else:
			other_objects.append(obj)

	animal_objects.sort_custom(_sort_animals)
	export_data["objects"] = animal_objects + other_objects

	var file = FileAccess.open("res://objects.json", FileAccess.WRITE)
	file.store_string(JSON.stringify(export_data, "\t")) # Pretty print with tab
	file.close()

	print("Export completed to objects.json")

func _process_node(node: Node3D) -> Dictionary:
	var server_data := {
		"static": is_node_static(node),
		"interaction": get_interaction(node),
		"colliders": get_all_colliders(node),
	}
	if node is MetaDATA:
		var meta: MetaDATA = node
		if meta.is_keyPad:
			server_data["keypad"] = {
				"correctSequence": meta.correctSequence
			}
			
	var data := {
		"name": node.name,
		"active": node.visible,
		"transform": {
			"position": _vec3_to_json(node.global_position),
			"rotation": _vec3_to_json(node.global_rotation_degrees),
			"scale": _vec3_to_json(node.scale),
		},
		"client": {
			"modelPath": get_model_path(node),
			"vertShaderPath": "../resources/shaders/material.vert",
			"fragShaderPath": "../resources/shaders/material.frag"
		},
		"server": server_data
	}

	return data

func _get_half_extents(node: Node3D) -> Dictionary:
	for child in node.get_children():
		if child is CollisionShape3D:
			var shape = child.shape
			if shape is BoxShape3D:
				var extents = shape.size * 0.5 * node.scale
				return _vec3_to_json(extents)
	return {"x": 0.0, "y": 0.0, "z": 0.0}  # Default

func get_all_colliders(node: Node3D) -> Array:
	var colliders := []
	for child in node.get_children():
		if child is CollisionShape3D:
			var shape: Shape3D = child.shape
			if shape is BoxShape3D:
				var is_trigger := false
				if child.has_meta("isTrigger"):
					is_trigger = child.get_meta("isTrigger")
					
				var extents = shape.size * 0.5
				
				var collider_data := {
					"center": _vec3_to_json(child.transform.origin * node.scale),
					"halfExtents": _vec3_to_json(extents),
					"orientation": _basis_to_json(child.global_transform.basis),
					"isTrigger": is_trigger
				}
				colliders.append(collider_data)
	return colliders

func round_decimal(value: float, decimals: int = 3) -> float:
	var factor = pow(10, decimals)
	return round(value * factor) / factor
	
func _vec3_to_json(vec: Vector3) -> Dictionary:
	return {
		"x": round_decimal(vec.x),
		"y": round_decimal(vec.y),
		"z": round_decimal(vec.z)
	}
func is_node_static(node: Node) -> bool:
	if node is MetaDATA:
		return node.is_static
	return true
	
func get_interaction(node: Node) -> String:
	if node is MetaDATA:
		return node.interaction
	return "NONE"
func get_model_path(node: Node) -> String:
	if node is MetaDATA:
		return node.model_path
	return "TODO"
	
func _sort_animals(a, b) -> bool:
	var animals = ["Chicken", "Sheep", "Pig", "Cow"]
	var index_a = animals.find(a["name"])
	var index_b = animals.find(b["name"])
	return index_a < index_b
	
func _basis_to_json(basis: Basis) -> Dictionary:
	return {
		"right": _vec3_to_json(basis.x),
		"up": _vec3_to_json(basis.y),
		"forward": _vec3_to_json(-basis.z)  # Negate Z to match right-handed forward
	}
