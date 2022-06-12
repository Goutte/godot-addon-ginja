extends Control

var failed := false

func assertEquals(expected, actual):
	if expected != actual:
		printerr("Assertion failure !\nExpected\n%s\n  but got\n%s" % [expected, actual])
		self.failed = true


func _ready():
	
	print("Rendering template…")
	
	var engine = Ginja.new()
	
	var tpl = "Hello, {{ name }} !"
	var msg = engine.render(tpl, {
		'name': "Nathanaël",
	})
	assertEquals("Hello, Nathanaël !", msg)
	print(msg)
	
# 	engine.set_trim_blocks(true)
# 	tpl = """{% include "templates/welcome" %}"""
# 	msg = engine.render(tpl, {
# 		'name': "Ægies",
# 	})
	# WOW: Inja REWRITES our templates/welcome file, adds a newline before EOF ; WTF
# 	assertEquals("Welcome, Ægies !\n", msg)
# 	print(msg)
	
	
	
# 	var callback_yolo_0 = funcref(self, "call_yolo_0")
# 	engine.add_callback("yolo", 0, callback_yolo_0)
# 	tpl = """Bonjour {{ yolo() }} !"""
# 	msg = engine.render(tpl, {
# 		'name': "RebeK",
# 	})
# 	print(msg)
	
	print(self)
	print(get_instance_id())
	
	
	engine.add_callback("yolo", 0, self, "call_yolo_0")
	tpl = """Bonjour {{ yolo() }} !"""
	msg = engine.render(tpl, {
		'name': "RebeK",
	})
	assertEquals("Bonjour YOLO !", msg)
	print(msg)
	
	
	engine.add_callback("yolo", 1, self, "call_yolo_1")
	tpl = """Salud {{ yolo("amigo") }} !"""
	msg = engine.render(tpl, {})
	assertEquals("Salud AMIGO !", msg)
	print(msg)
	
	
	
	engine.set_expression('(', ')')
	tpl = """Bonjour ( name ) !"""
	msg = engine.render(tpl, {
		'name': ")(-)(…)(-)(",
	})
	print(msg)
	
	
	# See Ginja::set_templates_path  (needs work upstream)
	#engine.set_templates_path("templates/")
	#tpl = """{% include "welcome" %}"""
	#msg = engine.render(tpl, {
	#	'name': "$€£",
	#})
	#print(msg)
	
	if self.failed:
		printerr("ERROR")
	else:
		print("Done")

func call_yolo_0() -> String:
	print("SUCCESS ! CALLED YOLO() ! CHEERS ! 04:24:37")
	return "YOLO"

func call_yolo_1(msg: String) -> String:
	print("SUCCESS ! CALLED YOLO_1() ! CHEERS ! 04:24:37")
	return msg.to_upper()

	
	

func call_yolo() -> String:
	return "YOLO"

func test_using_custom_function():
	var engine = Ginja.new()
	engine.add_function("yolo", self, "call_yolo")
	var msg = engine.render("Bonjour {{ yolo() }} !", {})
	assertEquals("Bonjour YOLO !", msg)
