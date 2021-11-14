from bpy import context
from bpy import path
from bpy import ops
from bpy import app
from bpy import data
from math import pi

import sys
argv = sys.argv
argv = argv[argv.index("--") + 1:]  # get all args after "--"

print(app.version_string)
blenderVersion = app.version
blenderVersionChar = app.version_char
blenderBuildBranch = app.build_branch

scene = context.scene
scene.render.image_settings.file_format = 'PNG'
scene.render.image_settings.compression = 25
sceneName = path.basename(context.blend_data.filepath)

scene.render.resolution_percentage = 100

for ob in context.scene.objects:
    if not ob.animation_data:
        continue
    print(ob.name)
    
    maxFrame = 0
    for track in ob.animation_data.nla_tracks:
        print("  ", track.name)
        track.is_solo = True
        for strip in track.strips:
            if strip.frame_end > maxFrame:
                maxFrame = strip.frame_end
    
    maxFrame *= 2
    for track in ob.animation_data.nla_tracks:
        print("  ", track.name)
        track.is_solo = True
        for strip in track.strips:
            print(" " * 4, strip.name, strip.frame_start, strip.frame_end)
            for iFrame in range(int(strip.frame_start),int(maxFrame)+1):
                scene.frame_set(1+(iFrame-1)%(int(strip.frame_end)))
                context.scene.render.filepath = "C:\\Users\\...\\Documents\\"+track.name.lower()+"_"+'{0:06d}'.format(iFrame)
                ops.render.render(write_still=True)

print('DONE')
