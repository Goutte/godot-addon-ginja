extends Control


func _ready():
	
	print("Rendering template…")
	
	var tpl = "Hello, {{ name }} !"
	var engine = Ginja.new()
	var msg = engine.render(tpl, {
		'name': "Nathanaël",
	})
	print(msg)
	
	
	tpl = """{% include "templates/welcome" %}"""
	msg = engine.render(tpl, {
		'name': "Ægies",
	})
	print(msg)
	
	
	# See Ginja::set_templates_path  (needs work upstream)
	#engine.set_templates_path("templates/")
	#tpl = """{% include "welcome" %}"""
	#msg = engine.render(tpl, {
	#	'name': "$€£",
	#})
	#print(msg)

