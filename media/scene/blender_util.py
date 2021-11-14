import bpy

# list bone names & rename
ob = bpy.context.object
if ob.type == 'ARMATURE':
	armature = ob.data
	for bone in armature.bones:
		print(bone.name)
        bone.name = bone.name.replace("Armature", "ArmatureRobot")

# rename fcurves
ac = bpy.data.actions["Anim_0_Walk"]
for fc in ac.fcurves:
	fc.data_path = fc.data_path.replace("Armature", "ArmatureRobot")

# enumerate nla tracks
ob = bpy.context.object
ad = ob.animation_data
if ad:
    for i, track in enumerate(ad.nla_tracks):
        print(track.name)
        track.is_solo = True

# frame start & frame end
for ob in bpy.context.scene.objects:
    if not ob.animation_data:
        continue
    print(ob.name)
    for track in ob.animation_data.nla_tracks:
        print("  ", track.name)
        for strip in track.strips:
            print(" " * 4, strip.name, strip.frame_start, strip.frame_end)

# iterate through all keyframes Reference
# action types, in order to filter the wanted curves if needed
actionTypes = ('location','rotation_euler','rotation_quaternion','scale')
# get the object from which we want the animation data
obj = bpy.data.objects['Armature']
# if it has some animation
if obj.animation_data:
    # iterates over the wanted curves
    # data_path gives the action's type
    for curve in [c for c in obj.animation_data.action.fcurves if c.data_path.endswith( actionTypes )]:
        # array_index give the corresponding X, Y or Z index of the curve
        print( curve.data_path, curve.array_index )
        for key in curve.keyframe_points:
            # curve's points has a 'co' vector giving the frame and the value
            print( 'frame: ', key.co[0], ' value: ', key.co[1] )
    pass
else:
    print("no data")

# reposition animation 
for action in bpy.data.actions:
    for fcurve in action.fcurves[0:3]:
        print( fcurve.data_path, fcurve.array_index )
        kf_1st = fcurve.keyframe_points[1]
        kf_1st_value = fcurve.keyframe_points[1].co[1]
        if fcurve.array_index != 1:
            for kf in fcurve.keyframe_points[1:]:
                print( 'frame: ', kf.co.x, ' value: ', kf.co.y )
                kf.co.y -= kf_1st_value
                print( 'frame: ', kf.co.x, ' value: ', kf.co.y )

# make animation in place at origin
actions = [ bpy.data.objects['Armature'].animation_data.action ]
for action in actions:
	dist = 0
	idx = 0
	for fcurve in action.fcurves[0:3]:
		print( fcurve.data_path, fcurve.array_index )
		if ( abs( fcurve.keyframe_points[-1].co.y - fcurve.keyframe_points[0].co.y ) >= dist ):
			dist = abs( fcurve.keyframe_points[-1].co.y - fcurve.keyframe_points[0].co.y )
			idx = fcurve.array_index
	fcurve = action.fcurves[idx]
	for kf in fcurve.keyframe_points:
		print( 'frame: ', kf.co.x, ' value: ', kf.co.y )
		kf.co.y = 0.0


# animation length per channel
actions = [ bpy.data.objects['Armature'].animation_data.action ]
for action in actions:
	for fcurve in action.fcurves[0:-1]:
		if fcurve.array_index != 0:
			continue
		kf = fcurve.keyframe_points
		print( fcurve.data_path, fcurve.array_index, " || ", kf[0].co.x, " - ", kf[-1].co.x )


# round animation last key position
actions = [ bpy.data.objects['Armature'].animation_data.action ]
for action in actions:
	for fcurve in action.fcurves[0:-1]:
		kf = fcurve.keyframe_points
		kf[-1].co.x = ceil(kf[-1].co.x)

# offset animation keys
offset = 112
actions = [ bpy.data.objects['Armature'].animation_data.action ]
for action in actions:
	for fcurve in action.fcurves[0:-1]:
		print( fcurve.data_path, fcurve.array_index )
		for kf in fcurve.keyframe_points:
			print( 'frame: ', kf.co.x, ' value: ', kf.co.y )
			kf.co.x += offset

