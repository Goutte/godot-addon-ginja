extends Control


func _ready():
	
	print("Rendering template…")
	
	var tpl = "Hello, {{ name }} !"
	
	var msg = Ginja.render(tpl, {
		'name': "Nathanaël",
	})
	
	print(msg)

