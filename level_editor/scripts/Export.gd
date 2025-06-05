@tool
extends EditorScript

var level = "NONE"
var object_nodes := []

func _run():
	var scenes = ["Pens.tscn", "Silo.tscn", "Windmill.tscn", "BarnLevel.tscn"]
	var file = FileAccess.open("res://objects.json", FileAccess.WRITE)
	for scene_path in scenes:
		var scene = load(scene_path)
		var root = scene.instantiate()
		
		get_editor_interface().get_edited_scene_root().add_child(root)
		export(root)
		root.queue_free()

	var export_data := {
		"_comment": "DO NOT CHANGE THE ORDER OF CHARACTERS. ANIMALS MUST REMAIN AT TOP OF JSON AND IN ORDER",
		"objects": []
	}
	
	# Sort animals to top if needed, based on their names
	var animals = ["Chicken", "Pig", "Sheep", "Cow"]
	#var animals = ["Cow", "Pig", "Sheep", "Chicken"]
	var animal_objects = []
	var other_objects = []
	var animals_added = []

	for obj in object_nodes:	
		if obj["name"] in animals_added:
			printerr("cannot have duplicate players")
		elif obj["name"] in animals:
			animal_objects.append(obj)
			animals_added.append(obj["name"])
		else:
			other_objects.append(obj)

	animal_objects.sort_custom(_sort_animals)
	
	for animal in animal_objects:
		animal["level"] = "ALL"
		
	var all_objects = animal_objects + other_objects
	
	var id_counter := 0
	for obj in all_objects:
		obj["_id"] = id_counter
		id_counter += 1
		
	export_data["objects"] = all_objects
	
	file.store_string(JSON.stringify(export_data, "\t")) # Pretty print with tab
	file.close()

	print("Export completed to objects.json")
		

func export(root):
	if not root:
		printerr("No scene root!")
		return
		
	level = root.name
	if (root.name != "BARN") and (root.name != "PENS") and (root.name != "WINDMILL") and (root.name != "SILO"):
		printerr("level must be one of the four levels")
		return

	for child in root.get_children():
		if child is Node3D:
			var object_json := _process_node(child)
			if object_json:
				object_nodes.append(object_json)
		

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
	var is_transparent := false
	if node.has_meta("isTransparent"):
		is_transparent = node.get_meta("isTransparent")
			
	var data := {
		"level": level,
		"name": node.name,
		"active": false,
		"transform": {
			"position": _vec3_to_json(node.global_position),
			"rotation": _vec3_to_json(node.global_rotation_degrees),
			"scale": _vec3_to_json(node.scale),
		},
		"client": {
			"modelPath": get_model_path(node),
			"vertShaderPath": "../resources/shaders/material.vert",
			"fragShaderPath": "../resources/shaders/material.frag",
			"isTransparent": is_transparent
			
		},
		"server": server_data
	}

	return data


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
	var animals = ["Chicken", "Pig", "Sheep", "Cow"]
	var index_a = animals.find(a["name"])
	var index_b = animals.find(b["name"])
	return index_a < index_b
	
func _basis_to_json(basis: Basis) -> Dictionary:
	return {
		"right": _vec3_to_json(basis.x),
		"up": _vec3_to_json(basis.y),
		"forward": _vec3_to_json(basis.z) 
	}
	
#func _basis_to_json(basis: Basis) -> Dictionary:
	#return {
		#"right": _vec3_to_json(Vector3(basis.x.x, basis.y.x, basis.z.x)),
		#"up": _vec3_to_json(Vector3(basis.x.y, basis.y.y, basis.z.y)),
		#"forward": _vec3_to_json(Vector3(-basis.x.z, -basis.y.z, -basis.z.z))  # Negated Z to flip to right-handed
	#}
	
func get_level_number(name: String) -> int:
	if name.begins_with("Level "):
		return int(name.get_slice(" ", 1))
	return -1
