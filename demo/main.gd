extends Control

var failed := false

func assertEquals(expected, actual):
	if expected != actual:
		printerr("Assertion failure !\nExpected\n%s\n  but got\n%s" % [expected, actual])
		self.failed = true


func _ready():
	
	print("Rendering template…")
	
	var ginja = Ginja.new()
	
	var tpl = "Hello, {{ name }} !"
	var msg = ginja.render(tpl, {
		'name': "Nathanaël",
	})
	assertEquals("Hello, Nathanaël !", msg)
	print(msg)
	
	ginja.set_trim_blocks(true)
	tpl = """{% include "templates/welcome" %}"""
	msg = ginja.render(tpl, {
		'name': "Ægies",
	})
	# WOW: Inja REWRITES our templates/welcome file, adds a newline before EOF ; WTF
	assertEquals("Welcome, Ægies !\n", msg)
	print(msg)
	
	
# 	var callback_yolo_0 = funcref(self, "call_yolo_0")
# 	ginja.add_callback("yolo", 0, callback_yolo_0)
# 	tpl = """Bonjour {{ yolo() }} !"""
# 	msg = ginja.render(tpl, {
# 		'name': "RebeK",
# 	})
# 	print(msg)
	
	
	#print(self)
	#print(get_instance_id())
	
	
	ginja.add_callback("yolo", 0, self, "call_yolo_0")
	tpl = """Bonjour {{ yolo() }} !"""
	msg = ginja.render(tpl, {
		'name': "RebeK",
	})
	assertEquals("Bonjour YOLO !", msg)
	print(msg)
	
	
	ginja.add_callback("yolo", 1, self, "call_yolo_1")
	tpl = """Salud {{ yolo("amigo") }} !"""
	msg = ginja.render(tpl, {})
	assertEquals("Salud AMIGO !", msg)
	print(msg)
	
	
	ginja.add_callback("repeat", 2, self, "call_repeat")
	tpl = """{{ repeat("ha", 3) }}"""
	msg = ginja.render(tpl, {})
	assertEquals("hahaha", msg)
	print(msg)
	
	
	ginja.set_expression('(', ')')
	tpl = """Bonjour ( name ) !"""
	msg = ginja.render(tpl, {
		'name': ")(-)(…)(-)(",
	})
	assertEquals("Bonjour )(-)(…)(-)( !", msg)
	print(msg)
	
	
	# See Ginja::set_templates_path  (needs work upstream)
	#ginja.set_templates_path("templates/")
	#tpl = """{% include "welcome" %}"""
	#msg = ginja.render(tpl, {
	#	'name': "$€£",
	#})
	#print(msg)
	
	if self.failed:
		printerr("ERROR")
	else:
		print("Done")


func call_yolo_0() -> String:
	#print("SUCCESS ! CALLED YOLO() ! CHEERS ! 04:24:37")
	return "YOLO"

func call_yolo_1(msg: String) -> String:
	return msg.to_upper()

func call_repeat(msg: String, times: int) -> String:
	return msg.repeat(times)




func call_yolo() -> String:
	return "YOLO"

func test_using_custom_function():
	var ginja = Ginja.new()
	ginja.add_function("yolo", self, "call_yolo")
	var msg = ginja.render("Bonjour {{ yolo() }} !", {})
	assertEquals("Bonjour YOLO !", msg)
