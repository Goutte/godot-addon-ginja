extends Sprite2D
class_name GooCursor

# A Cursor with a trail.
# Like the one in World of Goo.

# Set up
# ------
# The material and shader will both be automatically generated during ready.
# This Sprite has to stay in front of the camera. Best put it in a CanvasLayer.

@export
var base_size := 10.0
@export
var trail_length := 32 # number of circles in the trail
@export
var pulsation_frequency := 2.2222
@export
var pulsation_amplitude := 0.1618

# These need to be as small as possible, for performance.
# When it's too small, you'll see the trail being clipped.
@export
var canvas_width := 180
@export
var canvas_height := 180

################################################################################

var trail_positions := Array()
var trail_sizes := Array()
var trail_max_delta := base_size * 0.606 # max distance between 2 trail positions
var current_frame := 0



func _ready():
	initialize_trail_positions()
	initialize_trail_sizes()
	generate_texture()
	generate_material()
	print(generate_shader_code())	
	
	Input.set_mouse_mode(Input.MOUSE_MODE_HIDDEN)


func _input(event):
	if event is InputEventMouseMotion:
		update_trail(get_mouse_pos(event))
		update_shader()
		position = event.global_position


func _process(_delta):
	current_frame += 1
	update_trail(trail_positions[0])
	pulsate()
	update_shader()


func get_mouse_pos(event):
	var p = event.global_position
	var h = get_viewport_rect().size.y
#	p.y = h - p.y
	return p

func initialize_trail_positions():
	var v = Vector2()
	for i in range(trail_length):
		trail_positions.append(v)

func initialize_trail_sizes():
	trail_sizes = Array()
	for i in range(trail_length):
		# Hyperbolic
		#var size = 1.0/(1.0+i)
		# Linear
		var size = 1.0 * (trail_length - i) / trail_length
		# Other
		# … (from image curve?)
		trail_sizes.append(size)

func update_shader():
	for i in range(trail_positions.size()):
		self.material.set_shader_param(
			"trail_position_%d" % i,
			trail_positions[i]
		)

func update_trail(new_head):
	trail_positions.push_front(new_head)
	trail_positions.pop_back()
	for i in range(1, trail_positions.size()):
		var forcedLength: Vector2 = Vector2.ONE * trail_max_delta * trail_sizes[i]
		var difference: Vector2 = (
			trail_positions[i]
			-
			trail_positions[i-1]
		)
		trail_positions[i] = (
			trail_positions[i-1]
			+
			difference.normalized() * forcedLength
		)

func pulsate():
	var t = Time.get_ticks_msec() * 0.001
	for i in range(trail_positions.size()):
		var d = -i * TAU / trail_positions.size() * 0.618
		self.material.set_shader_param(
			"trail_size_%d" % i,
			trail_sizes[i] + abs(pulsation_amplitude * sin(t * pulsation_frequency + d))
		)

func get_shader_template():
	# The following is NOT a comment. Could also fetch it from File?
	return \
"""
shader_type canvas_item;

uniform float size = 10.0;
uniform float outline_1 = 2.5;
uniform float outline_2 = 1.0;

{% for trail_size in trail_sizes %}
uniform vec2 trail_position_{{ loop.index }} = vec2(0.0, 0.0);
uniform float trail_size_{{ loop.index }} = {{ trail_size }};
{% endfor %}

bool is_in_goo(vec2 here, float offset) {
	return
{% for i in range(trail_length) %}
		{% if not loop.is_first %}||{% endif %}
		length((here-trail_position_{{ i }})) <= size * trail_size_{{ i }} + offset
{% endfor %}
	;
}

void fragment() {
	COLOR = vec4(0.0, 0.0, 0.0, 0.0);
	
	vec2 here = FRAGCOORD.xy;
	
	if (is_in_goo(here, 0.0)) {
		// Inside
		COLOR = vec4(0.0, 0.0, 0.0, 1.0);
	} else if (is_in_goo(here, outline_1)) {
		// Opaque outline
		COLOR = vec4(1.0, 1.0, 1.0, 1.0);
	} else if (is_in_goo(here, outline_1 + outline_2)) {
		// Transparent outline
		COLOR = vec4(1.0, 1.0, 1.0, 0.4);
	}
}
"""

func generate_shader_code():
	# Requires the ginja module https://framagit.org/godotrio/godot-module-ginja
	var ginja = Ginja.new()
	
	return ginja.render(get_shader_template(), {
		'trail_length': self.trail_length,
		'trail_sizes': self.trail_sizes,
	})

func generate_texture():
	texture = ImageTexture.new()
	var image = Image.new()
	image.create(self.canvas_width, self.canvas_height, false, image.FORMAT_RGB8)
	texture.image = image

func generate_material():
	material = ShaderMaterial.new()
	material.shader = Shader.new()
	material.shader.set_code(generate_shader_code())


