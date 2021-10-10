@echo off
setlocal ENABLEDELAYEDEXPANSION

set "filename=robot.blend"
set "pyScript=gameanim_gltf_track.py"
set "compDiv=cpu"

"C:\Users\NyersCorp\Documents\blender-cyc-2.93.4-windows-x64\blender.exe" -b %~dp0!filename! -o //frame -F PNG -x 1 -y -P %~dp0!pyScript! -- !compDiv! >> "blender_render_gameanim_gltf.log" 2>&1

exit /b
