extends Control


func _ready():
	
	print("Rendering template…")
	
	var tpl = "Hello, {{ name }} !"
	var engine = Ginja.new()
	var msg = engine.render(tpl, {
		'name': "Nathanaël",
	})
	
	print(msg)

